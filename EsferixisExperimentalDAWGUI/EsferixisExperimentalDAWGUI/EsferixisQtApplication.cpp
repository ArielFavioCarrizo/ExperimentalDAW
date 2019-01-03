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

#include "EsferixisQtApplication.h"

#define SELFCLASS esferixis::Qt::Application

#include <utility>

esferixis::Qt::Application * SELFCLASS::instance_m = nullptr;

int SELFCLASS::run(int &argc, char **argv, esferixis::cps::Cont cont) {
	SELFCLASS *instance = new SELFCLASS();

	SELFCLASS::instance_m = instance;

	instance->qApp_m = std::make_unique<QApplication>(argc, argv, QCoreApplication::ApplicationFlags);
	instance->qApp_m->setQuitOnLastWindowClosed(false);

	instance->processGUIEvents_m = true;
	instance->quit_m = false;

	SELFCLASS::LocalSched localSched(instance);

	localSched.attachToCurrentThread();

	runCPS( SELFCLASS::lockGUI( cont ) );

	int retCode = instance->qApp_m->exec();

	localSched.detachFromCurrentThread();

	return retCode;
}

esferixis::cps::Cont SELFCLASS::toGuiThread(esferixis::cps::Cont cont) {
	SELFCLASS *instance = SELFCLASS::instance();

	QObject *obj = SELFCLASS::instance()->qApp_m->thread()->eventDispatcher();

	QMetaObject::invokeMethod(obj, [cont]() { runCPS(cont); }, ::Qt::QueuedConnection);

	return esferixis::cps::Sched::exit();
}

esferixis::cps::Cont SELFCLASS::lockGUI(esferixis::cps::Cont cont) {
	SELFCLASS *self = SELFCLASS::instance()->checkIsTheGUIThread();

	if (self->processGUIEvents_m) {
		self->processGUIEvents_m = false;

		runCPS(cont);

		while (!self->processGUIEvents_m && (!self->quit_m)) {
			self->qApp_m->processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::WaitForMoreEvents);
		}

		return esferixis::cps::CPS_RET;
	}
	else {
		return cont;
	}
}

esferixis::cps::Cont SELFCLASS::unlockGUI(esferixis::cps::Cont cont) {
	SELFCLASS::instance()->checkIsTheGUIThread()->processGUIEvents_m = true;

	return cont;
}

esferixis::cps::Cont SELFCLASS::quit() {
	SELFCLASS *self = SELFCLASS::instance()->checkIsTheGUIThread();

	self->quit_m = true;
	self->qApp_m->quit();

	return esferixis::cps::CPS_RET;
}

SELFCLASS::LocalSched::LocalSched(esferixis::Qt::Application *app) {
	this->app_m = app;
}

SELFCLASS::LocalSched::~LocalSched() {

}

esferixis::cps::Cont SELFCLASS::LocalSched::yield_impl(esferixis::cps::Cont cont) {
	QObject *obj = QThread::currentThread()->eventDispatcher();
	QMetaObject::invokeMethod(obj, [cont]() { runCPS(cont); }, ::Qt::QueuedConnection);

	return esferixis::cps::CPS_RET;
}

esferixis::cps::Cont SELFCLASS::LocalSched::fork_impl(esferixis::cps::Cont cont1, esferixis::cps::Cont cont2) {
	QObject *obj = QThread::currentThread()->eventDispatcher();
	QMetaObject::invokeMethod(obj, [cont2]() { runCPS(cont2); }, ::Qt::QueuedConnection );

	return cont1;
}

esferixis::cps::Cont SELFCLASS::LocalSched::waitFor_impl(std::chrono::nanoseconds duration, esferixis::cps::Cont cont) {
	QTimer::singleShot(std::chrono::duration_cast<std::chrono::milliseconds>(duration), [cont]() { runCPS(cont); });

	return esferixis::cps::CPS_RET;
}

esferixis::cps::Cont SELFCLASS::LocalSched::exit_impl() {
	return esferixis::cps::CPS_RET;
}

SELFCLASS::Application() {

}

SELFCLASS::~Application() {

}

esferixis::Qt::Application * SELFCLASS::instance() {
	if (SELFCLASS::instance_m == nullptr) {
		throw std::runtime_error("Missing instance");
	}
	else {
		return SELFCLASS::instance_m;
	}
}

esferixis::Qt::Application * SELFCLASS::checkIsTheGUIThread() {
	if (QThread::currentThread() == this->qApp_m->thread()) {
		return this;
	}
	else {
		throw std::runtime_error("It must be run on the GUI thread");
	}
}