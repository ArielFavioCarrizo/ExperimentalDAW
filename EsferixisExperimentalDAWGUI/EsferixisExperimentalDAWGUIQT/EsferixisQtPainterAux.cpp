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

#include "EsferixisQtPainterAux.h"
#include "EsferixisQtMisc.h"

#include <math.h>

#define SELFCLASS esferixis::Qt::PainterAux

SELFCLASS::PainterAux(QPainter& painter) : painter_m(painter)
{

}

SELFCLASS::~PainterAux()
{
}

void SELFCLASS::draw(esferixis::daw::gui::Grid grid, QRect area) {
	const float min_x = area.x();
	const float min_y = area.y();
	const float max_x = area.x() + area.width();
	const float max_y = area.y() + area.height();

	const float g_x0 = grid.offset().x();
	const float g_w = grid.size().x();
	const float g_y0 = grid.offset().y();
	const float g_h = grid.size().y();

	const int min_kx = std::ceil( (min_x - g_x0) / g_w );
	const float max_kx = std::floor( (max_x - g_x0) / g_w );

	const float min_ky = std::ceil( (min_y - g_y0) / g_h );
	const float max_ky = std::floor( (max_y - g_y0) / g_h );

	this->painter_m.setPen(esferixis::Qt::toQColor(grid.color()));

	for (int i = min_kx; i <= max_kx; i++) {
		const float x = g_x0 + g_w * (float)i;

		this->painter_m.drawLine(QLine(QPoint(x, min_y), QPoint(x, max_y)));
	}

	for (int i = min_ky; i <= max_ky; i++) {
		const float y = g_y0 + g_h * (float)i;

		this->painter_m.drawLine(QLine(QPoint(min_x, y), QPoint(max_x, y)));
	}
}