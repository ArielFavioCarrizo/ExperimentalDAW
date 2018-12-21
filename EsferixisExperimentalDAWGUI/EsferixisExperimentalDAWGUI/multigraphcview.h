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

#include <esferixis/common/cps/cont.h>

#include "multigraphcelement.h"

namespace esferixis {
	namespace daw {
		namespace gui {
			class MultigraphCView : private boost::noncopyable
			{
				/**
				 * @post Creates a client view
				 */
				MultigraphCView();

				/**
				 * @post Destroys the client view
				 */
				virtual ~MultigraphCView();

				/**
				 * @post Sets the continuation to notify a new element
				 */
				virtual void setOnNewElement(esferixis::cps::Cont cont) =0;

				/**
				 * @post Gets the notified new element
				 */
				virtual esferixis::daw::gui::MultigraphCElement * getNewElement() =0;

				/**
				 * @post Sets the continuation notify a element to delete
				 */
				virtual void setOnElementToDelete(esferixis::cps::Cont cont) =0;

				/**
				 * @post Gets the element to delete
				 */
				virtual esferixis::daw::gui::MultigraphCElement * getElementToDelete() =0;

				/*
				 * @post Sets the continuation for notify no more updates
				 */
				virtual void setOnNoMoreUpdates(esferixis::cps::Cont cont) =0;

				/**
				 * @post Sets the continuation after updating the current element selection
				 */
				virtual void setOnSelectionUpdated(esferixis::cps::Cont cont) =0;

				/**
				 * @post Gets the current selected element
				 */
				virtual esferixis::daw::gui::MultigraphCElement * getSelectedElement() =0;

				/**
				 * @post Do the next update
				 */
				virtual esferixis::cps::Cont doNextUpdate() =0;

				/*
				 * @post Sets the continuation after closing
				 */
				virtual void setOnClosed(esferixis::cps::Cont cont) =0;

				/**
				 * @pre There must be no more updates
				 * @post Closes the view
				 *       It will delete each element and then it will proceed to
				 *		 close the view
				 */
				virtual void close() =0;
			};
		}
	}
}