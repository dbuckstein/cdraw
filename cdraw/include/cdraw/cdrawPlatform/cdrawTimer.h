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
* cdrawTimer.h
* System and general high-precision timer interface.
*/

#ifndef _CDRAW_TIMER_H_
#define _CDRAW_TIMER_H_


#include "cdraw/cdraw/cdrawConfig.h"


/******************************************************************************
* Typedefs.
******************************************************************************/

enum
{
	warncode_timer_ticked,							// Warn code equivalent for timer tick.
	errcode_timer_sample = errcode_CUSTOM_BEGIN,	// Failed to sample time.
};


#define result_timer_ticked(x)	result_getwarn(x, warncode_timer_ticked)	// True if timer ticked.


/// <summary>
/// Type used to describe high-precision time counter (integer-based).
/// </summary>
typedef int64_t ctime_t;

/// <summary>
/// Type used to describe high-precision time in seconds (floating-point-based).
/// </summary>
typedef fp64_t	stime_t;

/// <summary>
/// Description of state of timer sample, relative to parent timer.
/// <para>
/// Diagram of derived values: 
/// 		  t(prev)		t(now)
/// 	|-----x---------|---x-----------|
/// 					t0				t1
/// 		  \____________/
/// 			dt(actual)
/// 	\______________/
/// 		dtx(expect)
/// Overstep (forward) = t - t0
/// Overstep (reverse) = t - t1
/// Delta ratio (actual to expected) = dt / dtx
/// </para>
/// </summary>
typedef struct cdrawTimerState
{
	/// <summary>
	/// Current elapsed time (greater than or equal to 't0' and less than 't1').
	/// </summary>
	ctime_t t;
	/// <summary>
	/// Lower bound of current tick interval (less than or equal to 't').
	/// </summary>
	ctime_t t0;
	/// <summary>
	/// Upper bound of current tick interval (greater than 't').
	/// </summary>
	ctime_t t1;
	/// <summary>
	/// Actual change in time from last tick to current time (from previous tick time to now).
	/// </summary>
	ctime_t dt;
	/// <summary>
	/// Expected change in time from last tick to current tick (length of completed interval).
	/// </summary>
	ctime_t dtx;
	/// <summary>
	/// Parent timer frequency of measurement (counts per second), scaled to local.
	/// </summary>
	ctime_t cps;
} cdrawTimerState;

/// <summary>
/// General high-precision timer descriptor.
/// </summary>
typedef struct cdrawTimer
{
	/// <summary>
	/// Current collected state of measurements.
	/// </summary>
	cdrawTimerState state;
	/// <summary>
	/// Time tracking, total time counted.
	/// </summary>
	ctime_t t_track;
	/// <summary>
	/// Time scaling.
	/// </summary>
	ctime_t t_scale;
} cdrawTimer;

/// <summary>
/// Timer tick callback format.
/// </summary>
typedef result_t(*cdrawTimerCallback)(ptr_t data, cdrawTimerState const* timerState);


