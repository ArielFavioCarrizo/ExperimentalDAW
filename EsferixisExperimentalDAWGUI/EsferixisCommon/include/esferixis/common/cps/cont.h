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
#include <cstdlib>

EsferixisCommon_C_BEGIN

typedef struct _esferixis_cps_cont {
	void (*funptr) (void *, _esferixis_cps_cont *nextCont);
	void *data;
} esferixis_cps_cont;

inline void esferixis_runcps(esferixis_cps_cont firstCont) {
	esferixis_cps_cont currentCont = firstCont;

	while (currentCont.funptr != NULL) {
		currentCont.funptr(currentCont.data, &currentCont);
	}
}

inline esferixis_cps_cont esferixis_cps_mkCont(void (*funptr) (void *, esferixis_cps_cont *), void *data) {
	esferixis_cps_cont cont;

	cont.funptr = funptr;
	cont.data = data;

	return cont;
}

EsferixisCommon_C_API void esferixis_cps_invalidContFun(void *data, esferixis_cps_cont *nextCont);

inline esferixis_cps_cont esferixis_cps_mkInvalidCont() {
	return esferixis_cps_mkCont(esferixis_cps_invalidContFun, nullptr);
}

EsferixisCommon_C_END

#ifdef __cplusplus
namespace esferixis {
	namespace cps {
		/**
		 * @post Creates a continuation with the given function pointer and data
		 */
		template<typename T>
		inline esferixis_cps_cont mkCont(void(*funptr) (T *data, esferixis_cps_cont *nextCont), T *data) {
			esferixis_cps_cont cont;
			cont.funptr = reinterpret_cast<void (*) (void *, esferixis_cps_cont *)>(funptr);
			cont.data = reinterpret_cast<void *>(data);

			return cont;
		}

		// Continuation for qutting the CPS loop (Trampoline)
		const esferixis_cps_cont CPS_RET = mkCont<void *>(nullptr, nullptr);
	}
}
#endif