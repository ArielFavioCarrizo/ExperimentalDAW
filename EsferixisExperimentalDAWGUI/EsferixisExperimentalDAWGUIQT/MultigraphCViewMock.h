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

#pragma once

#include "MultigraphCView.h"
#include "MultigraphCHNoteSegment.h"

#include "esferixis/common/data/linkedlist.h"

namespace esferixis {
	namespace daw {
		namespace gui {
			namespace test {
				class MultigraphCHNoteSegmentMock;

				class MultigraphCViewMock : public esferixis::daw::gui::MultigraphCView<esferixis::daw::gui::MultigraphCHNoteSegment, esferixis::daw::gui::MultigraphCHNoteSegment::Essence, esferixis::daw::gui::MultigraphCHNoteSegment::StateFeedback>
				{
					friend class MultigraphCHNoteSegmentMock;

				public:
					/**
					 * @post Creates the view mock
					 */
					static void create(esferixis::daw::gui::MultigraphCView<esferixis::daw::gui::MultigraphCHNoteSegment, esferixis::daw::gui::MultigraphCHNoteSegment::Essence, esferixis::daw::gui::MultigraphCHNoteSegment::StateFeedback>::ContextEssence contextEssence, esferixis_cps_cont *nextCont);

					void createElement(esferixis::daw::gui::MultigraphCHNoteSegment::Essence elementEssence, esferixis_cps_unsafecont cont, esferixis_cps_cont *nextCont) override;

					void setViewArea(QRectF viewArea, esferixis_cps_unsafecont cont, esferixis_cps_cont *nextCont) override;

					void lockElement(esferixis::daw::gui::MultigraphCHNoteSegment *element, esferixis_cps_cont cont, esferixis_cps_cont *nextCont) override;

					void unlockElement(esferixis::daw::gui::MultigraphCHNoteSegment *element, esferixis_cps_cont cont, esferixis_cps_cont *nextCont) override;

					void close(esferixis_cps_unsafecont cont, esferixis_cps_cont *nextCont) override;

				private:
					/**
					 * @post Creates a mock
					 */
					MultigraphCViewMock();

					/**
					 * @post Destroys the mock
					 */
					virtual ~MultigraphCViewMock();

					esferixis::LinkedList<esferixis::daw::gui::MultigraphCHNoteSegment *> noteSegments_m;

					esferixis::daw::gui::MultigraphCView<esferixis::daw::gui::MultigraphCHNoteSegment, esferixis::daw::gui::MultigraphCHNoteSegment::Essence, esferixis::daw::gui::MultigraphCHNoteSegment::StateFeedback>::StateFeedback stateFeedback_m;

					esferixis_cps_unsafecont onClosed_m;
					esferixis_cps_exception exception_m;

					esferixis_cps_cont nextExternalActionCont_m;
				};
			}
		}
	}
}

