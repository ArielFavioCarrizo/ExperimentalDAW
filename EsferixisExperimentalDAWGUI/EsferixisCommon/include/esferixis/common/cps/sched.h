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

#include <chrono>
#include <boost/noncopyable.hpp>

#include <esferixis/common/common.h>
#include <esferixis/common/cps/cont.h>

namespace esferixis {
	namespace cps {
		class EsferixisCommon_API Sched : private boost::noncopyable
		{
		public:
			/**
			 * @post Creates an scheduler of stackless green threads for an OS thread
			 */
			Sched();

			/**
			 * @pre The scheduler mustn't be attached to any OS thread
			 * @post Destroys the scheduler
			 */
			virtual ~Sched();

			/**
			 * @post Shows if the current os thread has a scheduler
			 */
			static bool currentThreadHasAScheduler();

			/**
			 * @pre Expects that the current OS thread doesn't have a scheduler and the scheduler isn't attached to any OS thread.
			 * @post Adds an scheduler in the current OS thread.
			         If the scheduler exists it throws an exception.
			 */
			void attachToCurrentThread();

			/**
			 * @pre Expects that the current OS thread does have the given scheduler
			   @post Removes the scheduler from the current OS thread
			 */
			void detachFromCurrentThread();

			/**
			 * @post Yields to another tasks with the given continuation
			 */
			static Cont yield(Cont cont);

			/**
			  * @post Forks the current green thread into two threads with the
					  given continuations
			 */
			static Cont fork(Cont cont1, Cont cont2);

			/**
			 * @post Waits the given duration and executes the given continuation in nanoseconds
			 */
			static Cont waitFor(std::chrono::nanoseconds duration, Cont cont);

			/**
			 * @post Terminates the current green thread
			 */
			static Cont exit();

		protected:
			/**
			 * @post Yields to another tasks with the given continuation (Implementation)
			 */
			virtual Cont yield_impl(Cont cont) = 0;

			 /**
			  * @post Forks the current green thread into two threads with the
			          given continuations (Implementation)
			  */
			virtual Cont fork_impl(Cont cont1, Cont cont2) =0;

			/**
			 * @post Waits the given duration an executes the given continuation (Implementation)
			 */
			virtual Cont waitFor_impl(std::chrono::nanoseconds duration, Cont cont) =0;

			/**
			 * @post Terminates the current green thread (Implementation)
			 */
			virtual Cont exit_impl() =0;

		private:
			/**
			 * @pre Expects that the current OS thread does have a scheduler
			 * @post Gets the current OS thread scheduler
			 */
			static esferixis::cps::Sched * currentSched();
		};
	}
}

