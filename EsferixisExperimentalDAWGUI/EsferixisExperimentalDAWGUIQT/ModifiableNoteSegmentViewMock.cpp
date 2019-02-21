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

#include "ModifiableNoteSegmentViewMock.h"

#include <esferixis/common/cps/object.h>
#include <esferixis/daw/gui/common/modifiableview.h>
#include <esferixis/daw/gui/common/viewnotesegment.h>
#include <esferixis/common/data/linkedlist.h>

namespace impl {
	namespace NoteSegment {
		class ImplData;
	}

	namespace View {
		struct ImplData {
			esferixis_daw_gui_modifiableview view;
			esferixis_daw_gui_modifiableview_stateFeedback stateFeedback;
			esferixis_rectf viewArea;
			esferixis::LinkedList<impl::NoteSegment::ImplData *> noteSegments;
		};

		void createElement(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);
		void lockElement(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);
		void unlockElement(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);
		void getViewArea(void *implData, esferixis_rectf *viewArea);
		void setViewArea(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);
		void close(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);

		constexpr esferixis_daw_gui_modifiableview_vtable vTable = {
			createElement,
			lockElement,
			unlockElement,
			getViewArea,
			setViewArea,
			close
		};
	}

	namespace NoteSegment {
		struct ImplData {
			esferixis_daw_gui_viewNoteSegment noteSegment;
			esferixis_daw_gui_viewNoteSegment_stateFeedback stateFeedback;

			impl::View::ImplData *viewData;
			esferixis::LinkedList<NoteSegment::ImplData *>::Node node;

			unsigned int id;
			void *userContext;
			double offset;
			double height;
			int isSelected;
		};

		void * getUserContext(void *implData);
		void setUserContext(void *implData, void *userContext);
		esferixis_daw_gui_viewNoteSegment_stateFeedback * getStateFeedback(void *implData);
		double getOffset(void *implData);
		double getHeight(void *implData);
		void getColor(void *implData, esferixis_rgbcolor *color);
		int isSelected(void *implData);
		void setOffset(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);
		void setHeight(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);
		void setIsSelected(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);
		void erase(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont);

		constexpr esferixis_daw_gui_viewNoteSegment_vtable vTable = {
			getUserContext,
			setUserContext,
			getStateFeedback,
			getOffset,
			getHeight,
			getColor,
			isSelected,
			setOffset,
			setHeight,
			setIsSelected,
			erase
		};
	}

	namespace View {
		void createElement(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
			struct STM {
				static void onSuccess(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
					esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;
					impl::View::ImplData *self_impl = (impl::View::ImplData *) methodContext->objImplData;

					esferixis_daw_gui_viewNoteSegment *noteSegment = (esferixis_daw_gui_viewNoteSegment *)self_impl->stateFeedback.element;
					impl::NoteSegment::ImplData *noteSegmentData = (impl::NoteSegment::ImplData *) noteSegment->implData;

					self_impl->noteSegments.addLast(&noteSegmentData->node);
				}

				static void onFailure(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
					*(context->cont.exception) = esferixis::cps::createException("Cannot create note segment because its loading has been failed -> " + esferixis::cps::destructiveExceptMsgCopy(*(context->cont.exception)));

					esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;
					impl::View::ImplData *self_impl = (impl::View::ImplData *) methodContext->objImplData;

					esferixis_daw_gui_viewNoteSegment *noteSegment = (esferixis_daw_gui_viewNoteSegment *)self_impl->stateFeedback.element;
					impl::NoteSegment::ImplData *noteSegmentData = (impl::NoteSegment::ImplData *) noteSegment->implData;

					delete noteSegmentData;

					*nextCont = context->cont.onFailure;
				}
			};

			esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;

			impl::View::ImplData *self_impl = (impl::View::ImplData *) methodContext->objImplData;
			esferixis_daw_gui_viewNoteSegment_essence *elementEssence = (esferixis_daw_gui_viewNoteSegment_essence *)methodContext->param;

			impl::NoteSegment::ImplData *elementData = new impl::NoteSegment::ImplData();

			elementData->viewData = self_impl;
			elementData->noteSegment.vtable = &impl::NoteSegment::vTable;
			elementData->noteSegment.implData = (void *)elementData;
			elementData->node.set(elementData);

			elementData->offset = elementEssence->offset;
			elementData->height = elementEssence->height;
			elementData->isSelected = elementEssence->isSelected;

			*(self_impl->stateFeedback.element) = &(elementData->noteSegment);

			self_impl->stateFeedback.onUpdated->exception = context->cont.exception;
			self_impl->stateFeedback.onUpdated->onSuccess = esferixis::cps::mkCont(STM::onSuccess, context);
			self_impl->stateFeedback.onUpdated->onFailure = esferixis::cps::mkCont(STM::onFailure, context);

			*nextCont = self_impl->stateFeedback.onElementLoad;
		}

