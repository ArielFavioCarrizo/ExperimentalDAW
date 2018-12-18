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
#include "EsferixisCPSSchedPool.h"

#include <boost/thread/mutex.hpp>
#include <stack>

#define SELFCLASS esferixis::cps::SchedPool
#define SELF_NewSched impl->factoryFunPtr( impl->factoryFunData )

struct SELFCLASS::Impl {
	esferixis::cps::Sched * (*factoryFunPtr) (void *);
	void *factoryFunData;

	std::stack<esferixis::cps::Sched *> unattachedScheds;
	std::stack<esferixis::cps::Sched *> createdScheds;

	boost::mutex stateMutex;
};

SELFCLASS::~SchedPool() {
	while ( !impl->createdScheds.empty()) {
		delete impl->createdScheds.top();
		impl->createdScheds.pop();
	}

	delete impl;
}

void SELFCLASS::setSchedForCurrentThread() {
	if (!esferixis::cps::Sched::currentThreadHasAScheduler()) {
		boost::unique_lock<boost::mutex> stateLock;

		esferixis::cps::Sched *schedToAttach;

		if (!impl->unattachedScheds.empty()) {
			schedToAttach = impl->unattachedScheds.top();
			impl->unattachedScheds.pop();

			stateLock.unlock();
		}
		else {
			stateLock.unlock();

			schedToAttach = SELF_NewSched;

			stateLock.lock();

			impl->createdScheds.push(schedToAttach);

			stateLock.unlock();
		}

		schedToAttach->attachToCurrentThread();
	}
}

void SELFCLASS::implInit(esferixis::cps::Sched * (*factoryFunPtr) (void *data), void *factoryFunData, int initialCapacity) {
	impl = new SELFCLASS::Impl();

	impl->factoryFunPtr = factoryFunPtr;
	impl->factoryFunData = factoryFunData;
	
	for (int i = 0; i <initialCapacity; i++) {
		auto eachSched = SELF_NewSched;

		impl->unattachedScheds.push(eachSched);
		impl->createdScheds.push(eachSched);
	}
}