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
	instance->sched.data = static_cast<void *>(instance);
	instance->sched.vtable = &SELFCLASS::sched_vtable;

	esferixis_cps_sched_attach(&instance->sched);

	esferixis_cps_cont nextCont;
	SELFCLASS::lockGUI(cont, &nextCont);
	esferixis_runcps(nextCont);

	int retCode = instance->qApp_m->exec();

	esferixis_cps_sched_detach(&instance->sched);

	return retCode;
}

void SELFCLASS::toGuiThread(esferixis_cps_cont cont, esferixis_cps_cont *nextCont) {
	SELFCLASS *instance = SELFCLASS::instance();

	QObject *obj = SELFCLASS::instance()->qApp_m->thread()->eventDispatcher();

	QMetaObject::invokeMethod(obj, [cont]() { esferixis_runcps(cont); }, ::Qt::QueuedConnection);

	esferixis_cps_sched_exit(nextCont);
}
void SELFCLASS::lockGUI(esferixis_cps_cont cont, esferixis_cps_cont *nextCont) {
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

		*nextCont = esferixis::cps::CPS_RET;
	}
	else {
		*nextCont = cont;
	}
}

void SELFCLASS::unlockGUI(esferixis_cps_cont cont, esferixis_cps_cont *nextCont) {
	unsigned int& guiLocks = SELFCLASS::instance()->instanceFromGUIThread()->guiLocks_m;

	if (guiLocks != 0) {
		guiLocks--;
	}
	else {
		throw std::runtime_error("GUI lock count underflow");
	}

	*nextCont = cont;
}

void SELFCLASS::exit(int returnCode, esferixis_cps_cont *nextCont) {
	SELFCLASS *self = SELFCLASS::instance()->instanceFromGUIThread();

	self->quit_m = true;
	self->qApp_m->exit(returnCode);

	*nextCont = esferixis::cps::CPS_RET;
}

bool SELFCLASS::onGUIThread() {
	return (QThread::currentThread() == SELFCLASS::instance()->qApp_m->thread());
}

void SELFCLASS::checkOnGUIThread() {
	if ( !SELFCLASS::onGUIThread() ) {
		throw std::runtime_error("It must be run on the GUI thread");
	}
}

void SELFCLASS::sched_yield(void *schedData, const esferixis_cps_cont *cont, esferixis_cps_cont *nextCont) {
	QObject *obj = QThread::currentThread()->eventDispatcher();

	esferixis_cps_cont cont_local = *cont;
	QMetaObject::invokeMethod(obj, [cont_local]() { esferixis_runcps(cont_local); }, ::Qt::QueuedConnection);

	*nextCont = esferixis::cps::CPS_RET;
}

void SELFCLASS::sched_fork(void *schedData, const esferixis_cps_cont *cont1, const esferixis_cps_cont *cont2, esferixis_cps_cont *nextCont) {
	QObject *obj = QThread::currentThread()->eventDispatcher();

	esferixis_cps_cont cont2_local = *cont2;
	QMetaObject::invokeMethod(obj, [cont2_local]() { esferixis_runcps(cont2_local); }, ::Qt::QueuedConnection);

	*nextCont = *cont1;
}

void SELFCLASS::sched_waitFor(void *schedData, int64_t duration, const esferixis_cps_cont *cont, esferixis_cps_cont *nextCont) {
	esferixis_cps_cont cont_local = *cont;

	QTimer::singleShot(std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::nanoseconds( duration ) ), [cont_local]() { esferixis_runcps(cont_local); });

	*nextCont = esferixis::cps::CPS_RET;
}

void SELFCLASS::sched_exit(void *schedData, esferixis_cps_cont *nextCont) {
	*nextCont = esferixis::cps::CPS_RET;
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