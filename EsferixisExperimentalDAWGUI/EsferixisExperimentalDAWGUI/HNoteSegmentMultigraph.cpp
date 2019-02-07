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

#include "HNoteSegmentMultigraph.h"

#include "EsferixisQtApplication.h"

#include <stdexcept>

#include <Qt>
#include <qpainter.h>
#include <QPaintEvent>

#define SELFCLASS esferixis::daw::gui::HNoteSegmentMultigraph

esferixis_cps_cont SELFCLASS::create(esferixis::daw::gui::HNoteSegmentMultigraph::Essence essence) {
	struct STM {
		static esferixis_cps_cont onCreateWidget(esferixis::daw::gui::HNoteSegmentMultigraph *self) {
			self->widget_m = new SELFCLASS::LocalQWidget(self);

			return self->essence_m.onWaitingViewCreation;
		}

		static esferixis_cps_cont onCreatedView_failure(esferixis::daw::gui::HNoteSegmentMultigraph *self) {
			*(self->essence_m.onInitialized.exception) = esferixis::cps::createException("Cannot create multigraph because instantion of the view has failed -> " + esferixis::cps::destructiveExceptMsgCopy(self->viewException_m));

			esferixis_cps_cont cont = self->essence_m.onInitialized.onFailure;

			delete self;

			return cont;
		}

		static esferixis_cps_cont onCreatedView_success(esferixis::daw::gui::HNoteSegmentMultigraph *self) {
			*(self->essence_m.instance) = self;

			return self->essence_m.onInitialized.onSuccess;
		}

		static esferixis_cps_cont onElementLoad(esferixis::daw::gui::HNoteSegmentMultigraph *self) {
			esferixis::daw::gui::HNoteSegmentMultigraph::ElementContext *elementContext = new esferixis::daw::gui::HNoteSegmentMultigraph::ElementContext();

			elementContext->multigraph = self;
			elementContext->noteSegment = self->viewState_m.referencedElement_m;

			elementContext->noteSegment->setContext(static_cast<void *>(elementContext));

			self->loadedElements_m.addLast(&elementContext->node_m);

			return self->viewState_m.onUpdated.onSuccess;
		}

		static esferixis_cps_cont onElementUnload(esferixis::daw::gui::HNoteSegmentMultigraph *self) {
			esferixis::daw::gui::MultigraphCHNoteSegment *noteSegment = self->viewState_m.referencedElement_m;

			esferixis::daw::gui::HNoteSegmentMultigraph::ElementContext *elementContext = static_cast<esferixis::daw::gui::HNoteSegmentMultigraph::ElementContext *>(noteSegment->getContext());

			self->loadedElements_m.remove(&elementContext->node_m);

			noteSegment->setContext(nullptr);

			delete elementContext;

			return self->viewState_m.onUpdated.onSuccess;
		}
	};

	esferixis::daw::gui::HNoteSegmentMultigraph *self = new esferixis::daw::gui::HNoteSegmentMultigraph();

	{
		auto e = essence.viewContextEssence;

		e->instance = &self->view_m;
		e->viewArea = essence.viewArea;

		e->onCreated.exception = &(self->viewException_m);
		e->onCreated.onFailure = esferixis::cps::mkCont(STM::onCreatedView_failure, self);
		e->onCreated.onSuccess = esferixis::cps::mkCont(STM::onCreatedView_success, self);

		e->stateFeedback.element = &(self->viewState_m.referencedElement_m);
		e->stateFeedback.elementStateFeedback = &(self->viewState_m.elementStateFeedback_m);
		e->stateFeedback.onElementLoad = esferixis::cps::mkCont(STM::onElementLoad, self);
		e->stateFeedback.onElementUnload = esferixis::cps::mkCont(STM::onElementLoad, self);
		e->stateFeedback.onUpdated = &(self->viewState_m.onUpdated);
	}

	self->widget_m = nullptr;
	self->view_m = nullptr;
	self->essence_m = essence;

	self->backgroundColor_m = essence.backgroundColor;
	self->gridCfg_m = essence.gridCfg;

	return esferixis::Qt::Application::toGuiThread(esferixis::cps::mkCont(STM::onCreateWidget, self));
}

QWidget * SELFCLASS::widget() const {
	return this->widget_m;
}

void SELFCLASS::setBackgroundColor(QColor color) {
	this->backgroundColor_m = color;
}

void SELFCLASS::setGridCfg(SELFCLASS::GridCfg gridCfg) {
	this->gridCfg_m = gridCfg;
}

esferixis_cps_cont SELFCLASS::destroy(const esferixis_cps_unsafecont cont) {
	struct STM {
		static esferixis_cps_cont deleteSelf(SELFCLASS *self) {
			esferixis_cps_unsafecont cont = self->nextExternalCont_m;

			self->widget_m->detachFromMultigraph();

			delete self;

			return cont.onSuccess;
		}

		static esferixis_cps_cont onFailure(SELFCLASS *self) {
			*(self->nextExternalCont_m.exception) = esferixis::cps::createException("Cannot destroy multigraph because its view couldn't have been destroyed -> " + esferixis::cps::destructiveExceptMsgCopy(self->viewException_m));

			return self->nextExternalCont_m.onFailure;
		}
	};

	this->nextExternalCont_m = cont;

	esferixis_cps_unsafecont nextCont;
	nextCont.exception = &(this->viewException_m);
	nextCont.onFailure = esferixis::cps::mkCont(STM::onFailure, this);
	nextCont.onSuccess = esferixis::cps::mkCont(STM::deleteSelf, this);

	return this->view_m->close(nextCont);
}

SELFCLASS::LocalQWidget::LocalQWidget(esferixis::daw::gui::HNoteSegmentMultigraph *multigraph) {
	this->multigraph_m = multigraph;
}

void SELFCLASS::LocalQWidget::paintEvent(QPaintEvent *event) {
	if (this->multigraph_m != nullptr) {
		QPainter painter(this);

		painter.setRenderHint(QPainter::Antialiasing, false);
		painter.fillRect(event->rect(), this->multigraph_m->backgroundColor_m);

		painter.save();

		// FIXME: Implement this
	}
}

void SELFCLASS::LocalQWidget::detachFromMultigraph() {
	this->multigraph_m = nullptr;
}