		void lockElement(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
			*nextCont = context->cont.onSuccess;
		}

		void unlockElement(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
			*nextCont = context->cont.onSuccess;
		}

		void getViewArea(void *implData, esferixis_rectf *viewArea) {
			*viewArea = ((impl::View::ImplData *) implData)->viewArea;
		}

		void setViewArea(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
			esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;
			impl::View::ImplData *self_impl = (impl::View::ImplData *) methodContext->objImplData;

			self_impl->viewArea = *((esferixis_rectf *)methodContext->param);

			*nextCont = context->cont.onSuccess;
		}

		void close(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
			struct STM {
				static void tryToUnloadLast(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
					esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;
					impl::View::ImplData *self_impl = (impl::View::ImplData *) methodContext->objImplData;

					if (!self_impl->noteSegments.isEmpty()) {
						impl::NoteSegment::ImplData *noteSegmentData = self_impl->noteSegments.last()->get();

						*(self_impl->stateFeedback.element) = &(noteSegmentData->noteSegment);

						esferixis_cps_unsafecont cont;
						cont.exception = context->cont.exception;
						cont.onSuccess = esferixis::cps::mkCont(unloadSuccess, context);
						cont.onFailure = esferixis::cps::mkCont(unloadFailure, context);

						*(self_impl->stateFeedback.onUpdated) = cont;

						*nextCont = self_impl->stateFeedback.onElementUnload;
					}
					else {
						delete self_impl;

						*nextCont = context->cont.onSuccess;
					}
				}

				static void unloadSuccess(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
					esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;
					impl::View::ImplData *self_impl = (impl::View::ImplData *) methodContext->objImplData;
					impl::NoteSegment::ImplData *noteSegmentData = self_impl->noteSegments.last()->get();

					delete noteSegmentData;

					*nextCont = esferixis::cps::mkCont(tryToUnloadLast, context);
				}

				static void unloadFailure(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
					esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;
					impl::View::ImplData *self_impl = (impl::View::ImplData *) methodContext->objImplData;
					impl::NoteSegment::ImplData *noteSegmentData = self_impl->noteSegments.last()->get();

					*(context->cont.exception) = esferixis::cps::createException("Cannot unload note segment (id " + std::to_string(noteSegmentData->id) + ") -> " + esferixis::cps::destructiveExceptMsgCopy(*(context->cont.exception)));

					*nextCont = context->cont.onFailure;
				}
			};

			*nextCont = esferixis::cps::mkCont(STM::tryToUnloadLast, context);
		}
	}

	namespace NoteSegment {
		void * getUserContext(void *implData) {
			return ((NoteSegment::ImplData *) implData)->userContext;
		}

		void setUserContext(void *implData, void *userContext) {
			((NoteSegment::ImplData *) implData)->userContext = userContext;
		}

