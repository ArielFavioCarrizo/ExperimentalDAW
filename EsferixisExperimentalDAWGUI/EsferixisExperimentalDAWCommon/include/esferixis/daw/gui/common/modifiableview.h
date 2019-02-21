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

#include <esferixis/common/cps/cont.h>
#include <esferixis/common/cps/exception.h>
#include <esferixis/common/cps/procedure.h>
#include <esferixis/common/cps/object.h>

#include <esferixis/common/graphics/rectf.h>

EsferixisCommon_C_BEGIN

typedef struct _esferixis_daw_gui_modifiableview_stateFeedback {
	void **element;

	esferixis_cps_cont onElementLoad;
	esferixis_cps_cont onElementUnload;
	esferixis_cps_cont onNewElementsBoundingBox;

	esferixis_cps_unsafecont *onUpdated;
} esferixis_daw_gui_modifiableview_stateFeedback;

typedef struct _esferixis_daw_gui_modifiableview_vtable {
	/**
	 * @post Creates an element with the given element essence
	 */
	void(*createElement) (esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);

	/**
	 * @post Locks the reference to element to preserve it
	 */
	void(*lockElement) (esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);

	/**
	 * @post Unlocks the reference to element to allow to delete it
	 */
	void(*unlockElement) (esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);

	/**
	 * @post Gets the view area
	 */
	void(*getViewArea) (void *implData, esferixis_rectf *viewArea);

	/**
	 * @post Sets an area to view (esferixis_rectf)
	 */
	void(*setViewArea) (esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);

	/**
	 * @post Gets the bounding box of elements
	 */
	void(*getElementsBoundingBox)(void *implData, esferixis_rectf *rect);

	/**
	 * @post Closes the view
	 *
	 *       It will unload each element and then it will proceed to
	 *		 close the view.
	 *		 The view will be invalidated.

	 *		 The parameters will be ignored.
	 */
	void(*close) (esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);
} esferixis_daw_gui_modifiableview_vtable;

typedef struct _esferixis_daw_gui_modifiableview {
	const esferixis_daw_gui_modifiableview_vtable *vtable;
	void *implData;
} esferixis_daw_gui_modifiableview;

typedef struct _esferixis_daw_gui_modifiableview_contextEssence {
	esferixis_daw_gui_modifiableview **instance;

	esferixis_rectf viewArea;
	esferixis_daw_gui_modifiableview_stateFeedback stateFeedback;
	esferixis_cps_unsafecont onCreated;
} esferixis_daw_gui_modifiableview_contextEssence;

inline void esferixis_daw_gui_modifiableview_createElement(esferixis_daw_gui_modifiableview *self, esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
	esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *) context->param;

	methodContext->objImplData = self->implData;
	self->vtable->createElement(context, nextCont);
}

inline void esferixis_daw_gui_modifiableview_lockElement(esferixis_daw_gui_modifiableview *self, esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
	esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;

	methodContext->objImplData = self->implData;
	self->vtable->lockElement(context, nextCont);
}

inline void esferixis_daw_gui_modifiableview_unlockElement(esferixis_daw_gui_modifiableview *self, esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
	esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;

	methodContext->objImplData = self->implData;
	self->vtable->unlockElement(context, nextCont);
}

inline esferixis_rectf esferixis_daw_gui_modifiableview_getViewArea(esferixis_daw_gui_modifiableview *self) {
	esferixis_rectf viewArea;

	self->vtable->getViewArea(self->implData, &viewArea);

	return viewArea;
}

inline void esferixis_daw_gui_modifiableview_setViewArea(esferixis_daw_gui_modifiableview *self, esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
	esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;

	methodContext->objImplData = self->implData;
	self->vtable->setViewArea(context, nextCont);
}

inline esferixis_rectf esferixis_daw_gui_modifiableview_getElementsBoundingBox(esferixis_daw_gui_modifiableview *self) {
	esferixis_rectf rect;

	self->vtable->getElementsBoundingBox(self->implData, &rect);

	return rect;
}

inline void esferixis_daw_gui_modifiableview_close(esferixis_daw_gui_modifiableview *self, esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
	esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;

	methodContext->objImplData = self->implData;

	self->vtable->close(context, nextCont);
}

EsferixisCommon_C_END

#ifdef __cplusplus
namespace esferixis {
	namespace daw {
		namespace gui {
			template<typename E, typename EEssence>
			class ModifiableView final {
			public:
				inline ModifiableView(esferixis_daw_gui_modifiableview *view) {
					this->cObject_m = view;
				}

				inline void createElement(esferixis::cps::MethodProcedureContext<EEssence> *context, esferixis_cps_cont *nextCont) {
					esferixis_daw_gui_modifiableview_createElement(this->cObject_m, context->cContext(), nextCont);
				}

				inline void lockElement(esferixis::cps::MethodProcedureContext<E> *context, esferixis_cps_cont *nextCont) {
					esferixis_daw_gui_modifiableview_lockElement(this->cObject_m, context->cContext(), nextCont);
				}

				inline void unlockElement(esferixis::cps::MethodProcedureContext<E> *context, esferixis_cps_cont *nextCont) {
					esferixis_daw_gui_modifiableview_unlockElement(this->cObject_m, context->cContext(), nextCont);
				}

				inline esferixis_rectf getViewArea() {
					return esferixis_daw_gui_modifiableview_getViewArea(this->cObject_m);
				}

				inline void setViewArea(esferixis::cps::MethodProcedureContext<esferixis_rectf> *context, esferixis_cps_cont *nextCont) {
					esferixis_daw_gui_modifiableview_setViewArea(this->cObject_m, context->cContext(), nextCont);
				}

				inline esferixis_rectf getElementsBoundingBox() {
					return esferixis_daw_gui_modifiableview_getElementsBoundingBox(this->cObject_m);
				}

				inline void close(esferixis::cps::MethodProcedureContext<void> *context, esferixis_cps_cont *nextCont) {
					esferixis_daw_gui_modifiableview_close(this->cObject_m, context->cContext(), nextCont);
				}

				inline esferixis_daw_gui_modifiableview * cObject() {
					return this->cObject_m;
				}

			private:
				esferixis_daw_gui_modifiableview *cObject_m;
			};
		}
	}
}
#endif