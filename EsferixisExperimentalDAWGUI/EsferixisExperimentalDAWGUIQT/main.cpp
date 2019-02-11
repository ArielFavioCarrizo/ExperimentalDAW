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

#include <esferixis/common/cps/cont.h>
#include "EsferixisQtApplication.h"

#include "MultigraphCViewMockTestWindow.h"

#include <stdexcept>

#include <QWindow>
#include <QObject>
#include <QCloseEvent>

void setup_simpleTest(void *dummy, esferixis_cps_cont *nextCont);
void setup_multigraphcCViewMockTestWindow(void *dummy, esferixis_cps_cont *nextCont);

int main(int argc, char *argv[])
{
	//return esferixis::Qt::Application::run(argc, argv, esferixis::cps::mkCont(setup_simpleTest, (void *) nullptr));
	return esferixis::Qt::Application::run(argc, argv, esferixis::cps::mkCont(setup_multigraphcCViewMockTestWindow, (void *) nullptr));
}

void setup_multigraphcCViewMockTestWindow(void *dummy, esferixis_cps_cont *nextCont) {
	struct LocalContext {
		esferixis::daw::gui::test::MultigraphCViewWindowMock *windowMock;
		esferixis_cps_exception windowException;
	};

	struct STM {
		static void onWindowInstFailure(LocalContext *context, esferixis_cps_cont *nextCont) {
			throw esferixis::cps::destructivelyConvertToStdException(context->windowException);
		}

		static void onWindowCreated(LocalContext *context, esferixis_cps_cont *nextCont) {
			esferixis::Qt::Application::unlockGUI(esferixis::cps::mkCont(onGUIUnlocked_1, context), nextCont);
		}

		static void onGUIUnlocked_1(LocalContext *context, esferixis_cps_cont *nextCont) {
			esferixis_cps_sched_exit(nextCont);
		}

		static void onWindowClosed_success(LocalContext *context, esferixis_cps_cont *nextCont) {
			esferixis::Qt::Application::unlockGUI(esferixis::cps::mkCont(onDestroyContext, context), nextCont);
		}

		static void onWindowClosed_failure(LocalContext *context, esferixis_cps_cont *nextCont) {
			throw esferixis::cps::destructivelyConvertToStdException(context->windowException);
		}

		static void onDestroyContext(LocalContext *context, esferixis_cps_cont *nextCont) {
			delete context;

			esferixis::Qt::Application::exit(0, nextCont);
		}
	};

	LocalContext *localContext = new LocalContext();

	esferixis::daw::gui::test::MultigraphCViewWindowMock::Essence windowContext;

	windowContext.windowMock = &(localContext->windowMock);
	windowContext.onCreated.exception = &(localContext->windowException);
	windowContext.onCreated.onFailure = esferixis::cps::mkCont(STM::onWindowInstFailure, localContext);
	windowContext.onCreated.onSuccess = esferixis::cps::mkCont(STM::onWindowCreated, localContext);

	windowContext.onClosed.exception = &(localContext->windowException);
	windowContext.onClosed.onFailure = esferixis::cps::mkCont(STM::onWindowClosed_failure, localContext);
	windowContext.onClosed.onSuccess = esferixis::cps::mkCont(STM::onWindowClosed_success, localContext);

	esferixis::daw::gui::test::MultigraphCViewWindowMock::create(windowContext, nextCont);
}

void setup_simpleTest(void *dummy, esferixis_cps_cont *nextCont) {
	struct Context {
		QWindow *window;
	};

	struct STM {
		static void onGUIUnlocked(Context *context, esferixis_cps_cont *nextCont) {
			esferixis_cps_sched_exit(nextCont);
		}
	};

	class LocalQWindow : public QWindow {
	public:
		LocalQWindow(Context *context) {
			this->context_m = context;
		}
	protected:
		bool event(QEvent *event) override {
			if (event->type() == QEvent::Type::Close) {
				this->deleteLater();
				event->accept();

				return true;
			}
			else {
				return QWindow::event(event);
			}
		}

	private:
		Context *context_m;
	};

	Context *context = new Context();

	QWindow *window = new LocalQWindow(context);

	context->window = window;

	window->resize(800, 600);
	window->show();
	window->setTitle("Test");

	QObject::connect(window, &QWindow::destroyed, []() {
		esferixis_cps_cont nextCont;
		esferixis::Qt::Application::exit(0, &nextCont);
		esferixis_runcps(nextCont);
	});

	esferixis::Qt::Application::unlockGUI( esferixis::cps::mkCont(STM::onGUIUnlocked, context), nextCont );
}

/*
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	bool willQuit = false;

	//a.setQuitOnLastWindowClosed(false);

	QWindow window;

	window.resize(800, 600);
	window.show();
	window.setTitle("Test");

	while ( !willQuit ) {
		a.processEvents(QEventLoop::WaitForMoreEvents);
	}

	return a.exec();
}
*/