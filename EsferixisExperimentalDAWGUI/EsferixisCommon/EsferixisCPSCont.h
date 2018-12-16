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

#include <stdexcept>

#include "EsferixisCommon.h"

namespace esferixis {
	namespace cps {
		class Cont final
		{
		public:
			/**
			 * @post Creates an invalid continuation
			 */
			inline Cont() {
				this->funptr_m = esferixis::cps::Cont::invalidFun;
			}

			/**
			 * @post Creates a continuation with the given function pointer and data
			 */
			template<typename T>
			inline Cont(Cont(*funptr) (T *), T *data) {
				this->funptr_m = reinterpret_cast<Cont(*) (void *)>(funptr);
				this->data_m = reinterpret_cast<void *>(data);
			}

			template<typename T>
			static inline Cont create(Cont(*funptr) (T *), T *data) {
				return Cont(funptr, data);
			}

			friend void runCPS(Cont firstContinuation);

		private:
			static Cont invalidFun(void *data) {
				throw std::runtime_error("Invalid continuation");
			}

			Cont(*funptr_m) (void *);
			void *data_m;
		};

		// Continuation for qutting the CPS loop (Trampoline)
		const Cont CPS_RET = Cont::create<void *>(nullptr, nullptr);

		/**
		 * @post Run the given continuation and its successors until
		         CPS_RET
		 */
		inline void runCPS(Cont firstContinuation) {
			Cont currentCont = firstContinuation;

			while (currentCont.funptr_m != nullptr) {
				currentCont = currentCont.funptr_m(currentCont.data_m);
			}
		}
	}
}
