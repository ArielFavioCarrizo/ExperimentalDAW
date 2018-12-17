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
#include <stdexcept>

#include "EsferixisCPSSched.h"

#include <iostream>

#define SELFCLASS esferixis::cps::Sched

static thread_local SELFCLASS * currentCPSSched = nullptr;

SELFCLASS::Sched() {
	
}

SELFCLASS::~Sched() {
	
}

void SELFCLASS::attach(esferixis::cps::Sched *sched) {
	if (currentCPSSched == nullptr) {
		currentCPSSched = sched;
	}
	else {
		throw std::runtime_error("The current OS thread already has a green threads scheduler!");
	}
}

void SELFCLASS::detach(esferixis::cps::Sched *sched) {
	if (currentCPSSched == sched) {
		currentCPSSched = nullptr;
	}
	else {
		throw std::runtime_error("Attemped to remove an scheduler that isn't assigned to the current OS thread!");
	}
}

esferixis::cps::Cont SELFCLASS::yield(esferixis::cps::Cont cont) {
	return SELFCLASS::currentSched()->yield_impl(cont);
}

esferixis::cps::Cont SELFCLASS::fork(esferixis::cps::Cont cont1, esferixis::cps::Cont cont2) {
	return SELFCLASS::currentSched()->fork_impl(cont1, cont2);
}

esferixis::cps::Cont SELFCLASS::waitFor(std::chrono::nanoseconds duration, esferixis::cps::Cont cont) {
	return SELFCLASS::currentSched()->waitFor_impl(duration, cont);
}

esferixis::cps::Cont SELFCLASS::exit() {
	return SELFCLASS::currentSched()->exit_impl();
}

esferixis::cps::Sched * SELFCLASS::currentSched() {
	if (currentCPSSched != nullptr) {
		return currentCPSSched;
	}
	else {
		throw std::runtime_error("The current OS thread doesn't have a green threads scheduler!");
	}
}