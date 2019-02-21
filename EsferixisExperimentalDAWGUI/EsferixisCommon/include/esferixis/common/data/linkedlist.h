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
EsferixisCommon_C_BEGIN

typedef struct _esferixis_linkedlist_node {
	void *data;

	_esferixis_linkedlist_node *previous;
	_esferixis_linkedlist_node *next;
} esferixis_linkedlist_node;

EsferixisCommon_C_END

#ifdef __cplusplus

#include <boost/noncopyable.hpp>
#include <stdexcept>

namespace esferixis {
	template<typename T>
	class LinkedList final : private boost::noncopyable {
	public:
		class Node final : private boost::noncopyable {
			friend class esferixis::LinkedList<T>;

		public:
			/**
			 * @post Creates a node
			 */
			inline Node() {
				this->previousNode_m = nullptr;
				this->nextNode_m = nullptr;
				this->list_m = nullptr;
			}

			/**
			 * @post Creates a node with the specified value
			 */
			inline Node(T value) {
				this->previousNode_m = nullptr;
				this->nextNode_m = nullptr;
				this->list_m = nullptr;
				this->value_m = value;
			}

			/**
			 * @post Sets the node's value
			 */
			inline void set(T value) {
				this->value_m = value;
			}

			/**
			 * @post Gets the node's value
			 */
			inline T get() const {
				return this->value_m;
			}

		private:
			esferixis::LinkedList<T>::Node *previousNode_m;
			esferixis::LinkedList<T>::Node *nextNode_m;

			esferixis::LinkedList<T> *list_m;

			T value_m;
		};

		/**
		 * @post Creates a linked list
		 */
		inline LinkedList() {
			this->firstNode_m = nullptr;
			this->lastNode_m = nullptr;
		}

		/**
		 * @pre The given node cannot already been present in a list
		 * @post Adds the given node at the last position
		 */
		inline void addLast(esferixis::LinkedList<T>::Node *node) {
			if ( node->list_m == nullptr ) {
				if ( this->firstNode_m == nullptr) {
					this->firstNode_m = node;
				}
				
				node->previousNode_m = this->lastNode_m;
				node->nextNode_m = nullptr;
				node->list_m = this;

				if (this->lastNode_m != nullptr) {
					this->lastNode_m->nextNode_m = node;
				}

				this->lastNode_m = node;
			}
		}

		/**
		 * @pre The given node must be present in the list
		 * @post Removes the given node
		 */
		inline void remove(esferixis::LinkedList<T>::Node *node) {
			if (node->list_m == this) {
				auto previousNode = node->previousNode_m;
				auto nextNode = node->nextNode_m;

				if (this->lastNode_m == node) {
					this->lastNode_m = previousNode;
				}
				else {
					nextNode->previousNode_m = previousNode;
				}

				if (this->firstNode_m == node) {
					this->firstNode_m = nullptr;
				}
				else {
					previousNode->nextNode_m = nextNode;
				}

				node->list_m = nullptr;
			}
			else {
				throw std::runtime_error("The given node is already been on a list");
			}
		}

		/**
		 * @post Gets the first node
		 */
		inline esferixis::LinkedList<T>::Node * first() const {
			return this->firstNode_m;
		}

		/**
		 * @post Gets the last node
		 */
		inline esferixis::LinkedList<T>::Node * last() const {
			return this->lastNode_m;
		}

		/**
		 * @post Gets the next node of the given node
		 */
		inline esferixis::LinkedList<T>::Node * next(esferixis::LinkedList<T>::Node *node) const {
			if (node->list_m == this) {
				return node->nextNode_m;
			}
			else {
				throw std::runtime_error("List mismatch");
			}
		}

		/**
		 * @post Gets the previous node of the given node
		 */
		inline esferixis::LinkedList<T>::Node * previous(esferixis::LinkedList<T>::Node *node) const {
			if (node->list_m == this) {
				return node->previousNode_m;
			}
			else {
				throw std::runtime_error("List mismatch");
			}
		}


		/**
		 * @post Gets a boolean indicating if it is empty
		 */
		bool isEmpty() const {
			return (this->firstNode_m == nullptr);
		}

	private:
		esferixis::LinkedList<T>::Node *firstNode_m;
		esferixis::LinkedList<T>::Node *lastNode_m;
	};
}

#endif