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

namespace esferixis {
	namespace daw {
		namespace gui {
			template<typename E>
			class MultigraphCView : public esferixis::Contextualized, private boost::noncopyable
			{
			public:
				/**
				 * @post Gets the referenced element on the current state
				 */
				virtual E * getReferencedElement() = 0;

				/**
				 * @post Sets the continuation to notify a new loaded element
				 */
				virtual void setOnElementLoad(esferixis::cps::Cont cont) =0;

				/**
				 * @post Sets the continuation to notify a element that will be unloaded
				 *      
				 *		 Warning: The element will be invalidated by the next action
				 */
				virtual void setOnElementToUnload(esferixis::cps::Cont cont) =0;

				/**
				 * @post Locks the reference to element to preserve it
				 */
				virtual esferixis::cps::Cont lockElement(E *element, esferixis::cps::Cont cont) = 0;

				/**
				 * @post Unlocks the reference to element to allow to delete it
				 */
				virtual esferixis::cps::Cont unlockElement(E *element, esferixis::cps::Cont cont) = 0;

				/*
				 * @post Sets the continuation after the view is closed
				 *		 
				 *		 Warning: The view will be invalidated by the next action
				 */
				virtual void setOnClosed(esferixis::cps::Cont cont) =0;

				/**
				 * @post Sets a time interval to view
				 */
				virtual esferixis::cps::Cont setTimeIntervalToView(double min, double max, esferixis::cps::Cont cont) =0;

				/**
				 * @post Closes the view
				 *		 
				 *       It will unload each element and then it will proceed to
				 *		 close the view
				 */
				virtual void close() =0;

				/**
				 * @post Do the next action
				 */
				virtual esferixis::cps::Cont doNextAction() =0;

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