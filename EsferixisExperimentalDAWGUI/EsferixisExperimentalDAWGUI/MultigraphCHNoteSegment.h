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

#include <boost/noncopyable.hpp>
#include <esferixis/common/cps/cont.h>
#include <esferixis/common/contextualized.h>
#include <QtGui/qcolor.h>

namespace esferixis {
	namespace daw {
		namespace gui {
			class MultigraphCHNoteSegment : public esferixis::Contextualized, private boost::noncopyable
			{
			public:
				struct Essence {
					double offset;
					double height;

					bool isAContinuation;
					bool isSelected;
				};

				/**
				 * @post Gets the offset
				 */
				virtual double getOffset() =0;

				/*
				 * @post Gets the height
				 */
				virtual double getHeight() =0;

				/**
				 * @post Gets the color
				 */
				virtual QColor getColor() =0;

				/**
				 * @post Gets a boolean indicating if the segment is a continuation of the previous note
				 */
				virtual bool isAContinuation() =0;

				/**
				 * @post Gets a boolean indicating if it is selected
				 */
				virtual bool isSelected() =0;

				/**
				 * @post Sets the offset
				 */
				virtual esferixis::cps::Cont setOffset(double offset, esferixis::cps::Cont cont) = 0;

				/**
				 * @post Sets the height
				 */
				virtual esferixis::cps::Cont setHeight(double height, esferixis::cps::Cont cont) = 0;

				/**
				 * @post Sets a boolean indicating if is a continuation
				 */
				virtual esferixis::cps::Cont setIsAContinuation(bool isAContinuation, esferixis::cps::Cont cont) = 0;

				/**
				 * @post Erases the note segment
				 */
				virtual esferixis::cps::Cont erase(esferixis::cps::Cont cont) =0;

				/**
				 * @post Sets the continuation to notify a new offset
				 */
				virtual void setOnNewOffset(esferixis::cps::Cont cont) = 0;

				/**
				 * @post Sets the continuation to notify a new height
				 */
				virtual void setOnNewHeight(esferixis::cps::Cont cont) = 0;

				/**
				 * @post Sets the continuation to notify a new color
				 */
				virtual void setOnNewColor(esferixis::cps::Cont cont) =0;

				/**
				 * @post Sets the continuation to notify a change in the 'isAContinuation' property
				 */
				virtual void setOnIsAContinuationChange(esferixis::cps::Cont cont) =0;

				/**
				 * @post Sets the continuation to notify a new selection state
				 */
				virtual void setOnNewSelectionState(esferixis::cps::Cont cont) = 0;

			protected:
				/**
				 * @post Creates an horizontal note segment
				 */
				MultigraphCHNoteSegment();

				/**
				 * @post Destroys the horizontal note segment
				 */
				virtual ~MultigraphCHNoteSegment();
			};
		}
	}
}
