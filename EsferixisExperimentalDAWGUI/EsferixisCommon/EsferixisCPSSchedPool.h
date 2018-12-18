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

#include "EsferixisCPSSched.h"

namespace esferixis {
	namespace cps {
		class EsferixisCommon_API SchedPool final
		{
		public:
			/**
			 * @post Creates a pool of CPS Schedulers with the given factory function
					 and its context, and the given initial capacity
			 */
			template<typename T>
			inline SchedPool(esferixis::cps::Sched * (*factoryFunPtr) (T *data), T *factoryFunData, int initialCapacity) {
				this->implInit(reinterpret_cast<esferixis::cps::Sched * (*) (void *)>(factoryFunPtr), reinterpret_cast<void *>(factoryFunData), initialCapacity);
			}

			/**
			 * @post Destroys the pool
			 */
			~SchedPool();

			/**
			 * @post Sets the scheduler for the current thread
					 If it exists does nothing
			 */
			void setSchedForCurrentThread();

		private:
			struct Impl;

			/**
			 * @post Does the implementation initialization with the given factory function
					 and its context, and the given initial capacity
			 */
			void implInit(esferixis::cps::Sched * (*factoryFunPtr) (void *data), void *factoryFunData, int initialCapacity);

			Impl *impl;
		};
	}
}

