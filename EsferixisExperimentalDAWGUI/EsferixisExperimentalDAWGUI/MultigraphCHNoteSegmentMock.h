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
#include "MultigraphCHNoteSegment.h"

#include "esferixis/common/data/linkedlist.h"

namespace esferixis {
	namespace daw {
		namespace gui {
			namespace test {
				class MultigraphCViewMock;

				class MultigraphCHNoteSegmentMock : public esferixis::daw::gui::MultigraphCHNoteSegment
				{
					friend class MultigraphCViewMock;

				public:
					double getTimeOffset() override;

					double getHeight() override;

					QColor getColor() override;

					bool isAContinuation() override;

					bool isSelected() override;

					esferixis_cps_cont setOffset(double offset, esferixis_cps_cont cont) override;

					esferixis_cps_cont setHeight(double height, esferixis_cps_cont cont) override;

					esferixis_cps_cont setIsAContinuation(bool isAContinuation, esferixis_cps_cont cont) override;

					esferixis_cps_cont erase(esferixis_cps_cont cont) override;

					void setOnNewOffset(esferixis_cps_cont cont) override;

					void setOnNewHeight(esferixis_cps_cont cont) override;

					void setOnNewColor(esferixis_cps_cont cont) override;

					void setOnIsAContinuationChange(esferixis_cps_cont cont) override;

					void setOnNewSelectionState(esferixis_cps_cont cont) override;

				protected:
					/**
					 * @post Destroys the note segment mock
					 */
					virtual ~MultigraphCHNoteSegmentMock();

				private:
					/**
					 * @post Creates a note segment mock with the specified essence and
					 *		 multigraph view mock
					 */
					MultigraphCHNoteSegmentMock(esferixis::daw::gui::MultigraphCHNoteSegment::Essence essence, esferixis::daw::gui::test::MultigraphCViewMock *multigraphCViewMock);

					esferixis::daw::gui::test::MultigraphCViewMock *multigraphCViewMock_m;
					esferixis::LinkedList<esferixis::daw::gui::MultigraphCHNoteSegment *>::Node containerNode_m;

					double timeOffset_m;
					double height_m;
					bool isAContinuation_m;
					bool isSelected_m;

					esferixis_cps_cont onNewOffset_m;
					esferixis_cps_cont onNewHeight_m;
					esferixis_cps_cont onNewColor_m;
					esferixis_cps_cont onIsAContinuationChange_m;
					esferixis_cps_cont onNewSelectionState_m;

					esferixis_cps_cont returnCont_m;
				};
			}
		}
	}
}

