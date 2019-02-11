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
#include <esferixis/common/graphics/rectf.h>

EsferixisCommon_C_BEGIN

typedef struct _esferixis_daw_gui_modifiableview_stateFeedback {
	void **element;
	void **elementStateFeedback;

	esferixis_cps_cont onElementLoad;
	esferixis_cps_cont onElementUnload;

	esferixis_cps_unsafecont *onUpdated;
} esferixis_daw_gui_modifiableview_stateFeedback;

typedef struct _esferixis_daw_gui_modifiableview_contextEssence {
	void **instance;

	esferixis_rectf viewArea;

	esferixis_daw_gui_modifiableview_stateFeedback stateFeedback;
	esferixis_cps_unsafecont onCreated;
} esferixis_daw_gui_modifiableview_contextEssence;

typedef struct _esferixis_daw_gui_modifiableview_vtable {
	/**
	 * @post Creates an element with the given essence and continuation
	 */
	esferixis_cps_cont(*createElement) (void *implData, void *essence, esferixis_cps_unsafecont cont);

	/**
	 * @post Locks the reference to element to preserve it
	 */
	esferixis_cps_cont(*lockElement) (void *implData, void *element, esferixis_cps_unsafecont cont);

	/**
	 * @post Unlocks the reference to element to allow to delete it
	 */
	esferixis_cps_cont(*unlockElement) (void *implData, void *element, esferixis_cps_unsafecont cont);

	/**
	 * @post Sets an area to view
	 */
	esferixis_cps_cont(*setViewArea) (void *implData, esferixis_rectf viewArea, esferixis_cps_unsafecont cont);

	/**
	 * @post Closes the view
	 *
	 *       It will unload each element and then it will proceed to
	 *		 close the view.
	 *		 The view will be invalidated.
	 */
	esferixis_cps_cont(*close) (void *implData, esferixis_cps_unsafecont cont);
} esferixis_daw_gui_modifiableview_vtable;

typedef struct _esferixis_daw_gui_modifiableview {
	esferixis_daw_gui_modifiableview_vtable *vtable;
	void *implData;
} esferixis_daw_gui_modifiableview;

inline esferixis_cps_cont esferixis_daw_gui_modifiableview_createElement(esferixis_daw_gui_modifiableview *self, void *essence, esferixis_cps_unsafecont cont) {
	return self->vtable->createElement(self->implData, essence, cont);
}

inline esferixis_cps_cont esferixis_daw_gui_modifiableview_lockElement(esferixis_daw_gui_modifiableview *self, void *element, esferixis_cps_unsafecont cont) {
	return self->vtable->lockElement(self->implData, element, cont);
}

inline esferixis_cps_cont esferixis_daw_gui_modifiableview_unlockElement(esferixis_daw_gui_modifiableview *self, void *element, esferixis_cps_unsafecont cont) {
	return self->vtable->unlockElement(self->implData, element, cont);
}

inline esferixis_cps_cont esferixis_daw_gui_modifiableview_setViewArea(esferixis_daw_gui_modifiableview *self, esferixis_rectf viewArea, esferixis_cps_unsafecont cont) {
	return self->vtable->setViewArea(self->implData, viewArea, cont);
}

inline esferixis_cps_cont esferixis_daw_gui_modifiableview_close(esferixis_daw_gui_modifiableview *self, esferixis_cps_unsafecont cont) {
	return self->vtable->close(self->implData, cont);
}

EsferixisCommon_C_END