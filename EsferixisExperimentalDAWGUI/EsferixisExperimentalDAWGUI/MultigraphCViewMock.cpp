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

esferixis_cps_cont SELFCLASS::create(esferixis::daw::gui::MultigraphCView<esferixis::daw::gui::MultigraphCHNoteSegment, esferixis::daw::gui::MultigraphCHNoteSegment::Essence>::ContextEssence contextEssence) {
	SELFCLASS *self = new SELFCLASS();

	*contextEssence.instance = self;

	self->onElementLoad_m = contextEssence.onElementLoad;
	self->onElementUnload_m = contextEssence.onElementUnload;

	return contextEssence.onInitialized;
}

SELFCLASS::MultigraphCViewMock()
{
	
}


SELFCLASS::~MultigraphCViewMock()
{
}

esferixis::daw::gui::MultigraphCHNoteSegment * SELFCLASS::getReferencedElement() {
	return this->referencedElement_m;
}

esferixis_cps_cont SELFCLASS::createElement(esferixis::daw::gui::MultigraphCHNoteSegment::Essence elementEssence, esferixis_cps_cont cont) {
	esferixis::daw::gui::test::MultigraphCHNoteSegmentMock *element = new esferixis::daw::gui::test::MultigraphCHNoteSegmentMock(elementEssence, this);

	this->referencedElement_m = element;

	this->nextExternalActionCont_m = cont;

	return this->onElementLoad_m;
}

void SELFCLASS::setOnElementLoad(esferixis_cps_cont cont) {
	this->onElementLoad_m = cont;
}

void SELFCLASS::setOnElementToUnload(esferixis_cps_cont cont) {
	this->onElementUnload_m = cont;
}

esferixis_cps_cont SELFCLASS::setTimeIntervalToView(double min, double max, esferixis_cps_cont cont) {
	return cont; // Do nothing
}

esferixis_cps_cont SELFCLASS::lockElement(esferixis::daw::gui::MultigraphCHNoteSegment *element, esferixis_cps_cont cont) {
	return cont; // Do nothing
}

esferixis_cps_cont SELFCLASS::unlockElement(esferixis::daw::gui::MultigraphCHNoteSegment *element, esferixis_cps_cont cont) {
	return cont; // Do nothing
}

esferixis_cps_cont SELFCLASS::close(esferixis_cps_cont cont) {
	struct STM {
		static esferixis_cps_cont loop(SELFCLASS *self) {
			if (!self->noteSegments_m.isEmpty()) {
				return self->noteSegments_m.first()->get()->erase(esferixis::cps::mkCont(loop, self));
			}
			else {
				esferixis_cps_cont cont = self->onClosed_m;

				delete self;

				return cont;
			}
		}
	};

	this->onClosed_m = cont;

	return esferixis::cps::mkCont(STM::loop, this);
}

esferixis_cps_cont SELFCLASS::doNextAction() {
	return this->nextExternalActionCont_m;
}