/******************************************************************************
* Functions.
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Reset timer.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawTimerReset(cdrawTimer* const timer);

	/// <summary>
	/// Calculate elapsed time in seconds.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="elapsedTime_out">Pointer to storage for result.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawTimerStateGetElapsedTime(cdrawTimerState const* const timerState, stime_t* const elapsedTime_out);

	/// <summary>
	/// Calculate delta time in seconds.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="deltaTime_out">Pointer to storage for result.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawTimerStateGetDeltaTime(cdrawTimerState const* const timerState, stime_t* const deltaTime_out);

	/// <summary>
	/// Calculate expected interval duration in seconds.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="intervalTime_out">Pointer to storage for result.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawTimerStateGetIntervalTime(cdrawTimerState const* const timerState, stime_t* const intervalTime_out);

	/// <summary>
	/// Calculate overstep from previous interval; [0,1) represents some time in the current interval.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="overstepRatio_out">Pointer to storage for result.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawTimerStateGetOverstepRatio(cdrawTimerState const* const timerState, stime_t* const overstepRatio_out);

	/// <summary>
	/// Calculate understep from following interval; [0,1) represents some time in the current interval.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="understepRatio_out">Pointer to storage for result.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawTimerStateGetUnderstepRatio(cdrawTimerState const* const timerState, stime_t* const understepRatio_out);

	/// <summary>
	/// Calculate the ratio of actual delta time to expected delta time (interval).
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="deltaTimeRatio_out">Pointer to storage for result.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawTimerStateGetDeltaTimeRatio(cdrawTimerState const* const timerState, stime_t* const deltaTimeRatio_out);

	/// <summary>
	/// Get the system timer counts per second (frequency).
	/// </summary>
	/// <param name="cps_out">Pointer to storage of system counts-per-second or frequency value.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawTimerGetSystemFrequency(ctime_t* const cps_out);

	/// <summary>
	/// Get the system current time.
	/// </summary>
	/// <param name="t_out">Pointer to storage of system time.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawTimerGetSystemTime(ctime_t* const t_out);

	/// <summary>
	/// Initialize system timer.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawTimerInitSystem(cdrawTimer* const timer);

	/// <summary>
	/// Step and sample system timer.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <returns>Zero if success; Warn if tick; Error code otherwise.</returns>
	result_t cdrawTimerStepSystem(cdrawTimer* const timer);

	/// <summary>
	/// Step and sample system timer, clipping excessive delta time above rate.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <returns>Zero if success; Warn if tick; Error code otherwise.</returns>
	result_t cdrawTimerStepSystemClip(cdrawTimer* const timer);

	/// <summary>
	/// Step and sample system timer, invoking callback on tick.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="callback">Timer callback function, called on tick.</param>
	/// <param name="callbackArg_opt">Optional argument to callback function.</param>
	/// <param name="callbackResult_out_opt">Optional pointer to store result of callback function.</param>
	/// <returns>Zero if success; Warn if tick; Error code otherwise.</returns>
	result_t cdrawTimerStepSystemCallback(cdrawTimer* const timer, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out_opt);

	/// <summary>
	/// Step and sample system timer, clipping excessive delta time above rate and invoking callback on tick.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="callback">Timer callback function, called on tick.</param>
	/// <param name="callbackArg_opt">Optional argument to callback function.</param>
	/// <param name="callbackResult_out_opt">Optional pointer to store result of callback function.</param>
	/// <returns>Zero if success; Warn if tick; Error code otherwise.</returns>
	result_t cdrawTimerStepSystemClipCallback(cdrawTimer* const timer, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out_opt);

	/// <summary>
	/// Step and sample system timer, ticking every second and invoking callback on tick.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="count_out_opt">Optional pointer to tick count storage.</param>
	/// <param name="callback">Timer callback function, called on tick.</param>
	/// <param name="callbackArg_opt">Optional argument to callback function.</param>
	/// <param name="callbackResult_out_opt">Optional pointer to store result of callback function.</param>
	/// <returns>Zero if success; Warn if tick; Error code otherwise.</returns>
	result_t cdrawTimerStepSystemMultiCallback(cdrawTimer* const timer, result_t* const count_out_opt, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out_opt);

	/// <summary>
	/// Initialize timer relative to an arbitrary frame of reference.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="cps_parent">Counts-per-second of parent frame of reference.</param>
	/// <param name="ticksPerSecond">Tick rate (ticks per second) relative to parent time frame.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawTimerSet(cdrawTimer* const timer, ctime_t const cps_parent, uint16_t const ticksPerSecond);

	/// <summary>
	/// Initialize timer relative to another timer's frame of reference.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="parent">Pointer to parent timer for local reference.</param>
	/// <param name="ticksPerSecond">Tick rate (ticks per second) relative to parent time frame.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawTimerParentSet(cdrawTimer* const timer, cdrawTimer const* const parent, uint16_t const ticksPerSecond);

	/// <summary>
	/// Step and sample timer.
	/// Timer advances by desired change in time relative to parent reference timer.
	/// If the final time is beyond the expected interval duration, the timer state will catch up after ticking.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="dt_parent">Delta time relative to parent time.</param>
	/// <returns>Zero if success; Warn if tick; Error code otherwise.</returns>
	result_t cdrawTimerStep(cdrawTimer* const timer, ctime_t const dt_parent);

	/// <summary>
	/// Step and sample timer, clipping excessive delta time above rate.
	/// Timer advances by desired change in time relative to parent reference timer.
	/// If the final time is beyond the expected interval duration, excess time after tick is clipped.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="dt_parent">Delta time relative to parent time.</param>
	/// <returns>Zero if success; Warn if tick; Error code otherwise.</returns>
	result_t cdrawTimerStepClip(cdrawTimer* const timer, ctime_t const dt_parent);

	/// <summary>
	/// Step and sample timer, invoking callback on tick.
	/// Timer advances by desired change in time relative to parent reference timer.
	/// If the final time is beyond the expected interval duration, the timer state will catch up after ticking.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="dt_parent">Delta time relative to parent time.</param>
	/// <param name="callback">Timer callback function, called on tick.</param>
	/// <param name="callbackArg_opt">Optional argument to callback function.</param>
	/// <param name="callbackResult_out_opt">Optional pointer to store result of callback function.</param>
	/// <returns>Zero if success; Warn if tick; Error code otherwise.</returns>
	result_t cdrawTimerStepCallback(cdrawTimer* const timer, ctime_t const dt_parent, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out_opt);
	
	/// <summary>
	/// Step and sample timer, clipping excessive delta time above rate and invoking callback on tick.
	/// Timer advances by desired change in time relative to parent reference timer.
	/// If the final time is beyond the expected interval duration, excess time after tick is clipped.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="dt_parent">Delta time relative to parent time.</param>
	/// <param name="callback">Timer callback function, called on tick.</param>
	/// <param name="callbackArg_opt">Optional argument to callback function.</param>
	/// <param name="callbackResult_out_opt">Optional pointer to store result of callback function.</param>
	/// <returns>Zero if success; Warn if tick; Error code otherwise.</returns>
	result_t cdrawTimerStepClipCallback(cdrawTimer* const timer, ctime_t const dt_parent, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out_opt);
	
	/// <summary>
	/// Step and sample timer, repeating ticks until the final time is reached and invoking callback on tick.
	/// Timer advances by desired change in time relative to parent reference timer.
	/// If the final time is beyond the expected interval duration, the timer will continue updating and ticking.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="dt_parent">Delta time relative to parent time.</param>
	/// <param name="count_out_opt">Optional pointer to tick count storage.</param>
	/// <param name="callback">Timer callback function, called on tick.</param>
	/// <param name="callbackArg_opt">Optional argument to callback function.</param>
	/// <param name="callbackResult_out_opt">Optional pointer to store result of callback function.</param>
	/// <returns>Zero if success; Warn if tick; Error code otherwise.</returns>
	result_t cdrawTimerStepMultiCallback(cdrawTimer* const timer, ctime_t const dt_parent, result_t* const count_out_opt, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out_opt);

	/// <summary>
	/// Step and sample timer.
	/// Timer advances by desired change in time relative to parent reference timer.
	/// If the final time is beyond the expected interval duration, the timer state will catch up after ticking.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="parent">Pointer to parent timer.</param>
	/// <returns>Zero if success; Warn if tick; Error code otherwise.</returns>
	result_t cdrawTimerParentStep(cdrawTimer* const timer, cdrawTimer const* const parent);

	/// <summary>
	/// Step and sample timer, clipping excessive delta time above rate.
	/// Timer advances by desired change in time relative to parent reference timer.
	/// If the final time is beyond the expected interval duration, excess time after tick is clipped.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="parent">Pointer to parent timer.</param>
	/// <returns>Zero if success; Warn if tick; Error code otherwise.</returns>
	result_t cdrawTimerParentStepClip(cdrawTimer* const timer, cdrawTimer const* const parent);

	/// <summary>
	/// Step and sample timer, invoking callback on tick.
	/// Timer advances by desired change in time relative to parent reference timer.
	/// If the final time is beyond the expected interval duration, the timer state will catch up after ticking.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="parent">Pointer to parent timer.</param>
	/// <param name="callback">Timer callback function, called on tick.</param>
	/// <param name="callbackArg_opt">Optional argument to callback function.</param>
	/// <param name="callbackResult_out_opt">Optional pointer to store result of callback function.</param>
	/// <returns>Zero if success; Warn if tick; Error code otherwise.</returns>
	result_t cdrawTimerParentStepCallback(cdrawTimer* const timer, cdrawTimer const* const parent, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out_opt);

	/// <summary>
	/// Step and sample timer, clipping excessive delta time above rate and invoking callback on tick.
	/// Timer advances by desired change in time relative to parent reference timer.
	/// If the final time is beyond the expected interval duration, excess time after tick is clipped.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="parent">Pointer to parent timer.</param>
	/// <param name="callback">Timer callback function, called on tick.</param>
	/// <param name="callbackArg_opt">Optional argument to callback function.</param>
	/// <param name="callbackResult_out_opt">Optional pointer to store result of callback function.</param>
	/// <returns>Zero if success; Warn if tick; Error code otherwise.</returns>
	result_t cdrawTimerParentStepClipCallback(cdrawTimer* const timer, cdrawTimer const* const parent, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out_opt);

	/// <summary>
	/// Step and sample timer, repeating ticks until the final time is reached and invoking callback on tick.
	/// Timer advances by desired change in time relative to parent reference timer.
	/// If the final time is beyond the expected interval duration, the timer will continue updating and ticking.
	/// </summary>
	/// <param name="timer">Pointer to affected timer object.</param>
	/// <param name="parent">Pointer to parent timer.</param>
	/// <param name="count_out_opt">Optional pointer to tick count storage.</param>
	/// <param name="callback">Timer callback function, called on tick.</param>
	/// <param name="callbackArg_opt">Optional argument to callback function.</param>
	/// <param name="callbackResult_out_opt">Optional pointer to store result of callback function.</param>
	/// <returns>Zero if success; Warn if tick; Error code otherwise.</returns>
	result_t cdrawTimerParentStepMultiCallback(cdrawTimer* const timer, cdrawTimer const* const parent, result_t* const count_out_opt, cdrawTimerCallback const callback, ptr_t const callbackArg_opt, result_t* const callbackResult_out_opt);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #ifndef _CDRAW_TIMER_H_