		esferixis_daw_gui_viewNoteSegment_stateFeedback * getStateFeedback(void *implData) {
			return &( ((NoteSegment::ImplData *) implData)->stateFeedback );
		}

		double getOffset(void *implData) {
			return ((NoteSegment::ImplData *)implData)->offset;
		}

		double getHeight(void *implData) {
			return ((NoteSegment::ImplData *)implData)->height;
		}

		void getColor(void *implData, esferixis_rgbcolor *color) {
			*color = esferixis_rgbcolor_new(0, 255, 0);
		}

		int isSelected(void *implData) {
			return ((NoteSegment::ImplData *)implData)->isSelected;
		}

		void setOffset(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
			esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;
			NoteSegment::ImplData *self_data = (NoteSegment::ImplData *) methodContext->objImplData;

			(*self_data->viewData->stateFeedback.element) = &self_data->noteSegment;
			(*self_data->viewData->stateFeedback.onUpdated) = context->cont;

			self_data->offset = *((double *)methodContext->param);

			*nextCont = self_data->stateFeedback.onNewOffset;
		}

		void setHeight(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
			esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;
			NoteSegment::ImplData *self_data = (NoteSegment::ImplData *) methodContext->objImplData;

			(*self_data->viewData->stateFeedback.element) = &self_data->noteSegment;
			(*self_data->viewData->stateFeedback.onUpdated) = context->cont;

			self_data->height = *((double *)methodContext->param);

			*nextCont = self_data->stateFeedback.onNewHeight;
		}

		void setIsSelected(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
			esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;
			NoteSegment::ImplData *self_data = (NoteSegment::ImplData *) methodContext->objImplData;

			(*self_data->viewData->stateFeedback.element) = &self_data->noteSegment;
			(*self_data->viewData->stateFeedback.onUpdated) = context->cont;

			self_data->isSelected = *((int *)methodContext->param);

			*nextCont = self_data->stateFeedback.onNewSelectionState;
		}

		void erase(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
			struct STM {
				static void afterNotification_success(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
					*nextCont = context->cont.onSuccess;

					esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;
					NoteSegment::ImplData *self_data = (NoteSegment::ImplData *) methodContext->objImplData;

					delete self_data;
				}

				static void afterNotification_failure(esferixis_cps_procedureContext *context, esferixis_cps_cont *nextCont) {
					esferixis_cps_exception causeException = *(context->cont.exception);

					*(context->cont.exception) = esferixis::cps::createException("Cannot erase note segment -> " + esferixis::cps::destructiveExceptMsgCopy(causeException));

					*nextCont = context->cont.onFailure;
				}
			};

			esferixis_cps_methodContext *methodContext = (esferixis_cps_methodContext *)context->param;
			NoteSegment::ImplData *self_data = (NoteSegment::ImplData *) methodContext->objImplData;

			(*self_data->viewData->stateFeedback.element) = &self_data->noteSegment;

			self_data->viewData->stateFeedback.onUpdated->exception = context->cont.exception;
			self_data->viewData->stateFeedback.onUpdated->onSuccess = esferixis::cps::mkCont(STM::afterNotification_success, context);
			self_data->viewData->stateFeedback.onUpdated->onFailure = esferixis::cps::mkCont(STM::afterNotification_failure, context);

			*nextCont = self_data->stateFeedback.onNewSelectionState;
		}
	}
}

void esferixis_daw_gui_test_createNoteSegmentViewMock(esferixis_daw_gui_modifiableview_contextEssence *context, esferixis_cps_cont *nextCont) {
	impl::View::ImplData *selfImpl = new impl::View::ImplData();

	selfImpl->view.implData = (void *) selfImpl;
	selfImpl->view.vtable = &impl::View::vTable;
	selfImpl->stateFeedback = context->stateFeedback;
	selfImpl->viewArea = context->viewArea;

	*(context->instance) = &(selfImpl->view);

	*nextCont = context->onCreated.onSuccess;
}