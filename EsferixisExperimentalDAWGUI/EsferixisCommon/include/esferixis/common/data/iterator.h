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

typedef struct _esferixis_cps_iterator_next_feedback {
	void **element;

	esferixis_cps_cont nextElementCont;
	esferixis_cps_cont endCont;

	esferixis_cps_failurecont failureCont;
} esferixis_cps_iterator_next_feedback;

typedef struct _esferixis_cps_iterator {
	esferixis_cps_cont(*next) (void *implData, esferixis_cps_iterator_next_feedback feedback);

	void *implData;
} esferixis_cps_iterator;

inline esferixis_cps_cont esferixis_cps_iterator_next(esferixis_cps_iterator iterator, esferixis_cps_iterator_next_feedback feedback) {
	return iterator.next(iterator.implData, feedback);
}