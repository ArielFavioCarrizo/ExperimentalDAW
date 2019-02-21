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

#include <esferixis/common/common.h>

#ifdef __cplusplus
#include <cstdint>
#else
#include "stdint.h"
#endif

EsferixisCommon_C_BEGIN

typedef struct _esferixis_rgbcolor {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} esferixis_rgbcolor;

inline esferixis_rgbcolor esferixis_rgbcolor_new(uint8_t red, uint8_t green, uint8_t blue) {
	esferixis_rgbcolor result;

	result.red = red;
	result.green = green;
	result.blue = blue;

	return result;
}

EsferixisCommon_C_END

#ifdef __cplusplus
namespace esferixis {
	class RGBColor final {
	public:
		/**
			* @post Creates a black RGB Color
			*/
		inline RGBColor() {
			this->cColor_m = esferixis_rgbcolor_new(0, 0, 0);
		}

		/**
			* @post Creates a RGB color
			*/
		inline RGBColor(uint8_t red, uint8_t green, uint8_t blue) {
			this->cColor_m = esferixis_rgbcolor_new(red, green, blue);
		}

		/**
			* @post Creates a RGB color from C struct
			*/
		inline RGBColor(esferixis_rgbcolor cColor) {
			this->cColor_m = cColor;
		}

		/**
			* @post Gets the red component
			*/
		inline uint8_t red() const {
			return this->cColor_m.red;
		}

		/**
			* @post Gets the green component
			*/
		inline uint8_t green() const {
			return this->cColor_m.green;
		}

		/**
			* @post Gets the blue component
			*/
		inline uint8_t blue() const {
			return this->cColor_m.blue;
		}

		/**
		* @post Gets the C struct version
		*/
		inline esferixis_rgbcolor cColor() const {
			return this->cColor_m;
		}

	private:
		esferixis_rgbcolor cColor_m;
	};
}
#endif