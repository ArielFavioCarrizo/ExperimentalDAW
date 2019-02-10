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

#include <esferixis/common/math/vec2f.h>
#include <esferixis/daw/gui/common/color.h>

namespace esferixis {
	namespace daw {
		namespace gui {
			class Grid final {
			public:
				/**
				 * @post Creates a default grid
				 */
				inline Grid() {
					this->color_m = esferixis::daw::gui::RGBColor(255, 0, 0);
					this->offset_m = esferixis::math::Vec2f(0.0f, 0.0f);
					this->size_m = esferixis::math::Vec2f(1.0f, 1.0f);
				}

				/**
				 * @post Creates an grid with the specified color,
						 offset and grid size
				 */
				inline Grid(esferixis::daw::gui::RGBColor color, esferixis::math::Vec2f offset, esferixis::math::Vec2f size) {
					this->color_m = color;
					this->offset_m = offset;
					this->size_m = size;
				}

				/**
				 * @post Returns the color
				 */
				inline esferixis::daw::gui::RGBColor color() const {
					return this->color_m;
				}

				/**
				 * @post Returns the offset
				 */
				inline esferixis::math::Vec2f offset() const {
					return this->offset_m;
				}

				/**
				 * @post Returns the size
				 */
				inline esferixis::math::Vec2f size() const {
					return this->size_m;
				}

				/**
				 * @post Translates the grid
				 */
				inline esferixis::daw::gui::Grid translate(esferixis::math::Vec2f vec) {
					return esferixis::daw::gui::Grid(this->color_m, this->offset_m + vec, this->size_m);
				}

			private:
				esferixis::daw::gui::RGBColor color_m;
				esferixis::math::Vec2f offset_m;
				esferixis::math::Vec2f size_m;
			};
		}
	}
}