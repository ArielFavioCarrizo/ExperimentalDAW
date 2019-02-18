/*
BSD 3-Clause License

Copyright (c) 2019, Ariel Favio Carrizo
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
#include <esferixis/common/graphics/rgbcolor.h>

EsferixisCommon_C_BEGIN

typedef struct _esferixis_daw_gui_grid {
	esferixis_rgbcolor color;
	esferixis_vec2f offset;
	esferixis_vec2f size;
} esferixis_daw_gui_grid;

inline esferixis_daw_gui_grid esferixis_daw_gui_grid_translate(esferixis_daw_gui_grid grid, esferixis_vec2f vec) {
	grid.offset = esferixis_vec2f_add(grid.offset, vec);

	return grid;
}

EsferixisCommon_C_END

namespace esferixis {
	namespace daw {
		namespace gui {
			class Grid final {
			public:
				/**
				 * @post Creates a default grid
				 */
				inline Grid() {
					this->cGrid_m.color = esferixis_rgbcolor_new(255, 0, 0);
					this->cGrid_m.offset = esferixis_vec2f_new(0.0f, 0.0f);
					this->cGrid_m.size = esferixis_vec2f_new(1.0f, 1.0f);
				}

				/**
				 * @post Creates a grid from the C version
				 */
				inline Grid(esferixis_daw_gui_grid cGrid) {
					this->cGrid_m = cGrid;
				}

				/**
				 * @post Creates an grid with the specified color,
						 offset and grid size
				 */
				inline Grid(esferixis::RGBColor color, esferixis::math::Vec2f offset, esferixis::math::Vec2f size) {
					this->cGrid_m.color = color.cColor();
					this->cGrid_m.offset = offset.cVec2f();
					this->cGrid_m.size = size.cVec2f();
				}

				/**
				 * @post Returns the color
				 */
				inline esferixis::RGBColor color() const {
					return esferixis::RGBColor(this->cGrid_m.color);
				}

				/**
				 * @post Returns the offset
				 */
				inline esferixis::math::Vec2f offset() const {
					return esferixis::math::Vec2f(this->cGrid_m.offset);
				}

				/**
				 * @post Returns the size
				 */
				inline esferixis::math::Vec2f size() const {
					return esferixis::math::Vec2f(this->cGrid_m.size);
				}

				/**
				 * @post Translates the grid
				 */
				inline esferixis::daw::gui::Grid translate(esferixis::math::Vec2f vec) const {
					return esferixis::daw::gui::Grid( esferixis_daw_gui_grid_translate(this->cGrid_m, vec.cVec2f()) );
				}

			private:
				esferixis_daw_gui_grid cGrid_m;
			};
		}
	}
}