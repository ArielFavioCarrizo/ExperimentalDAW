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

#include "MultigraphCViewMockTestWindow.h"
#include "EsferixisQtApplication.h"

#include <qlayout.h>
#include <qcolor.h>
#include <qevent.h>

#include "MultigraphCViewMock.h"

#define SELFCLASS esferixis::daw::gui::test::MultigraphCViewWindowMock

esferixis_cps_cont SELFCLASS::create(SELFCLASS::Essence essence) {
	struct STM {
		static esferixis_cps_cont onCreateWindow(SELFCLASS *self) {
			self->window_m = new SELFCLASS::LocalWindow(self);
			self->window_m->setWindowTitle("MultigraphCViewWindow test");
			self->window_m->setAttribute(::Qt::WA_DeleteOnClose, false);
			self->window_m->show();

			esferixis::daw::gui::HNoteSegmentMultigraph::Essence multigraphEssence;

			multigraphEssence.backgroundColor = ::Qt::GlobalColor::white;
			multigraphEssence.gridColor = ::Qt::GlobalColor::lightGray;
			multigraphEssence.instance = &(self->multigraph_m);

			multigraphEssence.onInitialized.exception = &(self->multigraphException_m);
			multigraphEssence.onInitialized.onFailure = esferixis::cps::mkCont(onMultigraphInstException, self);
			multigraphEssence.onInitialized.onSuccess = esferixis::cps::mkCont(onAddMultigraphWidget_goToGUIThread, self);

			multigraphEssence.onWaitingViewCreation = esferixis::cps::mkCont(onCreateView, self);
			multigraphEssence.viewContextEssence = &(self->viewContextEssence_m);

			return esferixis::daw::gui::HNoteSegmentMultigraph::create(multigraphEssence);
		}

		static esferixis_cps_cont onMultigraphInstException(SELFCLASS *self) {
			*(self->essence_m.onCreated.exception) = esferixis::cps::createException("Cannot create window because multigraph instantiation has failed -> " + esferixis::cps::destructiveExceptMsgCopy(self->multigraphException_m));

			return self->essence_m.onCreated.onFailure;
		}

		static esferixis_cps_cont onCreateView(SELFCLASS *self) {
			return esferixis::daw::gui::test::MultigraphCViewMock::create(self->viewContextEssence_m);
		}

		static esferixis_cps_cont onAddMultigraphWidget_goToGUIThread(SELFCLASS *self) {
			return esferixis::Qt::Application::toGuiThread(esferixis::cps::mkCont(onAddMultigraphWidget_onGUIThread, self));
		}

		static esferixis_cps_cont onAddMultigraphWidget_onGUIThread(SELFCLASS *self) {
			QHBoxLayout *layout = new QHBoxLayout();

			self->multigraph_m->setBackgroundColor(::Qt::GlobalColor::white);
			self->multigraph_m->setGridColor(::Qt::GlobalColor::lightGray);

			layout->addWidget(self->multigraph_m->widget());
			self->window_m->setLayout(layout);

			*(self->essence_m.windowMock) = self;

			return self->essence_m.onCreated.onSuccess;
		}
	};

	SELFCLASS *self = new SELFCLASS();
	self->essence_m = essence;

	return esferixis::Qt::Application::toGuiThread(esferixis::cps::mkCont(STM::onCreateWindow, self));
}

SELFCLASS::LocalWindow::LocalWindow(esferixis::daw::gui::test::MultigraphCViewWindowMock *parent) {
	this->parent_m = parent;
}

void SELFCLASS::LocalWindow::closeEvent(QCloseEvent *event) {
	struct STM {
		static esferixis_cps_cont onLockedGUI(esferixis::daw::gui::test::MultigraphCViewWindowMock *self) {
			esferixis_cps_unsafecont cont;

			cont.exception = &(self->multigraphException_m);
			cont.onFailure = esferixis::cps::mkCont(onMultigraphFailure, self);
			cont.onSuccess = esferixis::cps::mkCont(onMultigraphSuccess, self);

			return self->multigraph_m->destroy(cont);
		}

		static esferixis_cps_cont onMultigraphFailure(esferixis::daw::gui::test::MultigraphCViewWindowMock *self) {
			*(self->essence_m.onClosed.exception) = esferixis::cps::createException("Cannot destroy test window because its multigraph couldn't been destroyed -> " + esferixis::cps::destructiveExceptMsgCopy(self->multigraphException_m));
			
			return self->essence_m.onClosed.onFailure;
		}

		static esferixis_cps_cont onMultigraphSuccess(esferixis::daw::gui::test::MultigraphCViewWindowMock *self) {
			return esferixis::Qt::Application::toGuiThread( esferixis::cps::mkCont(onDeleteAll, self) );
		}

		static esferixis_cps_cont onDeleteAll(esferixis::daw::gui::test::MultigraphCViewWindowMock *self) {
			esferixis_cps_cont nextCont = self->essence_m.onClosed.onSuccess;
			
			self->window_m->deleteLater();

			delete self;

			return nextCont;
		}
	};

	event->accept();
	esferixis_runcps( esferixis::Qt::Application::lockGUI(esferixis::cps::mkCont(STM::onLockedGUI, this->parent_m)) );
}

SELFCLASS::MultigraphCViewWindowMock() {

}
