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

EsferixisCommon_C_BEGIN

typedef struct _esferixis_vec2f {
	float x;
	float y;
} esferixis_vec2f;

inline esferixis_vec2f esferixis_vec2f_new(float x, float y) {
	esferixis_vec2f result;

	result.x = x;
	result.y = y;

	return result;
}

inline esferixis_vec2f esferixis_vec2f_add(esferixis_vec2f a, esferixis_vec2f b) {
	esferixis_vec2f result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;

	return result;
}

inline esferixis_vec2f esferixis_vec2f_scale(esferixis_vec2f a, float scalar) {
	esferixis_vec2f result;

	result.x = a.x * scalar;
	result.y = a.y * scalar;

	return result;
}

EsferixisCommon_C_END

#ifdef __cplusplus
namespace esferixis {
	namespace math {
		class Vec2f {
		public:
			/**
			 * @post Creates a zero vector
			 */
			inline Vec2f() {
				this->c_vec_m = esferixis_vec2f_new(0.0f, 0.0f);
			}

			/**
			 * @post Creates a C++ vector from C vector
			 */
			inline Vec2f(esferixis_vec2f c_vec) {
				this->c_vec_m = c_vec;
			}

			/**
			 * @post Creates a vector with the given components
			 */
			inline Vec2f(float x, float y) {
				this->c_vec_m = esferixis_vec2f_new(x, y);
			}

			/**
			 * @post Returns the 'x' component
			 */
			float x() {
				return this->c_vec_m.x;
			}

			/**
			 * @post Returns the 'y' component
			 */
			float y() {
				return this->c_vec_m.y;
			}

			/**
			 * @post Returns the C vector 
			 */
			inline esferixis_vec2f cVec2f() const {
				return this->c_vec_m;
			}

			esferixis::math::Vec2f operator+(const esferixis::math::Vec2f& other) const {
				return esferixis::math::Vec2f( esferixis_vec2f_add(this->c_vec_m, other.c_vec_m) );
			}

		private:
			esferixis_vec2f c_vec_m;
		};
	}
}
#endif