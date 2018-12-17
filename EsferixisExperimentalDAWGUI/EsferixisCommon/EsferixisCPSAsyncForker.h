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

#include "EsferixisCommon.h"

#include <boost/noncopyable.hpp>

#include "EsferixisCPSCont.h"

namespace esferixis {
	namespace cps {
		class EsferixisCommon_API AsyncForker final : private boost::noncopyable
		{
		public:
			/**
			 * @post Creates an async forker
			 */
			AsyncForker();

			/**
			 * @post Destroys the async forker
			 */
			~AsyncForker();

			/**
			 * @post Forks the green thread with the given
			         continuations after forking and the
					 continuation after join
			 */
			Cont fork(esferixis::cps::Cont onFork1, esferixis::cps::Cont onFork2, esferixis::cps::Cont onJoin);

			/**
			 * @post Joins the thread
			 */
			Cont join();

		private:
			struct Impl;

			Impl *impl_m;
		};

	}
}

