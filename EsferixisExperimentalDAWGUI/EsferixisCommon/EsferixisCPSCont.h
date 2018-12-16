#pragma once

#include <stdexcept>

namespace esferixis {
	namespace cps {
		class Cont final
		{
		public:
			/**
			 * @post Crea una continuación inválida
			 */
			Cont();

			/**
			 * @post Crea una continuación con el puntero
					 a la función y los datos especificados
			 */
			template<typename T>
			Cont(Cont(*funptr) (T *), T *data) {
				this->funptr_m = reinterpret_cast<Cont(*) (void *)>(funptr);
				this->data_m = reinterpret_cast<void *>(data);
			}

			template<typename T>
			static Cont create(Cont(*funptr) (T *), T *data) {
				return Cont(funptr, data);
			}

			friend void runCPS(Cont firstContinuation);

		private:
			static Cont invalidFun(void *data);

			Cont(*funptr_m) (void *);
			void *data_m;
		};

		// Continuación 'dummy' para salir del loop de CPS
		const Cont CPS_RET = Cont::create<void *>(nullptr, nullptr);

		/**
		 * @post Ejecuta continuaciones hasta que haya un retorno
		 */
		inline void runCPS(Cont firstContinuation) {
			Cont currentCont = firstContinuation;

			while (currentCont.funptr_m != nullptr) {
				currentCont = currentCont.funptr_m(currentCont.data_m);
			}
		}
	}
}
