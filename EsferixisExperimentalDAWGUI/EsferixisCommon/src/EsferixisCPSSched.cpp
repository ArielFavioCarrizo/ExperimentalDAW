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

#include "stdafx.h"
#include <esferixis/common/cps/sched.h>

#include <chrono>
#include <stdexcept>
#include <iostream>

#include <boost/optional.hpp>

static thread_local boost::optional<esferixis_cps_sched> currentSched;

static inline esferixis_cps_sched getCurrentSched() {
	if ( currentSched.is_initialized() ) {
		return *currentSched;
	}
	else {
		std::cerr << "The current OS thread doesn't have a green threads scheduler!";
		std::terminate();
	}
}

bool esferixis_cps_sched_isPresent() {
	return currentSched.is_initialized();
}

void esferixis_cps_sched_attach(esferixis_cps_sched sched) {
	if (!currentSched.is_initialized() ) {
		currentSched = boost::optional<esferixis_cps_sched>(sched);
	}
	else {
		std::cerr << "The current OS thread already has a green threads scheduler!";
		std::terminate();
	}
}

void esferixis_cps_sched_detach(esferixis_cps_sched sched) {
	if (currentSched.is_initialized() && (currentSched->data == sched.data)) {
		currentSched = boost::optional<esferixis_cps_sched>();
	}
	else {
		std::cerr << "Attemped to remove an scheduler that isn't assigned to the current OS thread!";
		std::terminate();
	}
}

esferixis_cps_cont esferixis_cps_sched_yield(esferixis_cps_cont cont) {
	esferixis_cps_sched sched = getCurrentSched();

	return sched.vtable.yield(sched.data, cont);
}

esferixis_cps_cont esferixis_cps_sched_fork(esferixis_cps_cont cont1, esferixis_cps_cont cont2) {
	esferixis_cps_sched sched = getCurrentSched();

	return sched.vtable.fork(sched.vtable.fork, cont1, cont2);
}

esferixis_cps_cont esferixis_cps_sched_waitFor(int64_t duration, esferixis_cps_cont cont) {
	esferixis_cps_sched sched = getCurrentSched();

	return sched.vtable.waitFor(sched.vtable.fork, duration, cont);
}

esferixis_cps_cont esferixis_cps_sched_exit() {
	esferixis_cps_sched sched = getCurrentSched();

	return sched.vtable.exit(sched.data);
}