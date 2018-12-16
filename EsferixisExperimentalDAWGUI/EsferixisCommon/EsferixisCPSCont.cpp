#include "stdafx.h"
#include "EsferixisCPSCont.h"

esferixis::cps::Cont::Cont() {
	this->funptr_m = esferixis::cps::Cont::invalidFun;
}

esferixis::cps::Cont esferixis::cps::Cont::invalidFun(void *data) {
	throw std::runtime_error("Invalid continuation");
}
