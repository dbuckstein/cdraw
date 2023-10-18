/*
* Copyright 2023 Daniel S. Buckstein
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/

/*
* cdrawTimer.c
* Common implementation of high-precision timer.
*/

#include "cdraw/cdrawPlatform/cdrawTimer.h"


/******************************************************************************
* Private/internal functions.
******************************************************************************/

CDRAW_INL bool cdrawTimerStateInternalValidateBounds(cdrawTimerState const* const timerState)
{
	return ((timerState->t1 - timerState->t0) == timerState->dtx)	// bounds separated by expected interval duration
		&& (timerState->t >= timerState->t0)						// time is greater than or equal to lower bound
		&& (timerState->t < timerState->t1);						// time is less than upper bound (which is the lower bound of the next interval)
}

CDRAW_INL bool cdrawTimerStateInternalValidate(cdrawTimerState const* const timerState)
{
	return cdrawTimerStateInternalValidateBounds(timerState)	// bounds are valid
		&& ((timerState->t0 % timerState->dtx) == 0)			// lower bound is divisible by expected interval duration
		&& ((timerState->t1 % timerState->dtx) == 0);			// upper bound is divisible by expected interval duration
}

CDRAW_INL bool cdrawTimerInternalStepNormal(cdrawTimer* const timer, ctime_t dt_track)
{
	// simple calculation of steps clipped and remaining step time
	// always jump to final time
	timer->t_track += dt_track;

	// this algorithm works in both directions; sign check is in first step in any case
	if (timer->t_track >= timer->state.t1 || timer->t_track < timer->state.t0)
	{
		// remainder of interval length in time itself is same in new interval; choose based on sign of time
		dt_track = (timer->t_track >= 0) ? (timer->t_track % timer->state.dtx) : (timer->state.dtx + timer->t_track % timer->state.dtx);

		// advance state
		timer->state.dt = (timer->t_track - timer->state.t);
		timer->state.t0 = (timer->t_track - dt_track);
		timer->state.t1 = (timer->state.t0 + timer->state.dtx);
		timer->state.t = timer->t_track;
		failassert(cdrawTimerStateInternalValidate(&timer->state));
		return true;
	}
	return false;
}

CDRAW_INL void cdrawTimerInternalStepSpecial(cdrawTimer* const timer, ctime_t dt_track)
{
	// always tick and increment time, ensuring validity
	timer->state.dt = dt_track;
	timer->state.t0 = timer->state.t = (timer->state.t + timer->state.dt);
	timer->state.t1 = (timer->state.t0 + timer->state.dtx);
	failassert(cdrawTimerStateInternalValidateBounds(&timer->state));
}

CDRAW_INL result_t cdrawTimerInternalStepMulti(cdrawTimer* const timer, ctime_t dt_track, ctime_t* const dt_track_out, ctime_t const dt_step, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out)
{
	result_t count = 0;

	// repeat time transfer while there is more delta left than the current interval
	while (dt_track > timer->state.dtx)
	{
		// transfer time and store state (time goes up, delta goes down)
		dt_track -= timer->state.dtx;
		timer->t_track += dt_step;
		timer->state.dt = dt_step;
		timer->state.t0 += dt_step;
		timer->state.t1 += dt_step;
		timer->state.t = timer->t_track;
		failassert(cdrawTimerStateInternalValidateBounds(&timer->state));
		*callbackResult_out = callback(callbackArg_opt, &timer->state);
		++count;
	}
	
	// send back final time
	*dt_track_out = dt_track;
	return count;
}

CDRAW_INL int64_t cdrawTimerInternalReduceRationalGCD(int64_t numerator, int64_t denominator, int64_t* const numerator_out, int64_t* const denominator_out, int64_t* const gcd_out_opt)
{
	failassertret(numerator_out && denominator_out, 0);

	int64_t i = 0, r = denominator, n = numerator, d = 0;

	// iterate to find GCD
	while (r)
	{
		d = r;		// previous remainder becomes denominator
		r = n % d;	// calculate new remainder
		n = d;		// previous denominator becomes numerator
	}

	// d now holds the GCD
	if (d)
	{
		// reduce fraction
		numerator /= d;
		denominator /= d;

		// calculate integer and remainder
		i = numerator / denominator;
		numerator %= denominator;
	}

	// store final numerator and denominator, return integer
	*numerator_out = numerator;
	*denominator_out = denominator;
	if (gcd_out_opt)
		*gcd_out_opt = d;
	return i;
}


#define cdraw_timer_count2sec(count,cps)	((count) / (stime_t)(cps))

bool cdrawTimerInternalSysInit(ctime_t* const cps_out);
bool cdrawTimerInternalSysStep(ctime_t* const t_out);


