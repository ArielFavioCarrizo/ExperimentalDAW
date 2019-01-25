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

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <esferixis/common/contextualized.h>

#include <esferixis/common/cps/cont.h>
#include <esferixis/common/cps/exception.h>

namespace esferixis {
	namespace daw {
		namespace gui {
			template<typename E, typename EEssence, typename EStateFeedback>
			class MultigraphCView : public esferixis::Contextualized, private boost::noncopyable
			{
			public:
				struct StateFeedback {
					E **element;
					EStateFeedback **elementStateFeedback;

					esferixis_cps_cont onElementToLoad;
					esferixis_cps_cont onLoadedElement;

					esferixis_cps_cont onElementUnload;

					esferixis_cps_unsafecont *onUpdated;
				};

				struct ContextEssence {
					esferixis::daw::gui::MultigraphCView<E, EEssence> **instance;

					StateFeedback stateFeedback;
					esferixis_cps_unsafecont onCreated;
				};

				/**
				 * @post Creates an element with the given essence and continuation
				 */
				virtual esferixis_cps_cont createElement(EEssence elementEssence, esferixis_cps_unsafecont cont) =0;

				/**
				 * @post Locks the reference to element to preserve it
				 */
				virtual esferixis_cps_cont lockElement(E *element, esferixis_cps_cont cont) = 0;

				/**
				 * @post Unlocks the reference to element to allow to delete it
				 */
				virtual esferixis_cps_cont unlockElement(E *element, esferixis_cps_cont cont) = 0;

				/**
				 * @post Sets a time interval to view
				 */
				virtual esferixis_cps_cont setTimeIntervalToView(double min, double max, esferixis_cps_unsafecont cont) =0;

				/**
				 * @post Closes the view
				 *		 
				 *       It will unload each element and then it will proceed to
				 *		 close the view.
				 *		 The view will be invalidated.
				 */
				virtual esferixis_cps_cont close(esferixis_cps_unsafecont cont) =0;

			protected:
				/**
				 * @post Creates a client view
				 */
				MultigraphCView() {

				}

				/**
				 * @post Destroys the client view
				 */
				virtual ~MultigraphCView() {

				}
			};
		}
	}
}