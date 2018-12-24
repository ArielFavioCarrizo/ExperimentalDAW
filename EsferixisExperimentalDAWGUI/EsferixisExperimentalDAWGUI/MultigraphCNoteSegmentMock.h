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
#include "MultigraphCNoteSegment.h"

namespace esferixis {
	namespace daw {
		namespace gui {
			namespace test {
				class MultigraphCViewMock;

				class MultigraphCNoteSegmentMock : public esferixis::daw::gui::MultigraphCNoteSegment
				{
					friend class MultigraphCViewMock;

				public:
					double getOffset() override;

					double getHeight() override;

					QColor getColor() override;

					bool isAContinuation() override;

					bool isSelected() override;

					void setOnNewSelectionState(esferixis::cps::Cont cont) override;

					void setOnNewColor(esferixis::cps::Cont cont) override;

					void setOnIsAContinuationChange(esferixis::cps::Cont cont) override;

					esferixis::cps::Cont select(esferixis::cps::Cont cont) override;

					esferixis::cps::Cont deselect(esferixis::cps::Cont cont) override;

				protected:
					/**
					 * @post Creates a note segment mock
					 */
					MultigraphCNoteSegmentMock();

					/**
					 * @post Destroys the note segment mock
					 */
					virtual ~MultigraphCNoteSegmentMock();

				private:

				};
			}
		}
	}
}