/******************************************************************************
* Public functions.
******************************************************************************/

result_t cdrawTimerReset(cdrawTimer* const timer)
{
	result_init();
	asserterr_ptr(timer, errcode_invalidarg);
	timer->t_track = timer->t_scale = 0;
	timer->state.t = timer->state.t0 = timer->state.t1 = timer->state.dt = timer->state.dtx = timer->state.cps;
	result_return();
}

result_t cdrawTimerStateGetElapsedTime(cdrawTimerState const* const timerState, stime_t* const elapsedTime_out)
{
	result_init();
	asserterr_ptr(timerState, errcode_invalidarg);
	asserterr_ptr(elapsedTime_out, errcode_invalidarg);
	*elapsedTime_out = cdraw_timer_count2sec(timerState->t, timerState->cps);
	result_return();
}

result_t cdrawTimerStateGetDeltaTime(cdrawTimerState const* const timerState, stime_t* const deltaTime_out)
{
	result_init();
	asserterr_ptr(timerState, errcode_invalidarg);
	asserterr_ptr(deltaTime_out, errcode_invalidarg);
	*deltaTime_out = cdraw_timer_count2sec(timerState->dt, timerState->cps);
	result_return();
}

result_t cdrawTimerStateGetIntervalTime(cdrawTimerState const* const timerState, stime_t* const intervalTime_out)
{
	result_init();
	asserterr_ptr(timerState, errcode_invalidarg);
	asserterr_ptr(intervalTime_out, errcode_invalidarg);
	*intervalTime_out = cdraw_timer_count2sec(timerState->dtx, timerState->cps);
	result_return();
}

result_t cdrawTimerStateGetOverstepRatio(cdrawTimerState const* const timerState, stime_t* const overstepRatio_out)
{
	result_init();
	asserterr_ptr(timerState, errcode_invalidarg);
	asserterr_ptr(overstepRatio_out, errcode_invalidarg);
	ctime_t const overstep = (timerState->t - timerState->t0);
	*overstepRatio_out = cdraw_timer_count2sec(overstep, timerState->dtx);
	result_return();
}

result_t cdrawTimerStateGetUnderstepRatio(cdrawTimerState const* const timerState, stime_t* const understepRatio_out)
{
	result_init();
	asserterr_ptr(timerState, errcode_invalidarg);
	asserterr_ptr(understepRatio_out, errcode_invalidarg);
	ctime_t const understep = (timerState->t - timerState->t1);
	*understepRatio_out = cdraw_timer_count2sec(understep, timerState->dtx);
	result_return();
}

result_t cdrawTimerStateGetDeltaTimeRatio(cdrawTimerState const* const timerState, stime_t* const deltaTimeRatio_out)
{
	result_init();
	asserterr_ptr(timerState, errcode_invalidarg);
	asserterr_ptr(deltaTimeRatio_out, errcode_invalidarg);
	*deltaTimeRatio_out = cdraw_timer_count2sec(timerState->dt, timerState->dtx);
	result_return();
}

result_t cdrawTimerInitSystem(cdrawTimer* const timer)
{
	result_init();
	asserterr_ptr(timer, errcode_invalidarg);
	bool result = cdrawTimerInternalSysInit(&timer->state.cps);
	asserterr(result, errcode_timer_sample);
	result = cdrawTimerInternalSysStep(&timer->t_track);
	asserterr(result, errcode_timer_sample);

	// set values, considered an "always tick" timer with the exception of a set rate and time tracker
	timer->state.t1 = timer->state.dtx = timer->state.cps;
	timer->state.t0 = timer->state.dt = timer->state.t = timer->t_scale = 0;
	failassert(cdrawTimerStateInternalValidate(&timer->state));
	result_return();
}

result_t cdrawTimerStepSystem(cdrawTimer* const timer)
{
	result_init();
	asserterr_ptr(timer, errcode_invalidarg);
	asserterr(timer->state.cps, errcode_invalidarg);
	bool const result = cdrawTimerInternalSysStep(&timer->t_track);
	asserterr(result, errcode_timer_sample);

	// always tick, tracking time and delta
	ctime_t const dt_track = (timer->t_track - timer->state.t);
	cdrawTimerInternalStepSpecial(timer, dt_track);
	result_inc(1);
	result_return();
}

result_t cdrawTimerStepSystemClip(cdrawTimer* const timer)
{
	result_init();
	asserterr_ptr(timer, errcode_invalidarg);
	asserterr(timer->state.cps, errcode_invalidarg);
	bool const result = cdrawTimerInternalSysStep(&timer->t_track);
	asserterr(result, errcode_timer_sample);

	// always tick, tracking time and delta
	ctime_t const dt_track = ((timer->t_track - timer->state.t) % timer->state.cps);
	cdrawTimerInternalStepSpecial(timer, dt_track);
	result_inc(1);
	result_return();
}

