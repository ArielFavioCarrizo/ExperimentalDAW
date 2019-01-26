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

#include "MultigraphCViewMock.h"
#include "MultigraphCHNoteSegmentMock.h"

#define SELFCLASS esferixis::daw::gui::test::MultigraphCViewMock

esferixis_cps_cont SELFCLASS::create(esferixis::daw::gui::MultigraphCView<esferixis::daw::gui::MultigraphCHNoteSegment, esferixis::daw::gui::MultigraphCHNoteSegment::Essence, esferixis::daw::gui::MultigraphCHNoteSegment::StateFeedback>::ContextEssence contextEssence) {
	/*
	*(contextEssence.onInitialized.exception) = esferixis::cps::createException("Mock failure");

	return contextEssence.onInitialized.onFailure;
	*/

	SELFCLASS *self = new SELFCLASS();

	*contextEssence.instance = self;

	self->stateFeedback_m = contextEssence.stateFeedback;

	*(self->stateFeedback_m.element) = nullptr;
	*(self->stateFeedback_m.elementStateFeedback) = nullptr;
	*(self->stateFeedback_m.onUpdated) = esferixis_cps_mkInvalidUnsafeCont();

	self->onClosed_m = esferixis_cps_mkInvalidUnsafeCont();

	return contextEssence.onCreated.onSuccess;
}

SELFCLASS::MultigraphCViewMock()
{
	
}


SELFCLASS::~MultigraphCViewMock()
{
}

esferixis_cps_cont SELFCLASS::createElement(esferixis::daw::gui::MultigraphCHNoteSegment::Essence elementEssence, esferixis_cps_unsafecont cont) {
	esferixis::daw::gui::test::MultigraphCHNoteSegmentMock *element = new esferixis::daw::gui::test::MultigraphCHNoteSegmentMock(elementEssence, this);

	*(this->stateFeedback_m.element) = element;
	*(this->stateFeedback_m.elementStateFeedback) = &(element->stateFeedback);

	return this->stateFeedback_m.onElementLoad;
}

esferixis_cps_cont SELFCLASS::lockElement(esferixis::daw::gui::MultigraphCHNoteSegment *element, esferixis_cps_cont cont) {
	return cont; // Do nothing
}

esferixis_cps_cont SELFCLASS::unlockElement(esferixis::daw::gui::MultigraphCHNoteSegment *element, esferixis_cps_cont cont) {
	return cont; // Do nothing
}

esferixis_cps_cont SELFCLASS::close(esferixis_cps_unsafecont cont) {
	struct STM {
		static esferixis_cps_cont findElement(SELFCLASS *self) {
			if (!self->noteSegments_m.isEmpty()) {
				esferixis_cps_unsafecont cont;

				cont.exception = &(self->exception_m);
				cont.onSuccess = esferixis::cps::mkCont(findElement, self);
				cont.onFailure = esferixis::cps::mkCont(onFailure, self);

				return self->noteSegments_m.first()->get()->erase(cont);
			}
			else {
				esferixis_cps_cont cont = self->onClosed_m.onSuccess;

				delete self;

				return cont;
			}
		}

		static esferixis_cps_cont onFailure(SELFCLASS *self) {
			*(self->onClosed_m.exception) = esferixis::cps::createException("Cannot destroy element" + esferixis::cps::destructiveExceptMsgCopy(self->exception_m));

			return self->onClosed_m.onFailure;
		}
	};

	this->onClosed_m = cont;

	return esferixis::cps::mkCont(STM::findElement, this);
}