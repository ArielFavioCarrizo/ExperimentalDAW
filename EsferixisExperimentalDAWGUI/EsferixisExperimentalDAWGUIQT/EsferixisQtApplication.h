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

#include <qthread.h>
#include <qapplication.h>
#include <qabstracteventdispatcher.h>
#include <qtimer.h>

#include <boost/noncopyable.hpp>

#include <esferixis/common/cps/cont.h>
#include <esferixis/common/cps/sched.h>

#include <memory>

namespace esferixis {
	namespace Qt {
		class Application final : private boost::noncopyable
		{
		public:
			/**
			 * @post Exec QApplication with the specified continuation
					 to execute after it is started

					 Returns error code
			 */
			static int run(int &argc, char **argv, esferixis_cps_cont cont);

			/**
			 * @post Executes continuation in the GUI thread
			 */
			static void toGuiThread(esferixis_cps_cont cont, esferixis_cps_cont *nextCont);

			/**
			 * @post Lock GUI
			 */
			static void lockGUI(esferixis_cps_cont cont, esferixis_cps_cont *nextCont);

			/**
			 * @post Unlock GUI
			 */
			static void unlockGUI(esferixis_cps_cont cont, esferixis_cps_cont *nextCont);

			/**
			 * @post Exits application with the specified return code
			 */
			static void exit(int returnCode, esferixis_cps_cont *nextCont);

			/**
			 * @post Returns if the current thread is the GUI thread
			 */
			static bool onGUIThread();

			/**
			 * @post Checks if the current thread is the GUI thread.
					 Throws an exception if it is called from another thread.
			 */
			static void checkOnGUIThread();

		private:
			static void sched_yield(void *schedData, const esferixis_cps_cont *cont, esferixis_cps_cont *nextCont);
			static void sched_fork(void *schedData, const esferixis_cps_cont *cont1, const esferixis_cps_cont *cont2, esferixis_cps_cont *nextCont);
			static void sched_waitFor(void *schedData, int64_t duration, const esferixis_cps_cont *cont, esferixis_cps_cont *nextCont);
			static void sched_exit(void *schedData, esferixis_cps_cont *nextCont);

			static constexpr esferixis_cps_sched_vtable sched_vtable = {
				sched_yield,
				sched_fork,
				sched_waitFor,
				sched_exit
			};

			esferixis_cps_sched sched;

			/**
			 * @post Creates an QApplication wrapped in a CPS style manager
			 */
			Application();

			/**
			 * @post Destroys the object
			 */
			~Application();

			/**
			 * @post Gets instance
			 */
			static esferixis::Qt::Application * instance();

			/**
			 * @post Gets the instance checking that it is called from the GUI thread
			 */
			esferixis::Qt::Application * instanceFromGUIThread();

			/**
			 * @post Returns a boolean value indicating if the GUI events are enabled
			 */
			bool processGUIEvents();

			static esferixis::Qt::Application *instance_m;

			std::unique_ptr<QApplication> qApp_m;

			unsigned int guiLocks_m;
			bool quit_m;
		};
	}
}