result_t cdrawTimerStepSystemCallback(cdrawTimer* const timer, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out_opt)
{
	// if no callback, same as normal
	if (!callback)
		return cdrawTimerStepSystem(timer);

	result_init();
	asserterr_ptr(timer, errcode_invalidarg);
	asserterr(timer->state.cps, errcode_invalidarg);
	cdrawTimerStepSystem(timer);
	result_t const callbackResult = callback(callbackArg_opt, &timer->state);
	if (callbackResult_out_opt)
		*callbackResult_out_opt = callbackResult;
	result_inc(1);
	result_return();
}

result_t cdrawTimerStepSystemClipCallback(cdrawTimer* const timer, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out_opt)
{
	// if no callback, same as clip
	if (!callback)
		return cdrawTimerStepSystemClip(timer);

	result_init();
	asserterr_ptr(timer, errcode_invalidarg);
	asserterr(timer->state.cps, errcode_invalidarg);
	cdrawTimerStepSystemClip(timer);
	result_t const callbackResult = callback(callbackArg_opt, &timer->state);
	if (callbackResult_out_opt)
		*callbackResult_out_opt = callbackResult;
	result_inc(1);
	result_return();
}

result_t cdrawTimerStepSystemMultiCallback(cdrawTimer* const timer, result_t* const count_out_opt, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out_opt)
{
	// if no callback, same as normal
	if (!callback)
		return cdrawTimerStepSystem(timer);

	result_init();
	asserterr_ptr(timer, errcode_invalidarg);
	asserterr(timer->state.cps, errcode_invalidarg);

	result_t callbackResult = 0, count = 1;
	ctime_t dt_track;
	bool result = cdrawTimerInternalSysStep(&dt_track);
	asserterr(result, errcode_timer_sample);

	// tick for every second passed and once at the end
	dt_track -= timer->state.t;
	count += cdrawTimerInternalStepMulti(timer, dt_track, &dt_track, timer->state.cps, callback, callbackArg_opt, &callbackResult);
	cdrawTimerInternalStepSpecial(timer, dt_track);
	callbackResult = callback(callbackArg_opt, &timer->state);
	if (callbackResult_out_opt)
		*callbackResult_out_opt = callbackResult;
	if (count_out_opt)
		*count_out_opt = count;
	result_inc(1);
	result_return();
}

result_t cdrawTimerSet(cdrawTimer* const timer, cdrawTimer const* const parent, uint16_t const ticksPerSecond)
{
	result_init();
	asserterr_ptr(timer, errcode_invalidarg);
	asserterr_ptr(parent, errcode_invalidarg);
	asserterr(parent->state.cps, errcode_invalidarg);
	if (ticksPerSecond > 0)
	{
		// time potentially increases faster than expected
		//	e.g. root->cps = 10,000,000, parent->cps = 60, cps = 24
		//		-> parent->t_scale = 3
		//		-> parent->state.dtx = 500,000
		//		-> parent->state.cps = 30,000,000
		// calculate interval to handle remainder
		timer->state.dtx = cdrawTimerInternalReduceRationalGCD(parent->state.cps, ticksPerSecond, &timer->t_track, &timer->t_scale, 0);
		asserterr(timer->state.dtx, errcode_timer_sample);
		timer->state.t1 = timer->state.dtx = (timer->state.dtx * timer->t_scale + timer->t_track);
		timer->state.t0 = timer->state.dt = timer->state.t = timer->t_track = 0;
		timer->state.cps = timer->t_scale * parent->state.cps;
	}
	else
	{
		// functionally equivalent to "always tick"
		timer->state.t1 = timer->state.dtx = 1;
		timer->state.t0 = timer->state.dt = timer->state.t = 0;
		timer->state.cps = parent->state.cps;
		timer->t_track = timer->t_scale = 0;
	}
	failassert(cdrawTimerStateInternalValidate(&timer->state));
	result_return();
}

result_t cdrawTimerStep(cdrawTimer* const timer, ctime_t const dt_parent)
{
	result_init();
	asserterr_ptr(timer, errcode_invalidarg);
	asserterr(timer->state.cps, errcode_invalidarg);
	if (timer->t_scale)
	{
		ctime_t const dt_track = (dt_parent * timer->t_scale);
		if (cdrawTimerInternalStepNormal(timer, dt_track))
			result_inc(1);
	}
	else
	{
		cdrawTimerInternalStepSpecial(timer, dt_parent);
		result_inc(1);
	}
	result_return();
}

