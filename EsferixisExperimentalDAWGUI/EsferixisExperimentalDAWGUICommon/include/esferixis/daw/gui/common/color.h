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

#include <cstdint>

namespace esferixis {
	namespace daw {
		namespace gui {
			class RGBColor final {
			public:
				/**
				 * @post Creates a black RGB Color
				 */
				inline RGBColor() {
					this->red_m = 0.0f;
					this->green_m = 0.0f;
					this->blue_m = 0.0f;
				}

				/**
				 * @post Creates a RGB color
				 */
				inline RGBColor(uint8_t red, uint8_t green, uint8_t blue) {
					this->red_m = red;
					this->green_m = green;
					this->blue_m = blue;
				}

				/**
				 * @post Gets the red component
				 */
				inline uint8_t red() {
					return this->red_m;
				}

				/**
				 * @post Gets the green component
				 */
				inline uint8_t green() {
					return this->green_m;
				}

				/**
				 * @post Gets the blue component
				 */
				inline uint8_t blue() {
					return this->blue_m;
				}

			private:
				uint8_t red_m;
				uint8_t green_m;
				uint8_t blue_m;
			};
		}
	}
}