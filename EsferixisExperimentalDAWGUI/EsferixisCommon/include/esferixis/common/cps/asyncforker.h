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

#include <esferixis/common/common.h>
#include <esferixis/common/cps/cont.h>

EsferixisCommon_C_BEGIN

typedef struct _esferixis_cps_asyncforker esferixis_cps_asyncforker;

typedef struct _esferixis_cps_asyncforker_cfg {
	esferixis_cps_cont onFork1;
	esferixis_cps_cont onFork2;
	esferixis_cps_cont onJoin;
} esferixis_cps_asyncforker_cfg;

/**
	* @post Creates an async forker
	*/
EsferixisCommon_C_API esferixis_cps_asyncforker * esferixis_cps_asyncforker_new();

/**
	* @post Destroys the given async forker
	*/
EsferixisCommon_C_API void esferixis_cps_asyncforker_delete(esferixis_cps_asyncforker *asyncForker);

/**
	* @post Returns a pointer to the given async forker's config
	*/
EsferixisCommon_C_API esferixis_cps_asyncforker_cfg * esferixis_cps_asyncforker_config(esferixis_cps_asyncforker *asyncForker);

/**
	* @post Forks the green thread with the given forker
	*/
EsferixisCommon_C_API esferixis_cps_cont esferixis_cps_asyncforker_fork(esferixis_cps_asyncforker *asyncForker);

/**
	* @post Joins the green thread with the given forker
	*/
EsferixisCommon_C_API esferixis_cps_cont esferixis_cps_asyncforker_join(esferixis_cps_asyncforker *asyncForker);

EsferixisCommon_C_END