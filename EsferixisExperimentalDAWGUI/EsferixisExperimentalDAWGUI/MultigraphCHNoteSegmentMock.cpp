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

	this->timeOffset_m = essence.offset;
	this->height_m = essence.height;
	this->isAContinuation_m = false;
	this->isSelected_m = false;
}


SELFCLASS::~MultigraphCHNoteSegmentMock()
{
}

double SELFCLASS::getTimeOffset() {
	return this->timeOffset_m;
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

esferixis_cps_cont SELFCLASS::setOffset(double offset, esferixis_cps_cont cont) {
	this->multigraphCViewMock_m->nextExternalActionCont_m = cont;

	this->timeOffset_m = offset;

	return this->onNewOffset_m;
}

esferixis_cps_cont SELFCLASS::setHeight(double height, esferixis_cps_cont cont) {
	this->multigraphCViewMock_m->nextExternalActionCont_m = cont;

	this->height_m = height;

	return this->onNewHeight_m;
}

esferixis_cps_cont SELFCLASS::setIsAContinuation(bool isAContinuation, esferixis_cps_cont cont) {
	this->multigraphCViewMock_m->nextExternalActionCont_m = cont;

	this->isAContinuation_m = isAContinuation;
	return this->onIsAContinuationChange_m;
}

esferixis_cps_cont SELFCLASS::erase(esferixis_cps_cont cont) {
	struct STM {
		static esferixis_cps_cont deleteItself(SELFCLASS *self) {
			esferixis_cps_cont cont = self->returnCont_m;

			self->multigraphCViewMock_m->noteSegments_m.remove(&(self->containerNode_m));

			delete self;

			return cont;
		}
	};

	this->multigraphCViewMock_m->referencedElement_m = this;
	this->multigraphCViewMock_m->nextExternalActionCont_m = esferixis::cps::mkCont(STM::deleteItself, this);
	this->returnCont_m = cont;

	return this->multigraphCViewMock_m->onElementUnload_m;
}

void SELFCLASS::setOnNewOffset(esferixis_cps_cont cont) {
	this->onNewOffset_m = cont;
}

void SELFCLASS::setOnNewHeight(esferixis_cps_cont cont) {
	this->onNewHeight_m = cont;
}

void SELFCLASS::setOnNewColor(esferixis_cps_cont cont) {
	this->onNewColor_m = cont;
}

void SELFCLASS::setOnIsAContinuationChange(esferixis_cps_cont cont) {
	this->onIsAContinuationChange_m = cont;
}

void SELFCLASS::setOnNewSelectionState(esferixis_cps_cont cont) {
	this->onNewSelectionState_m = cont;
}