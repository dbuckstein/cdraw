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
* cdrawPlatform.c
* Main source for platform library.
*/

#include <stdio.h>
#include <stdlib.h>

#define CDRAW_ASSERT_TEST cdraw_assert_test_flag

#include "cdraw/cdrawPlatform.h"


result_t cdrawConsoleTest()
{
	result_init();
	cdrawConsoleCreate();
	cdrawConsoleDebugPatch();
	cdrawConsoleDestroy();
	result_return();
}


typedef struct cdrawTimerTest_t
{
	result_t count;
	byte_t* label;
} cdrawTimerTest_t;

result_t cdrawBaseTestTimerCallback(ptr_t const data, cdrawTimerState const* const timerState)
{
	cdrawTimerTest_t* const test = (cdrawTimerTest_t*)data;
	ctime_t const t = timerState->t;
	ctime_t const t0 = timerState->t0;
	ctime_t const t1 = timerState->t1;
	ctime_t const deltaTime = timerState->dt;
	ctime_t const deltaTimeExp = timerState->dtx;
	stime_t const deltaTimeSec = (deltaTime / (stime_t)timerState->cps);
	stime_t const deltaTimeSecExp = (deltaTimeExp / (stime_t)timerState->cps);
	result_t const result = printf("\n %s(%d): t=%+lld; t0=%+lld; t1=%+lld; dt=%+lld; dtx=%+lld; ds=%+.6lf; dsx=%+.6lf", test->label, ++test->count, t, t0, t1, deltaTime, deltaTimeExp, deltaTimeSec, deltaTimeSecExp);
	return result;
}

result_t cdrawBaseTestTimerCallbackLayered(ptr_t const data, cdrawTimerState const* const timerState)
{
	cdrawTimer* const child = (cdrawTimer*)data;
	ctime_t const t = timerState->t;
	ctime_t const t0 = timerState->t0;
	ctime_t const t1 = timerState->t1;
	ctime_t const deltaTime = timerState->dt;
	result_t const result = printf("\n %s: t=%+lld; t0=%+lld; t1=%+lld; dt=%+lld", (child ? "TIMER" : "    CHILD"), t, t0, t1, deltaTime);
	if (child)
		cdrawTimerStepCallback(child, timerState->dt, cdrawBaseTestTimerCallbackLayered, 0, 0);
	return result;
}

