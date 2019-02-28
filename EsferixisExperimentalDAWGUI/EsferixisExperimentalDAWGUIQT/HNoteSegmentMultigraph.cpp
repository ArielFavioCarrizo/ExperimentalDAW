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

#include "HNoteSegmentMultigraph.h"

#include "EsferixisQtApplication.h"

#include <stdexcept>

#include <Qt>
#include <qpainter.h>
#include <QPaintEvent>
#include <qgridlayout.h>
#include "esferixis/daw/gui/common/grid.h"
#include "EsferixisQtPainterAux.h"

#define SELFCLASS esferixis::daw::gui::HNoteSegmentMultigraph

const int SELFCLASS::scrollBarMaxValue = 10000000;

void SELFCLASS::create(esferixis::daw::gui::HNoteSegmentMultigraph::Essence essence, esferixis_cps_cont *nextCont) {
	struct STM {
		static void onCreateWidget(esferixis::daw::gui::HNoteSegmentMultigraph *self, esferixis_cps_cont *nextCont) {
			QGridLayout *layout = new QGridLayout();
			layout->setHorizontalSpacing(0);
			layout->setVerticalSpacing(0);

			self->rootWidget_m = new QWidget();
			self->rootWidget_m->setLayout(layout);

			self->pictureWidget_m = new SELFCLASS::PictureWidget(self);
			layout->addWidget(self->pictureWidget_m, 0, 0);

			self->hScrollBar_m = new QScrollBar(::Qt::Horizontal);
			self->hScrollBar_m->setMinimum(0);
			self->hScrollBar_m->setMaximum(scrollBarMaxValue);
			layout->addWidget(self->hScrollBar_m, 1, 0);

			self->vScrollBar_m = new QScrollBar(::Qt::Vertical);
			self->vScrollBar_m->setMinimum(0);
			self->vScrollBar_m->setMaximum(scrollBarMaxValue);
			layout->addWidget(self->vScrollBar_m, 0, 1);

			*nextCont = self->essence_m.onWaitingViewCreation;
		}

		static void onCreatedView_failure(esferixis::daw::gui::HNoteSegmentMultigraph *self, esferixis_cps_cont *nextCont) {
			*(self->essence_m.onInitialized.exception) = esferixis::cps::createException("Cannot create multigraph because instantion of the view has failed -> " + esferixis::cps::destructiveExceptMsgCopy(self->viewException_m));

			*nextCont = self->essence_m.onInitialized.onFailure;

			delete self;
		}

		static void onCreatedView_success(esferixis::daw::gui::HNoteSegmentMultigraph *self, esferixis_cps_cont *nextCont) {
			*(self->essence_m.instance) = self;

			*nextCont = self->essence_m.onInitialized.onSuccess;
		}

		static void onElementLoad(esferixis::daw::gui::HNoteSegmentMultigraph *self, esferixis_cps_cont *nextCont) {
			esferixis::daw::gui::HNoteSegmentMultigraph::ElementContext *elementContext = new esferixis::daw::gui::HNoteSegmentMultigraph::ElementContext();

			elementContext->multigraph = self;
			elementContext->noteSegment = self->viewState_m.referencedElement_m;

			esferixis_daw_gui_viewNoteSegment_setUserContext(elementContext->noteSegment, static_cast<void *>(elementContext));

			self->loadedElements_m.addLast(&elementContext->node_m);

			*nextCont = self->viewState_m.onUpdated.onSuccess;
		}

		static void onElementUnload(esferixis::daw::gui::HNoteSegmentMultigraph *self, esferixis_cps_cont *nextCont) {
			esferixis_daw_gui_viewNoteSegment *noteSegment = self->viewState_m.referencedElement_m;

			esferixis::daw::gui::HNoteSegmentMultigraph::ElementContext *elementContext = static_cast<esferixis::daw::gui::HNoteSegmentMultigraph::ElementContext *>( esferixis_daw_gui_viewNoteSegment_getUserContext( noteSegment ) );

			self->loadedElements_m.remove(&elementContext->node_m);

			esferixis_daw_gui_viewNoteSegment_setUserContext(noteSegment, nullptr);

			delete elementContext;

			*nextCont = self->viewState_m.onUpdated.onSuccess;
		}

		static void onNewElementsBoundingBox(esferixis::daw::gui::HNoteSegmentMultigraph *self, esferixis_cps_cont *nextCont) {
			esferixis::Qt::Application::toGuiThread(esferixis::cps::mkCont(onUpdateScrollBars, self), nextCont);
		}

		static void onUpdateScrollBars(esferixis::daw::gui::HNoteSegmentMultigraph *self, esferixis_cps_cont *nextCont) {
			const esferixis_rectf viewArea = esferixis_daw_gui_modifiableview_getViewArea(self->view_m);
			const esferixis_vec2f viewAreaSize = esferixis_rectf_size(viewArea);

			const esferixis_rectf elementsBoundingBox = esferixis_daw_gui_modifiableview_getElementsBoundingBox(self->view_m);
			const esferixis_vec2f elementsBoundingBoxSize = esferixis_rectf_size(elementsBoundingBox);

			if (elementsBoundingBoxSize.x > viewAreaSize.x) {
				const double width = (double)elementsBoundingBoxSize.x - (double)viewAreaSize.x;

				const double pageWidth = (double)SELFCLASS::scrollBarMaxValue * (double)viewAreaSize.x / width;
				const int position = std::min(
					SELFCLASS::scrollBarMaxValue,
					std::max(0, (int) ( ( (double)viewArea.topLeft.x - (double)elementsBoundingBox.topLeft.x) / width ) )
				);

				self->hScrollBar_m->setSingleStep(pageWidth * 0.01);
				self->hScrollBar_m->setPageStep(pageWidth);
				self->hScrollBar_m->setValue(position);
				self->hScrollBar_m->setEnabled(true);
			}
			else {
				self->hScrollBar_m->setEnabled(false);
			}

			if (elementsBoundingBoxSize.y > viewAreaSize.y) {
				const double height = (double)elementsBoundingBoxSize.y - (double)viewAreaSize.y;

				const double pageHeight = (double)SELFCLASS::scrollBarMaxValue * (double)viewAreaSize.y / (double) height;
				const int position = std::min(
					SELFCLASS::scrollBarMaxValue,
					std::max(0, (int)(((double)viewArea.topLeft.y - (double)elementsBoundingBox.topLeft.y) / height))
				);

				self->vScrollBar_m->setSingleStep(pageHeight * 0.01);
				self->vScrollBar_m->setPageStep(pageHeight);
				self->vScrollBar_m->setValue(position);
				self->vScrollBar_m->setEnabled(true);
			}
			else {
				self->vScrollBar_m->setEnabled(false);
			}


			*nextCont = self->viewState_m.onUpdated.onSuccess;
		}

		static void onClose_deleteSelf(SELFCLASS *self, esferixis_cps_cont *nextCont) {
			esferixis_cps_unsafecont cont = self->nextExternalCont_m;

			self->pictureWidget_m->detachFromMultigraph();

			delete self;

			*nextCont = cont.onSuccess;
		}

		static void onClose_onFailure(SELFCLASS *self, esferixis_cps_cont *nextCont) {
			*(self->nextExternalCont_m.exception) = esferixis::cps::createException("Cannot destroy multigraph because its view couldn't have been destroyed -> " + esferixis::cps::destructiveExceptMsgCopy(self->viewException_m));

			*nextCont = self->nextExternalCont_m.onFailure;
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

		e->stateFeedback.element = (void **) &(self->viewState_m.referencedElement_m);
		e->stateFeedback.onElementLoad = esferixis::cps::mkCont(STM::onElementLoad, self);
		e->stateFeedback.onElementUnload = esferixis::cps::mkCont(STM::onElementLoad, self);
		e->stateFeedback.onNewElementsBoundingBox = esferixis::cps::mkCont(STM::onNewElementsBoundingBox, self);
		e->stateFeedback.onUpdated = &(self->viewState_m.onUpdated);
	}

	self->pictureWidget_m = nullptr;
	self->view_m = nullptr;
	self->essence_m = essence;

	self->backgroundColor_m = essence.backgroundColor;
	self->grid_m = essence.grid;

	{
		esferixis_cps_unsafecont nextUnsafeCont;
		nextUnsafeCont.exception = &(self->viewException_m);
		nextUnsafeCont.onFailure = esferixis::cps::mkCont(STM::onClose_onFailure, self);
		nextUnsafeCont.onSuccess = esferixis::cps::mkCont(STM::onClose_deleteSelf, self);
		self->closeViewContext_m = esferixis::cps::MethodProcedureContext<void>(nullptr, nextUnsafeCont);
	}

	esferixis::Qt::Application::toGuiThread(esferixis::cps::mkCont(STM::onCreateWidget, self), nextCont);
}

QWidget * SELFCLASS::widget() const {
	return this->rootWidget_m;
}

void SELFCLASS::setBackgroundColor(QColor color) {
	this->backgroundColor_m = color;
}

void SELFCLASS::setGrid(esferixis::daw::gui::Grid grid) {
	this->grid_m = grid;
}

void SELFCLASS::destroy(const esferixis_cps_unsafecont cont, esferixis_cps_cont *nextCont) {
	this->nextExternalCont_m = cont;

	esferixis_daw_gui_modifiableview_close(this->view_m, this->closeViewContext_m.cContext(), nextCont);
}

SELFCLASS::PictureWidget::PictureWidget(esferixis::daw::gui::HNoteSegmentMultigraph *multigraph) {
	this->multigraph_m = multigraph;
}

void SELFCLASS::PictureWidget::paintEvent(QPaintEvent *event) {
	const esferixis_rectf viewArea = esferixis_daw_gui_modifiableview_getViewArea(this->multigraph_m->view_m);
	const esferixis_vec2f viewAreaSize = esferixis_rectf_size(viewArea);
	const esferixis_vec2f sizeFactor = esferixis_vec2f_new((float)this->size().width() / (float)viewAreaSize.x, (float) this->size().height() / (float)viewAreaSize.y);

	if (this->multigraph_m != nullptr) {
		QPainter painter(this);
		esferixis::Qt::PainterAux painterAux(painter);

		painter.setRenderHint(QPainter::Antialiasing, false);
		painter.fillRect(event->rect(), this->multigraph_m->backgroundColor_m);

		painter.save();

		painterAux.render(
			this->multigraph_m->grid_m.translate( -esferixis::math::Vec2f(viewArea.topLeft) ).scale( esferixis::math::Vec2f(sizeFactor) ),
			event->rect()
		);

		painter.restore();

		// FIXME: Implement this
	}
}

void SELFCLASS::PictureWidget::detachFromMultigraph() {
	this->multigraph_m = nullptr;
}