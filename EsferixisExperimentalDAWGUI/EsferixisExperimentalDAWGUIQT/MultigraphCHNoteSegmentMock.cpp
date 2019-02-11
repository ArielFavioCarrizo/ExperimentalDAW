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

bool SELFCLASS::isSelected() {
	return this->isSelected_m;
}

esferixis_cps_cont SELFCLASS::setOffset(double offset, esferixis_cps_unsafecont cont) {
	*(this->multigraphCViewMock_m->stateFeedback_m.onUpdated) = cont;

	this->timeOffset_m = offset;

	return this->stateFeedback.onNewOffset;
}

esferixis_cps_cont SELFCLASS::setHeight(double height, esferixis_cps_unsafecont cont) {
	*(this->multigraphCViewMock_m->stateFeedback_m.onUpdated) = cont;

	this->height_m = height;

	return this->stateFeedback.onNewHeight;
}

esferixis_cps_cont SELFCLASS::erase(esferixis_cps_unsafecont cont) {
	struct STM {
		static void deleteItself(SELFCLASS *self, esferixis_cps_cont *nextCont) {
			*nextCont = self->returnCont_m.onSuccess;

			self->multigraphCViewMock_m->noteSegments_m.remove(&(self->containerNode_m));

			delete self;
		}

		static void onFailure(SELFCLASS *self, esferixis_cps_cont *nextCont) {
			*(self->returnCont_m.exception) = esferixis::cps::createException("Cannot remove element because notification has failed -> " + esferixis::cps::destructiveExceptMsgCopy(self->externalException_m));

			*nextCont = self->returnCont_m.onFailure;
		}
	};

	this->returnCont_m = cont;

	*(this->multigraphCViewMock_m->stateFeedback_m.element) = this;

	this->multigraphCViewMock_m->stateFeedback_m.onUpdated->exception = &(this->externalException_m);
	this->multigraphCViewMock_m->stateFeedback_m.onUpdated->onFailure = esferixis::cps::mkCont(STM::onFailure, this);
	this->multigraphCViewMock_m->stateFeedback_m.onUpdated->onSuccess = esferixis::cps::mkCont(STM::deleteItself, this);

	return this->multigraphCViewMock_m->stateFeedback_m.onElementUnload;
}