void cdrawTimerTest()
{
	cdrawTimer timerSys[1];
	cdrawTimerInitSystem(timerSys);

	cdrawTimer timer[1];

	// count each combo for a few seconds, try different behaviors with window
	int16_t const rate = 60;//3;//4;//7;//50;//300;//
	ctime_t const dur_s = 2;
	ctime_t dur;

	cdrawTimerTest_t test;

	// forward tests
	printf("\n\n //////// FORWARD //////// \n");

	// system: step
	// timer: big step
	printf("\n //// SYS: step //// TIMER: step ////");
	cdrawTimerSet(timer, timerSys, rate);
	dur = dur_s * timer->state.cps;
	test.count = 0;
	test.label = "step+step";
	while (timer->t_track < dur)
	{
		cdrawTimerStepSystem(timerSys);
		cdrawTimerStepCallback(timer, +timerSys->state.dt, cdrawBaseTestTimerCallback, &test, 0);
	}

	// system: step
	// timer: clip step
	printf("\n //// SYS: step //// TIMER: clip ////");
	cdrawTimerSet(timer, timerSys, rate);
	dur = dur_s * timer->state.cps;
	test.count = 0;
	test.label = "step+clip";
	while (timer->t_track < dur)
	{
		cdrawTimerStepSystem(timerSys);
		cdrawTimerStepClipCallback(timer, +timerSys->state.dt, cdrawBaseTestTimerCallback, &test, 0);
	}

	// system: step
	// timer: multi step
	printf("\n //// SYS: step //// TIMER: mult ////");
	cdrawTimerSet(timer, timerSys, rate);
	dur = dur_s * timer->state.cps;
	test.count = 0;
	test.label = "step+mult";
	while (timer->t_track < dur)
	{
		cdrawTimerStepSystem(timerSys);
		cdrawTimerStepMultiCallback(timer, +timerSys->state.dt, &test.count, cdrawBaseTestTimerCallback, &test, 0);
	}

	// system: clip
	// timer: big step
	printf("\n //// SYS: clip //// TIMER: step ////");
	cdrawTimerSet(timer, timerSys, rate);
	dur = dur_s * timer->state.cps;
	test.count = 0;
	test.label = "clip+step";
	while (timer->t_track < dur)
	{
		cdrawTimerStepSystemClip(timerSys);
		cdrawTimerStepCallback(timer, +timerSys->state.dt, cdrawBaseTestTimerCallback, &test, 0);
	}

	// system: clip
	// timer: clip step
	printf("\n //// SYS: clip //// TIMER: clip ////");
	cdrawTimerSet(timer, timerSys, rate);
	dur = dur_s * timer->state.cps;
	test.count = 0;
	test.label = "clip+clip";
	while (timer->t_track < dur)
	{
		cdrawTimerStepSystemClip(timerSys);
		cdrawTimerStepClipCallback(timer, +timerSys->state.dt, cdrawBaseTestTimerCallback, &test, 0);
	}

	// system: clip
	// timer: multi step
	printf("\n //// SYS: clip //// TIMER: mult ////");
	cdrawTimerSet(timer, timerSys, rate);
	dur = dur_s * timer->state.cps;
	test.count = 0;
	test.label = "clip+mult";
	while (timer->t_track < dur)
	{
		cdrawTimerStepSystemClip(timerSys);
		cdrawTimerStepMultiCallback(timer, +timerSys->state.dt, &test.count, cdrawBaseTestTimerCallback, &test, 0);
	}

	// reverse tests
	printf("\n\n //////// REVERSE //////// \n");

	// system: step
	// timer: big step
	printf("\n //// SYS: step //// TIMER: step ////");
	cdrawTimerSet(timer, timerSys, rate);
	dur = -dur_s * timer->state.cps;
	test.count = 0;
	test.label = "step-step";
	while (timer->t_track > dur)
	{
		cdrawTimerStepSystem(timerSys);
		cdrawTimerStepCallback(timer, -timerSys->state.dt, cdrawBaseTestTimerCallback, &test, 0);
	}

	// system: step
	// timer: clip step
	printf("\n //// SYS: step //// TIMER: clip ////");
	cdrawTimerSet(timer, timerSys, rate);
	dur = -dur_s * timer->state.cps;
	test.count = 0;
	test.label = "step-clip";
	while (timer->t_track > dur)
	{
		cdrawTimerStepSystem(timerSys);
		cdrawTimerStepClipCallback(timer, -timerSys->state.dt, cdrawBaseTestTimerCallback, &test, 0);
	}

	// system: step
	// timer: multi step
	printf("\n //// SYS: step //// TIMER: mult ////");
	cdrawTimerSet(timer, timerSys, rate);
	dur = -dur_s * timer->state.cps;
	test.count = 0;
	test.label = "step-mult";
	while (timer->t_track > dur)
	{
		cdrawTimerStepSystem(timerSys);
		cdrawTimerStepMultiCallback(timer, -timerSys->state.dt, &test.count, cdrawBaseTestTimerCallback, &test, 0);
	}

	// system: clip
	// timer: big step
	printf("\n //// SYS: clip //// TIMER: step ////");
	cdrawTimerSet(timer, timerSys, rate);
	dur = -dur_s * timer->state.cps;
	test.count = 0;
	test.label = "clip-step";
	while (timer->t_track > dur)
	{
		cdrawTimerStepSystemClip(timerSys);
		cdrawTimerStepCallback(timer, -timerSys->state.dt, cdrawBaseTestTimerCallback, &test, 0);
	}

	// system: clip
	// timer: clip step
	printf("\n //// SYS: clip //// TIMER: clip ////");
	cdrawTimerSet(timer, timerSys, rate);
	dur = -dur_s * timer->state.cps;
	test.count = 0;
	test.label = "clip-clip";
	while (timer->t_track > dur)
	{
		cdrawTimerStepSystemClip(timerSys);
		cdrawTimerStepClipCallback(timer, -timerSys->state.dt, cdrawBaseTestTimerCallback, &test, 0);
	}

	// system: clip
	// timer: multi step
	printf("\n //// SYS: clip //// TIMER: mult ////");
	cdrawTimerSet(timer, timerSys, rate);
	dur = -dur_s * timer->state.cps;
	test.count = 0;
	test.label = "clip-mult";
	while (timer->t_track > dur)
	{
		cdrawTimerStepSystemClip(timerSys);
		cdrawTimerStepMultiCallback(timer, -timerSys->state.dt, &test.count, cdrawBaseTestTimerCallback, &test, 0);
	}

	// layered tests
	printf("\n\n //////// LAYERED //////// \n");
	{
		cdrawTimer other[1];
		cdrawTimerSet(timer, timerSys, rate);
		cdrawTimerSet(other, timer, 24);
		dur = 5 * timer->state.cps;
		while (timer->t_track < dur)
		{
			cdrawTimerStepSystem(timerSys);
			cdrawTimerStepCallback(timer, timerSys->state.dt, cdrawBaseTestTimerCallbackLayered, other, 0);
		}
	}

	// done
	printf("\n\n //////// DONE //////// \n");

	cdrawTimerReset(timer);
	cdrawTimerReset(timerSys);
}


void cdrawPluginTest()
{
	cdrawPlugin plugin;
	cdrawPluginInfo* pluginInfo = NULL;
	size_t numPluginInfo = 0;
	byte_t pluginInfoBuffer[2048];
	cstr_t pluginInfoBufferItr;
	ptrdiff_t id = 0;
	
	cdrawPluginReset(&plugin);
	
	cdrawPluginInfoListLoad(&pluginInfo, &numPluginInfo, "../../../../resource/_plugin/cdraw_plugin_info.txt");
	
	cdrawPluginLoad(&plugin, pluginInfo + id, id, cdrawPluginTest);
	pluginInfoBufferItr = pluginInfoBuffer;
	cdrawPluginInfoPrint(&plugin.info, &pluginInfoBufferItr);
	printf("\n %s", pluginInfoBuffer);
	cdrawPluginReload(&plugin, cdrawPluginTest);
	cdrawPluginUnload(&plugin, cdrawPluginTest);

	id = -2;
	cdrawPluginLoad(&plugin, pluginInfo - id, id, cdrawPluginTest);
	pluginInfoBufferItr = pluginInfoBuffer;
	cdrawPluginInfoPrint(&plugin.info, &pluginInfoBufferItr);
	printf("\n %s", pluginInfoBuffer);
	cdrawPluginReload(&plugin, cdrawPluginTest);
	cdrawPluginUnload(&plugin, cdrawPluginTest);

	cdrawPluginInfoListRelease(&pluginInfo);
}


result_t cdrawPlatformTest()
{
	result_init();
	//cdrawConsoleTest();
	//cdrawTimerTest();
	cdrawPluginTest();
	result_return();
}
