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

#include <esferixis/common/cps/asyncforker.h>
#include <esferixis/common/cps/sched.h>

#include <atomic>
#include <stdexcept>
#include <iostream>

struct _esferixis_cps_asyncforker {
	std::atomic<int> remainingJoins;
	esferixis_cps_asyncforker_cfg config;
};

esferixis_cps_asyncforker * esferixis_cps_asyncforker_new() {
	esferixis_cps_asyncforker *forker = new esferixis_cps_asyncforker();

	forker->config.onFork1 = esferixis_cps_mkInvalidCont();
	forker->config.onFork2 = esferixis_cps_mkInvalidCont();
	forker->config.onJoin = esferixis_cps_mkInvalidCont();

	forker->remainingJoins = 0;

	return forker;
}

void esferixis_cps_asyncforker_delete(esferixis_cps_asyncforker *asyncForker) {
	delete asyncForker;
}

esferixis_cps_asyncforker_cfg * esferixis_cps_asyncforker_config(esferixis_cps_asyncforker *asyncForker) {
	return &(asyncForker->config);
}

void esferixis_cps_asyncforker_fork(esferixis_cps_asyncforker *asyncForker, esferixis_cps_cont *nextCont) {
	if (asyncForker->remainingJoins == 0) {
		esferixis_cps_asyncforker_cfg config = asyncForker->config;

		asyncForker->remainingJoins = 2;

		esferixis_cps_sched_fork(&config.onFork1, &config.onFork2, nextCont);
	}
	else {
		std::cerr << "Unexpected forked state";
		std::terminate();
	}
}

void esferixis_cps_asyncforker_join(esferixis_cps_asyncforker *asyncForker, esferixis_cps_cont *nextCont) {
	const int remainingJoins_old = asyncForker->remainingJoins--;

	if (remainingJoins_old == 2) {
		esferixis_cps_sched_exit(nextCont);
	}
	else if (remainingJoins_old == 1) {
		esferixis_cps_sched_exit(nextCont);
	}
	else {
		std::cerr << "Unexpected joined state";
		std::terminate();
	}
}