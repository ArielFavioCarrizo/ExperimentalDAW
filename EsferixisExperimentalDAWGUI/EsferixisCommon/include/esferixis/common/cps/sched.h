/*
BSD 3-Clause License

Copyright (c) 2018, Ariel Favio Carrizo
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <esferixis/common/common.h>
#include <esferixis/common/cps/cont.h>

#ifdef __cplusplus
#include <cstdint>
#else
#include "stdint.h"
#endif

EsferixisCommon_C_BEGIN

typedef struct esferixis_cps_sched_vtable {
	esferixis_cps_cont(*yield) (void *schedData, esferixis_cps_cont cont);
	esferixis_cps_cont(*fork) (void *schedData, esferixis_cps_cont cont1, esferixis_cps_cont cont2);
	esferixis_cps_cont(*waitFor)(void *schedData, int64_t duration, esferixis_cps_cont cont);
	esferixis_cps_cont(*exit)(void *schedData);
} esferixis_cps_sched_vtable;

typedef struct esferixis_cps_sched {
	esferixis_cps_sched_vtable vtable;
	void *data;
} esferixis_cps_sched;

/**
 * @post Shows if the current os thread has a scheduler
 */
EsferixisCommon_C_API bool esferixis_cps_sched_isPresent();

/**
 * @pre Expects that the current OS thread doesn't have a scheduler and the scheduler isn't attached to any OS thread.
 * @post Adds an scheduler in the current OS thread.
 */
EsferixisCommon_C_API void esferixis_cps_sched_attach(esferixis_cps_sched sched);

/**
 * @pre Expects that the current OS thread does have the given scheduler
   @post Removes the scheduler from the current OS thread
 */
EsferixisCommon_C_API void esferixis_cps_sched_detach(esferixis_cps_sched sched);

/**
 * @post Yields to another tasks with the given continuation
 */
EsferixisCommon_C_API esferixis_cps_cont esferixis_cps_sched_yield(esferixis_cps_cont cont);

/**
  * @post Forks the current green thread into two threads with the
		  given continuations
 */
EsferixisCommon_C_API esferixis_cps_cont esferixis_cps_sched_fork(esferixis_cps_cont cont1, esferixis_cps_cont cont2);

/**
 * @post Waits the given duration and executes the given continuation in nanoseconds
 */
EsferixisCommon_C_API esferixis_cps_cont esferixis_cps_sched_waitFor(int64_t duration, esferixis_cps_cont cont);

/**
 * @post Terminates the current green thread
 */
EsferixisCommon_C_API esferixis_cps_cont esferixis_cps_sched_exit();

EsferixisCommon_C_END