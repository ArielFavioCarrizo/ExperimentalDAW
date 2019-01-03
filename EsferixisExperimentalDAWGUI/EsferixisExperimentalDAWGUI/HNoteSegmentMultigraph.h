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
#include <esferixis/common/data/linkedlist.h>

#include <qwidget.h>

namespace esferixis {
	namespace daw {
		namespace gui {
			class HNoteSegmentMultigraph final : private boost::noncopyable
			{
			public:
				/**
				 * @post Creates an editable multigraph based on horizontal note segments
				         with the specified continuation
				 */
				static esferixis::cps::Cont create(esferixis::daw::gui::HNoteSegmentMultigraph **instance, esferixis::cps::Cont cont);

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
				 * @post Sets the grid color
				 */
				void setForegroundColor(QColor color);

				/**
				 * @post Returns the context essence of the view
			     *		 
				 *		 Warning: Use of this essence must be preceeded by the setting of the 'on opened' continuation
				 */
				esferixis::daw::gui::MultigraphCView<esferixis::daw::gui::MultigraphCHNoteSegment, esferixis::daw::gui::MultigraphCHNoteSegment::Essence>::ContextEssence viewContextEssence() const;

				/**
				 * @post Sets the continuation on opened
				 * 
				 *		 This continuation will be executed after a view is created using the view context essence of
				 *		 this multigraph
				 */
				void setOnOpened(esferixis::cps::Cont cont);

				/**
				 * @post Closes the multiview without destroying it
				 */
				esferixis::cps::Cont close(esferixis::cps::Cont cont);

			private:
				class LocalQWidget final : public QWidget {
				public:
					LocalQWidget(esferixis::daw::gui::HNoteSegmentMultigraph *multigraph);

					void paintEvent(QPaintEvent *event) override;

				private:
					esferixis::daw::gui::HNoteSegmentMultigraph *multigraph_m;
				};

				struct ElementContext {
					esferixis::daw::gui::MultigraphCHNoteSegment *noteSegment;
					esferixis::daw::gui::HNoteSegmentMultigraph *multigraph;

					esferixis::LinkedList<ElementContext *>::Node node_m;
				};

				esferixis::daw::gui::MultigraphCView<esferixis::daw::gui::MultigraphCHNoteSegment, esferixis::daw::gui::MultigraphCHNoteSegment::Essence>::ContextEssence contextEssence_m;

				LocalQWidget *widget_m;
				QColor backgroundColor_m;
				QColor gridColor_m;

				esferixis::daw::gui::MultigraphCView<esferixis::daw::gui::MultigraphCHNoteSegment, esferixis::daw::gui::MultigraphCHNoteSegment::Essence> *view_m;
				esferixis::LinkedList<ElementContext *> loadedElements_m;

				esferixis::cps::Cont nextExternalCont_m;
			};
		}
	}
}
