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

	int isSelected;
} esferixis_daw_gui_viewNoteSegment_essence;

typedef struct _esferixis_daw_gui_viewNoteSegment_vtable {
	/**
	 * @post Gets the user context
	 */
	void * (*getUserContext)(void *implData);

	/**
	 * @post Sets the user context
	 */
	void (*setUserContext)(void *implData, void *userContext);

	/**
	 * @post Gets the state feedback
	 */
	esferixis_daw_gui_viewNoteSegment_stateFeedback * (*getStateFeedback) (void *implData);

	/**
	 * @post Gets the time offset
	 */
	double (*getOffset) (void *implData);

	/*
	 * @post Gets the height
	 */
	double (*getHeight) (void *implData);

	/**
	 * @post Gets the color
	 */
	void (*getColor) (void *implData, esferixis_rgbcolor *color);

	/**
	 * @post Gets a integer indicating if it is selected
	 */
	int (*isSelected) (void *implData);

	/**
	 * @post Sets the offset
	 */
	void (*setOffset) (esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);

	/**
	 * @post Sets the height (double)
	 */
	void (*setHeight) (esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);

	/**
	 * @post Sets if it is selected (int)
	 */
	void (*setIsSelected) (esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);

	/**
	 * @post Erases the note segment
	 */
	void (*erase) (esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);
} esferixis_daw_gui_viewNoteSegment_vtable;

typedef struct _esferixis_daw_gui_viewNoteSegment {
	const esferixis_daw_gui_viewNoteSegment_vtable *vtable;
	void *implData;
} esferixis_daw_gui_viewNoteSegment;

inline void * esferixis_daw_gui_viewNoteSegment_getUserContext(esferixis_daw_gui_viewNoteSegment *self) {
	return self->vtable->getUserContext(self->implData);
}

inline void esferixis_daw_gui_viewNoteSegment_setUserContext(esferixis_daw_gui_viewNoteSegment *self, void *userContext) {
	self->vtable->setUserContext(self->implData, userContext);
}

inline esferixis_daw_gui_viewNoteSegment_stateFeedback * esferixis_daw_gui_viewNoteSegment_getStateFeedback(esferixis_daw_gui_viewNoteSegment *self) {
	return self->vtable->getStateFeedback(self->implData);
}

inline double esferixis_daw_gui_viewNoteSegment_getOffset(esferixis_daw_gui_viewNoteSegment *self) {
	return self->vtable->getOffset(self->implData);
}

inline double esferixis_daw_gui_viewNoteSegment_getHeight(esferixis_daw_gui_viewNoteSegment *self) {
	return self->vtable->getHeight(self->implData);
}

inline esferixis_rgbcolor esferixis_daw_gui_viewNoteSegment_getColor(esferixis_daw_gui_viewNoteSegment *self) {
	esferixis_rgbcolor color;

	self->vtable->getColor(self->implData, &color);

	return color;
}

inline int esferixis_daw_gui_viewNoteSegment_isSelected(esferixis_daw_gui_viewNoteSegment *self) {
	return self->vtable->isSelected(self->implData);
}

inline void esferixis_daw_gui_viewNoteSegment_setOffset(esferixis_daw_gui_viewNoteSegment *self, esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
	esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;

	methodContext->objImplData = self->implData;
	self->vtable->setOffset(context, nextCont);
}

inline void esferixis_daw_gui_viewNoteSegment_setHeight(esferixis_daw_gui_viewNoteSegment *self, esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
	esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;

	methodContext->objImplData = self->implData;
	self->vtable->setHeight(context, nextCont);
}

inline void esferixis_daw_gui_viewNoteSegment_setIsSelected(esferixis_daw_gui_viewNoteSegment *self, esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
	esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;

	methodContext->objImplData = self->implData;
	self->vtable->setIsSelected(context, nextCont);
}

inline void esferixis_daw_gui_viewNoteSegment_erase(esferixis_daw_gui_viewNoteSegment *self, esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
	esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;

	methodContext->objImplData = self->implData;
	self->vtable->erase(context, nextCont);
}
EsferixisCommon_C_END

#ifdef __cplusplus
namespace esferixis {
	namespace daw {
		namespace gui {
			template<typename E, typename EEssence>
			class ViewNoteSegment final {
			public:
				inline ViewNoteSegment(esferixis_daw_gui_viewNoteSegment *cObject) {
					this->cObject = cObject;
				}

				inline void * getUserContext() {
					return esferixis_daw_gui_viewNoteSegment_getUserContext(this->cObject_m);
				}

				inline void setUserContext(void *userContext) {
					esferixis_daw_gui_viewNoteSegment_setUserContext(this->cObject, userContext);
				}

				inline esferixis_daw_gui_viewNoteSegment_stateFeedback * getStateFeedback() {
					return esferixis_daw_gui_viewNoteSegment_getStateFeedback(this->cObject_m);
				}

				inline double getOffset() {
					return esferixis_daw_gui_viewNoteSegment_getOffset(this->cObject);
				}

				inline double getHeight() {
					return esferixis_daw_gui_viewNoteSegment_getHeight(this->cObject);
				}

				inline esferixis_rgbcolor getColor() {
					return esferixis_daw_gui_viewNoteSegment_getColor(this->cObject);
				}

				inline void setOffset(esferixis::cps::MethodProcedureContext<double> *context, esferixis_cps_cont *nextCont) {
					esferixis_daw_gui_viewNoteSegment_setOffset(this->cObject, context->cContext(), nextCont);
				}

				inline void setHeight(esferixis::cps::MethodProcedureContext<double> *context, esferixis_cps_cont *nextCont) {
					esferixis_daw_gui_viewNoteSegment_setHeight(this->cObject, context->cContext(), nextCont);
				}

				inline esferixis_daw_gui_viewNoteSegment *cObject() {
					return this->cObject_m;
				}

			private:
				esferixis_daw_gui_viewNoteSegment *cObject_m;
			};
		}
	}
}
#endif