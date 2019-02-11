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

#include <esferixis/common/cps/cont.h>
#include <esferixis/common/cps/exception.h>
#include <esferixis/common/graphics/rgbcolor.h>

EsferixisCommon_C_BEGIN

typedef struct _esferixis_daw_gui_viewNoteSegment_stateFeedback {
	esferixis_cps_cont onNewOffset;
	esferixis_cps_cont onNewHeight;
	esferixis_cps_cont onNewColor;
	esferixis_cps_cont onNewSelectionState;
} esferixis_daw_gui_viewNoteSegment_stateFeedback;

typedef struct _esferixis_daw_gui_viewNoteSegment_essence {
	double offset;
	double height;

	bool isSelected;
} esferixis_daw_gui_viewNoteSegment_essence;

typedef struct _esferixis_daw_gui_viewNoteSegment_vtable {
	/**
	 * @post Gets the time offset
	 */
	double (*getTimeOffset) (void *implData);

	/*
	 * @post Gets the height
	 */
	double (*getHeight) (void *implData);

	/**
	 * @post Gets the color
	 */
	esferixis_rgbcolor (*getColor) (void *implData);

	/**
	 * @post Gets a boolean indicating if it is selected
	 */
	bool (*isSelected) (void *implData);

	/**
	 * @post Sets the offset
	 */
	void (*setOffset) (void *implData, double offset, const esferixis_cps_unsafecont *cont, esferixis_cps_cont *nextCont);

	/**
	 * @post Sets the height
	 */
	void (*setHeight) (void *implData, double height, const esferixis_cps_unsafecont *cont, esferixis_cps_cont *nextCont);

	/**
	 * @post Erases the note segment
	 */
	void (*erase) (void *implData, const esferixis_cps_unsafecont *cont, esferixis_cps_cont *nextCont);
} esferixis_daw_gui_viewNoteSegment_vtable;

typedef struct _esferixis_daw_gui_viewNoteSegment {
	const esferixis_daw_gui_viewNoteSegment_vtable *vtable;
	void *implData;
} esferixis_daw_gui_viewNoteSegment;

inline double esferixis_daw_gui_viewNoteSegment_getTimeOffset(esferixis_daw_gui_viewNoteSegment *self) {
	return self->vtable->getTimeOffset(self->implData);
}

inline double esferixis_daw_gui_viewNoteSegment_getHeight(esferixis_daw_gui_viewNoteSegment *self) {
	return self->vtable->getHeight(self->implData);
}

inline esferixis_rgbcolor esferixis_daw_gui_viewNoteSegment_getColor(esferixis_daw_gui_viewNoteSegment *self) {
	return self->vtable->getColor(self->implData);
}

inline bool esferixis_daw_gui_viewNoteSegment_isSelected(esferixis_daw_gui_viewNoteSegment *self) {
	return self->vtable->isSelected(self->implData);
}

inline void esferixis_daw_gui_viewNoteSegment_setOffset(esferixis_daw_gui_viewNoteSegment *self, double offset, esferixis_cps_unsafecont cont, esferixis_cps_cont *nextCont) {
	self->vtable->setOffset(self->implData, offset, &cont, nextCont);
}

inline void esferixis_daw_gui_viewNoteSegment_setHeight(esferixis_daw_gui_viewNoteSegment *self, double height, esferixis_cps_unsafecont cont, esferixis_cps_cont *nextCont) {
	self->vtable->setHeight(self->implData, height, &cont, nextCont);
}

inline void esferixis_daw_gui_viewNoteSegment_setHeight(esferixis_daw_gui_viewNoteSegment *self, esferixis_cps_unsafecont cont, esferixis_cps_cont *nextCont) {
	self->vtable->erase(self->implData, &cont, nextCont);
}
EsferixisCommon_C_END