result_t cdrawTimerStepClip(cdrawTimer* const timer, ctime_t const dt_parent)
{
	result_init();
	asserterr_ptr(timer, errcode_invalidarg);
	asserterr(timer->state.cps, errcode_invalidarg);
	if (timer->t_scale)
	{
		ctime_t const dt_track = ((dt_parent * timer->t_scale) % timer->state.dtx);
		if (cdrawTimerInternalStepNormal(timer, dt_track))
			result_inc(1);
	}
	else
	{
		cdrawTimerInternalStepSpecial(timer, dt_parent);
		result_inc(1);
	}
	result_return();
}

result_t cdrawTimerStepCallback(cdrawTimer* const timer, ctime_t const dt_parent, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out_opt)
{
	// if no callback, same as normal
	if (!callback)
		return cdrawTimerStep(timer, dt_parent);

	result_init();
	asserterr_ptr(timer, errcode_invalidarg);
	asserterr(timer->state.cps, errcode_invalidarg);
	if (cdrawTimerStep(timer, dt_parent))
	{
		result_t const callbackResult = callback(callbackArg_opt, &timer->state);
		if (callbackResult_out_opt)
			*callbackResult_out_opt = callbackResult;
		result_inc(1);
	}
	result_return();
}

result_t cdrawTimerStepClipCallback(cdrawTimer* const timer, ctime_t const dt_parent, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out_opt)
{
	// if no callback, same as clip
	if (!callback)
		return cdrawTimerStepClip(timer, dt_parent);
	
	result_init();
	asserterr_ptr(timer, errcode_invalidarg);
	asserterr(timer->state.cps, errcode_invalidarg);
	if (cdrawTimerStepClip(timer, dt_parent))
	{
		result_t const callbackResult = callback(callbackArg_opt, &timer->state);
		if (callbackResult_out_opt)
			*callbackResult_out_opt = callbackResult;
		result_inc(1);
	}
	result_return();
}

result_t cdrawTimerStepMultiCallback(cdrawTimer* const timer, ctime_t const dt_parent, result_t* const count_out_opt, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out_opt)
{
	// if no callback, same as normal
	if (!callback)
		return cdrawTimerStep(timer, dt_parent);
	
	result_init();
	asserterr_ptr(timer, errcode_invalidarg);
	asserterr(timer->state.cps, errcode_invalidarg);

	// multi-step algorithm: 
	//	-> while remaining time exceeds expected, transfer time to elapsed and take full step
	//	-> transfer remaining time and handle final step if needed
	result_t callbackResult = 0;
	result_t count = 0;

	// track time normally
	if (timer->t_scale)
	{
		// forward
		if (dt_parent > 0)
		{
			ctime_t dt_track_f = (dt_parent * (+timer->t_scale));
			count = cdrawTimerInternalStepMulti(timer, dt_track_f, &dt_track_f, (+timer->state.dtx), callback, callbackArg_opt, &callbackResult);

			// final step: transfer remaining to time
			timer->t_track += dt_track_f;
			if (timer->t_track >= timer->state.t1)
			{
				// store state: calculate actual delta, store time, step interval
				// note: expected interval does not change
				timer->state.dt = timer->t_track - timer->state.t;
				timer->state.t = timer->t_track;
				timer->state.t0 = timer->state.t1;
				timer->state.t1 += timer->state.dtx;
				failassert(cdrawTimerStateInternalValidate(&timer->state));
				callbackResult = callback(callbackArg_opt, &timer->state);
				++count;
			}
		}

		// reverse (same logic as above but negative)
		else if (dt_parent < 0)
		{
			ctime_t dt_track_r = (dt_parent * (-timer->t_scale));
			count = cdrawTimerInternalStepMulti(timer, dt_track_r, &dt_track_r, (-timer->state.dtx), callback, callbackArg_opt, &callbackResult);

			// final step
			timer->t_track -= dt_track_r;
			if (timer->t_track < timer->state.t0)
			{
				timer->state.dt = timer->t_track - timer->state.t;
				timer->state.t = timer->t_track;
				timer->state.t1 = timer->state.t0;
				timer->state.t0 -= timer->state.dtx;
				failassert(cdrawTimerStateInternalValidate(&timer->state));
				callbackResult = callback(callbackArg_opt, &timer->state);
				++count;
			}
		}
	}

	// always tick
	else
	{
		cdrawTimerInternalStepSpecial(timer, dt_parent);
		callbackResult = callback(callbackArg_opt, &timer->state);
		++count;
	}

	// store most recent result
	if (count > 0)
	{
		if(callbackResult_out_opt)
			*callbackResult_out_opt = callbackResult;
		result_inc(1);
	}
	if (count_out_opt)
		*count_out_opt = count;
	result_return();
}
