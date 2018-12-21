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

#define SELFCLASS esferixis::cps::AsyncForker

struct esferixis::cps::AsyncForker::Impl {
	Cont onJoin;

	std::atomic<int> remainingJoins;
};

SELFCLASS::AsyncForker() {
	this->impl_m = new SELFCLASS::Impl();

	this->impl_m->remainingJoins = 0;
}

SELFCLASS::~AsyncForker() {
	delete this->impl_m;
}

esferixis::cps::Cont SELFCLASS::fork(Cont onFork1, Cont onFork2) {
	if (this->impl_m->remainingJoins == 0) {
		this->impl_m->remainingJoins = 2;

		return esferixis::cps::Sched::fork(onFork1, onFork2);
	}
	else {
		throw std::runtime_error("Unexpected forked state");
	}
}

esferixis::cps::Cont SELFCLASS::fork(Cont onFork1, Cont onFork2, Cont onJoin) {
	if (this->impl_m->remainingJoins == 0) {
		this->impl_m->onJoin = onJoin;
		this->impl_m->remainingJoins = 2;

		return esferixis::cps::Sched::fork(onFork1, onFork2);
	}
	else {
		throw std::runtime_error("Unexpected forked state");
	}
}

esferixis::cps::Cont SELFCLASS::join() {
	const int remainingJoins_old = this->impl_m->remainingJoins--;

	if ( remainingJoins_old == 2 ) {
		return esferixis::cps::Sched::exit();
	}
	else if ( remainingJoins_old == 1 ) {
		return esferixis::cps::Sched::exit();
	}
	else {
		throw std::runtime_error("Unexpected joined state");
	}
}

void SELFCLASS::setOnJoin(Cont onJoin) {
	this->impl_m->onJoin = onJoin;
}