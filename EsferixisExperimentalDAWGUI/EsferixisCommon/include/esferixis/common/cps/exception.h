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

#include <esferixis/common/cps/cont.h>

#ifdef __cplusplus
#include <string>
#include <stdexcept>
#endif

typedef struct _esferixis_cps_exception {
	const char *message;
	void(*destroy) (void *implData);

	void *implData;
} esferixis_cps_exception;

typedef struct _esferixis_cps_unsafecont {
	esferixis_cps_exception *exception;

	esferixis_cps_cont onSuccess;
	esferixis_cps_cont onFailure;
} esferixis_cps_unsafecont;

/**
 * @post Destroys the specified exception
 */
inline void esferixis_cps_exception_destroy(esferixis_cps_exception exception) {
	exception.destroy(exception.implData);
}

/**
 * @post Creates an exception with the specified description size
 */
EsferixisCommon_C_API esferixis_cps_exception esferixis_cps_exception_create(size_t descriptionSize);

/**
 * @post Creates an invalid 'unsafecont'
 */
inline esferixis_cps_unsafecont esferixis_cps_mkInvalidUnsafeCont() {
	esferixis_cps_unsafecont unsafeCont;

	unsafeCont.onFailure = esferixis_cps_mkInvalidCont();
	unsafeCont.onSuccess = esferixis_cps_mkInvalidCont();
}

#ifdef __cplusplus
namespace esferixis {
	namespace cps {
		/**
		 * @post Creates an exception with the specified message
		 */
		inline esferixis_cps_exception createException(std::string message) {
			size_t messageLength = message.length() + sizeof(char);

			esferixis_cps_exception exception = esferixis_cps_exception_create(messageLength);

			memcpy((void *)exception.message, (void *)message.c_str(), messageLength);

			return exception;
		}

		/**
		 * @post Copies the exception description destroying the original
				 exception in the process
		 */
		inline std::string destructiveExceptMsgCopy(esferixis_cps_exception exception) {
			std::string message(exception.message);
			esferixis_cps_exception_destroy(exception);

			return message;
		}

		/**
		 * @post Destructively converts the given exception to std exception
		 */
		inline std::runtime_error destructivelyConvertToStdException(esferixis_cps_exception exception) {
			std::string message = esferixis::cps::destructiveExceptMsgCopy(exception);

			return std::runtime_error(message);
		}
	}
}
#endif