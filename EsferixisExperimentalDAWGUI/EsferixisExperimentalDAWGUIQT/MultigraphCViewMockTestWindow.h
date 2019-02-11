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
#include <qwidget.h>

#include <esferixis/common/cps/cont.h>
#include <esferixis/common/cps/exception.h>

#include "MultigraphCView.h"
#include "HNoteSegmentMultigraph.h"

namespace esferixis {
	namespace daw {
		namespace gui {
			namespace test {
				class MultigraphCViewWindowMock
				{
				public:
					struct Essence {
						esferixis::daw::gui::test::MultigraphCViewWindowMock **windowMock;

						esferixis_cps_unsafecont onCreated;
						esferixis_cps_unsafecont onClosed;
					};

					/**
					 * @pre The GUI must be locked
					 * @post Creates a multigraphCView window mock
					 */
					static void create(Essence essence, esferixis_cps_cont *nextCont);

				private:
					class LocalWindow : public QWidget {
					public:
						LocalWindow(esferixis::daw::gui::test::MultigraphCViewWindowMock *parent);

					protected:
						void closeEvent(QCloseEvent *event) override;

					private:
						esferixis::daw::gui::test::MultigraphCViewWindowMock *parent_m;
					};

					/**
					 * @post Creates a window of multigraphCView window mock
					 */
					MultigraphCViewWindowMock();

					Essence essence_m;

					esferixis::daw::gui::MultigraphCView<esferixis::daw::gui::MultigraphCHNoteSegment, esferixis::daw::gui::MultigraphCHNoteSegment::Essence, esferixis::daw::gui::MultigraphCHNoteSegment::StateFeedback>::ContextEssence viewContextEssence_m;
					struct ViewState {
						esferixis::daw::gui::MultigraphCHNoteSegment::StateFeedback *elementStateFeedback;
					};
					ViewState viewState_m;

					QWidget *window_m;
					esferixis::daw::gui::HNoteSegmentMultigraph *multigraph_m;
					esferixis_cps_exception multigraphException_m;

					esferixis_cps_cont onNextExternalOp_m;
					esferixis_cps_cont onClosed_m;
				};
			}
		}
	}
}
