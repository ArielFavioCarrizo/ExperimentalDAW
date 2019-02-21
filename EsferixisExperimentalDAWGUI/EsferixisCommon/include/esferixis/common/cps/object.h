/*
BSD 3-Clause License

Copyright (c) 2019, Ariel Favio Carrizo
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

#include <esferixis/common/cps/cont.h>
#include <esferixis/common/cps/exception.h>

EsferixisCommon_C_BEGIN

typedef struct _esferixis_cps_methodContext {
	void *objImplData;
	void *param;
} esferixis_cps_methodContext;

EsferixisCommon_C_END

#ifdef __cplusplus
namespace esferixis {
	namespace cps {
		template<typename T>
		class MethodProcedureContext final {
		public:
			/**
			 * @post Creates a method procedure context with
					 the specified parameter value and
					 the specified return continuation
			 */
			inline MethodProcedureContext(T *param, esferixis_cps_unsafecont cont) {
				this->c_methodContext_m.param = (void *)param;

				this->c_context_m.param = (void *)&this->c_methodContext_m;
				this->c_context_m.cont = cont;
			}

			/**
			 * @post Creates a empty method procedure context
			 */
			MethodProcedureContext() : MethodProcedureContext(nullptr, esferixis_cps_mkInvalidUnsafeCont()) {

			}

			/**
			 * @post Sets the parameter pointer
			 */
			inline void setParameter(T *param) {
				this->c_methodContext_m.param = (void *)param;
			}

			/**
			 * @post Gets the parameter pointer
			 */
			inline T * getParameter() {
				return (T *) this->c_methodContext_m.param;
			}

			/**
			 * @post Sets the continuation
			 */
			inline void setCont(esferixis_cps_unsafecont cont) {
				this->c_context_m.cont = cont;
			}

			/**
			 * @post Gets the continuation
			 */
			inline esferixis_cps_unsafecont getCont() {
				return this->c_context_m.cont;
			}

			/**
			 * @post Gets the pointer to C context
			 */
			inline esferixis_cps_procedureContext * cContext() {
				return &(this->c_context_m);
			}

		private:
			esferixis_cps_methodContext c_methodContext_m;
			esferixis_cps_procedureContext c_context_m;
		};
	}
}
#endif