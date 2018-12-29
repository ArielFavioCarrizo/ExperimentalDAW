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

#include "MultigraphCHNoteSegmentMock.h"

#include "MultigraphCViewMock.h"

#define SELFCLASS esferixis::daw::gui::test::MultigraphCHNoteSegmentMock

SELFCLASS::MultigraphCHNoteSegmentMock(esferixis::daw::gui::MultigraphCHNoteSegment::Essence essence, esferixis::daw::gui::test::MultigraphCViewMock *multigraphCViewMock) :
	multigraphCViewMock_m(multigraphCViewMock),
	containerNode_m(this)
{
	this->multigraphCViewMock_m->noteSegments_m.addLast(&(this->containerNode_m));

	this->offset_m = essence.offset;
	this->height_m = essence.height;
	this->isAContinuation_m = false;
	this->isSelected_m = false;
}


SELFCLASS::~MultigraphCHNoteSegmentMock()
{
}

double SELFCLASS::getOffset() {
	return this->offset_m;
}

double SELFCLASS::getHeight() {
	return this->height_m;
}

QColor SELFCLASS::getColor() {
	return Qt::GlobalColor::red;
}

bool SELFCLASS::isAContinuation() {
	return this->isAContinuation_m;
}

bool SELFCLASS::isSelected() {
	return this->isSelected_m;
}

esferixis::cps::Cont SELFCLASS::setOffset(double offset, esferixis::cps::Cont cont) {
	this->multigraphCViewMock_m->nextExternalActionCont_m = cont;

	this->offset_m = offset;
	return this->onNewOffset_m;
}

esferixis::cps::Cont SELFCLASS::setHeight(double height, esferixis::cps::Cont cont) {
	this->multigraphCViewMock_m->nextExternalActionCont_m = cont;

	this->height_m = height;
	return this->onNewHeight_m;
}

esferixis::cps::Cont SELFCLASS::setIsAContinuation(bool isAContinuation, esferixis::cps::Cont cont) {
	this->multigraphCViewMock_m->nextExternalActionCont_m = cont;

	this->isAContinuation_m = isAContinuation;
	return this->onIsAContinuationChange_m;
}

esferixis::cps::Cont SELFCLASS::erase(esferixis::cps::Cont cont) {
	this->multigraphCViewMock_m->referencedElement_m = this;
	this->multigraphCViewMock_m->nextExternalActionCont_m = esferixis::cps::Cont(SELFCLASS::deleteItself, this);
	this->returnCont_m = cont;

	return this->multigraphCViewMock_m->onElementUnload_m;
}

esferixis::cps::Cont SELFCLASS::deleteItself(esferixis::daw::gui::test::MultigraphCHNoteSegmentMock *self) {
	esferixis::daw::gui::test::MultigraphCViewMock *view = self->multigraphCViewMock_m;
	esferixis::cps::Cont cont = self->returnCont_m;

	view->noteSegments_m.remove(&(self->containerNode_m));

	delete self;

	return cont;
}

void SELFCLASS::setOnNewOffset(esferixis::cps::Cont cont) {
	this->onNewOffset_m = cont;
}

void SELFCLASS::setOnNewHeight(esferixis::cps::Cont cont) {
	this->onNewHeight_m = cont;
}

void SELFCLASS::setOnNewColor(esferixis::cps::Cont cont) {
	// Do nothing
}

void SELFCLASS::setOnIsAContinuationChange(esferixis::cps::Cont cont) {
	this->onIsAContinuationChange_m = cont;
}

void SELFCLASS::setOnNewSelectionState(esferixis::cps::Cont cont) {
	this->onNewSelectionState_m = cont;
}