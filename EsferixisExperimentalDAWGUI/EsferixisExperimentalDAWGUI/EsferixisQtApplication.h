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
			static int run(int &argc, char **argv, esferixis::cps::Cont cont);

			/**
			 * @post Executes continuation in the GUI thread
			 */
			static esferixis::cps::Cont toGuiThread(esferixis::cps::Cont cont);

			/**
			 * @post Lock GUI
			 */
			static void lockGUI();

			/**
			 * @post Unlock GUI
			 */
			static void unlockGUI();

			/**
			 * @post Quits application
			 */
			static esferixis::cps::Cont quit();

		private:
			class Esferixis_EXPORT LocalSched final : public esferixis::cps::Sched {
			public:
				/**
				 * @post Creates a QT scheduler wrapper
				 */
				LocalSched(esferixis::Qt::Application *app);

				/**
				 * @post Destroys the scheduler wrapper
				 */
				~LocalSched();

			protected:
				esferixis::cps::Cont yield_impl(esferixis::cps::Cont cont) override;
				esferixis::cps::Cont fork_impl(esferixis::cps::Cont cont1, esferixis::cps::Cont cont2) override;
				esferixis::cps::Cont waitFor_impl(std::chrono::nanoseconds duration, esferixis::cps::Cont cont) override;
				esferixis::cps::Cont exit_impl() override;
			private:
				esferixis::Qt::Application *app_m;
			};

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
			 * @post Checks that is the same thread as the GUI
			 */
			esferixis::Qt::Application * checkIsTheGUIThread();

			static esferixis::Qt::Application *instance_m;

			std::unique_ptr<QApplication> qApp_m;

			bool processGUIEvents_m;
			bool quit_m;
		};
	}
}
