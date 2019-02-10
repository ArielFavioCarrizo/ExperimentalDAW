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

#include <esferixis/common/cps/cont.h>
#include <esferixis/common/cps/exception.h>
#include <esferixis/common/data/linkedlist.h>
#include <esferixis/daw/gui/common/grid.h>

#include <qwidget.h>

namespace esferixis {
	namespace daw {
		namespace gui {
			class HNoteSegmentMultigraph final : private boost::noncopyable
			{
			public:
				struct Essence {
					esferixis::daw::gui::HNoteSegmentMultigraph **instance;
					esferixis::daw::gui::MultigraphCView<esferixis::daw::gui::MultigraphCHNoteSegment, esferixis::daw::gui::MultigraphCHNoteSegment::Essence, esferixis::daw::gui::MultigraphCHNoteSegment::StateFeedback>::ContextEssence *viewContextEssence;

					QRectF viewArea;

					QColor backgroundColor;
					esferixis::daw::gui::Grid grid;

					esferixis_cps_cont onWaitingViewCreation;

					esferixis_cps_unsafecont onInitialized;
				};

				/**
				 * @post Creates an editable multigraph based on horizontal notes with the specified essence
				 */
				static esferixis_cps_cont create(Essence essence);

				/*
				 * @post Gets the QT widget
				 */
				QWidget * widget() const;

				/**
				 * @pre It must be done from the GUI thread
				 * @post Sets the background color
				 */
				void setBackgroundColor(QColor color);

				/**
				 * @pre It must be done from the GUI thread
				 * @post Sets the grid
				 */
				void setGrid(esferixis::daw::gui::Grid grid);

				/**
				 * @post Destroys the multiview without destroying the widget
				 */
				esferixis_cps_cont destroy(esferixis_cps_unsafecont cont);

			private:
				class LocalQWidget final : public QWidget {
				public:
					LocalQWidget(esferixis::daw::gui::HNoteSegmentMultigraph *multigraph);

					void paintEvent(QPaintEvent *event) override;

					void detachFromMultigraph();

				private:
					esferixis::daw::gui::HNoteSegmentMultigraph *multigraph_m;
				};

				struct ElementContext {
					esferixis::daw::gui::MultigraphCHNoteSegment *noteSegment;
					esferixis::daw::gui::HNoteSegmentMultigraph *multigraph;

					esferixis::LinkedList<ElementContext *>::Node node_m;
				};

				Essence essence_m;

				LocalQWidget *widget_m;

				QColor backgroundColor_m;
				esferixis::daw::gui::Grid grid_m;

				esferixis::daw::gui::MultigraphCView<esferixis::daw::gui::MultigraphCHNoteSegment, esferixis::daw::gui::MultigraphCHNoteSegment::Essence, esferixis::daw::gui::MultigraphCHNoteSegment::StateFeedback> *view_m;

				struct ViewState {
					esferixis::daw::gui::MultigraphCHNoteSegment *referencedElement_m;
					esferixis::daw::gui::MultigraphCHNoteSegment::StateFeedback *elementStateFeedback_m;

					esferixis_cps_unsafecont onUpdated;
				};
				esferixis_cps_exception viewException_m;

				ViewState viewState_m;

				esferixis::LinkedList<ElementContext *> loadedElements_m;

				esferixis_cps_unsafecont nextExternalCont_m;
			};
		}
	}
}
