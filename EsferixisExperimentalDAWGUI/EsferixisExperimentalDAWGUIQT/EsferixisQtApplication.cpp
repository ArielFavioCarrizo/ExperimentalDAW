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
#include <limits>

esferixis::Qt::Application * SELFCLASS::instance_m = nullptr;

int SELFCLASS::run(int &argc, char **argv, esferixis_cps_cont cont) {
	SELFCLASS *instance = new SELFCLASS();

	SELFCLASS::instance_m = instance;

	instance->qApp_m = std::make_unique<QApplication>(argc, argv, QCoreApplication::ApplicationFlags);
	instance->qApp_m->setQuitOnLastWindowClosed(false);

	instance->guiLocks_m = 0;
	instance->quit_m = false;

	// Define and attach the CPS scheduler
	esferixis_cps_sched sched;
	{
		esferixis_cps_sched_vtable sched_vtable;

		sched_vtable.yield = SELFCLASS::sched_yield;
		sched_vtable.waitFor = SELFCLASS::sched_waitFor;
		sched_vtable.fork = SELFCLASS::sched_fork;
		sched_vtable.exit = SELFCLASS::sched_exit;

		sched.data = static_cast<void *>(instance);
		sched.vtable = sched_vtable;

		esferixis_cps_sched_attach(sched);
	}

	esferixis_runcps( SELFCLASS::lockGUI( cont ) );

	int retCode = instance->qApp_m->exec();

	esferixis_cps_sched_detach(sched);

	return retCode;
}

esferixis_cps_cont SELFCLASS::toGuiThread(esferixis_cps_cont cont) {
	SELFCLASS *instance = SELFCLASS::instance();

	QObject *obj = SELFCLASS::instance()->qApp_m->thread()->eventDispatcher();

	QMetaObject::invokeMethod(obj, [cont]() { esferixis_runcps(cont); }, ::Qt::QueuedConnection);

	return esferixis_cps_sched_exit();
}

esferixis_cps_cont SELFCLASS::lockGUI(esferixis_cps_cont cont) {
	SELFCLASS *self = SELFCLASS::instance()->instanceFromGUIThread();

	bool processGUIEvents_old = self->processGUIEvents();

	if (self->guiLocks_m != std::numeric_limits<unsigned int>::max() ) {
		self->guiLocks_m++;
	}
	else {
		throw std::runtime_error("GUI lock count overflow");
	}

	if ( processGUIEvents_old ) {
		esferixis_runcps(cont);

		while (!self->processGUIEvents() && (!self->quit_m)) {
			self->qApp_m->processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::WaitForMoreEvents);
		}

		return esferixis::cps::CPS_RET;
	}
	else {
		return cont;
	}
}

esferixis_cps_cont SELFCLASS::unlockGUI(esferixis_cps_cont cont) {
	unsigned int& guiLocks = SELFCLASS::instance()->instanceFromGUIThread()->guiLocks_m;

	if (guiLocks != 0) {
		guiLocks--;
	}
	else {
		throw std::runtime_error("GUI lock count underflow");
	}

	return cont;
}

esferixis_cps_cont SELFCLASS::exit(int returnCode) {
	SELFCLASS *self = SELFCLASS::instance()->instanceFromGUIThread();

	self->quit_m = true;
	self->qApp_m->exit(returnCode);

	return esferixis::cps::CPS_RET;
}

bool SELFCLASS::onGUIThread() {
	return (QThread::currentThread() == SELFCLASS::instance()->qApp_m->thread());
}

void SELFCLASS::checkOnGUIThread() {
	if ( !SELFCLASS::onGUIThread() ) {
		throw std::runtime_error("It must be run on the GUI thread");
	}
}

esferixis_cps_cont SELFCLASS::sched_yield(void *schedData, esferixis_cps_cont cont) {
	QObject *obj = QThread::currentThread()->eventDispatcher();
	QMetaObject::invokeMethod(obj, [cont]() { esferixis_runcps(cont); }, ::Qt::QueuedConnection);

	return esferixis::cps::CPS_RET;
}

esferixis_cps_cont SELFCLASS::sched_fork(void *schedData, esferixis_cps_cont cont1, esferixis_cps_cont cont2) {
	QObject *obj = QThread::currentThread()->eventDispatcher();
	QMetaObject::invokeMethod(obj, [cont2]() { esferixis_runcps(cont2); }, ::Qt::QueuedConnection);

	return cont1;
}

esferixis_cps_cont SELFCLASS::sched_waitFor(void *schedData, int64_t duration, esferixis_cps_cont cont) {
	QTimer::singleShot(std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::nanoseconds( duration ) ), [cont]() { esferixis_runcps(cont); });

	return esferixis::cps::CPS_RET;
}

esferixis_cps_cont SELFCLASS::sched_exit(void *schedData) {
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

esferixis::Qt::Application * SELFCLASS::instanceFromGUIThread() {
	SELFCLASS::checkOnGUIThread();
	
	return this;
}

bool SELFCLASS::processGUIEvents() {
	return (this->guiLocks_m == 0);
}