/* Copyright (c) 2006-2020 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CORE_UTIL_SHAREDPOINTER_H__
#define __CORE_UTIL_SHAREDPOINTER_H__

#include <stdlib.h>

#include <stdint.h>
#include <stddef.h>

namespace mbed {
namespace util {

/** Shared pointer class.
  *
  * Similar to std::shared_ptr in C++11.
  *
  * Usage: SharedPointer<class> POINTER(new class())
  *
  * When POINTER is passed around by value the copy constructor and
  * destructor counts the number of references to the original object.
  * If the counter reaches zero, delete is called on the object pointed to.
  *
  * To avoid loops, "weak" references should be used by calling the original
  * pointer directly through POINTER.get().
  */

template <class T>
class SharedPointer {
public:
    /**
     * @brief Create empty SharedPointer not pointing to anything.
     * @details Used for variable declaration.
     */
    SharedPointer(): pointer(NULL), counter(NULL) {
    }

    /**
     * @brief Create new SharedPointer
     * @param _pointer Pointer to take control over
     */
    SharedPointer(T* _pointer): pointer(_pointer) {
        // allocate counter on the heap so it can be shared
        counter = (uint32_t*) malloc(sizeof(uint32_t));
        *counter = 1;
    }

    /**
     * @brief Destructor.
     * @details Decrement reference counter and delete object if no longer pointed to.
     */
    ~SharedPointer() {
        decrementCounter();
    }

    /**
     * @brief Copy constructor.
     * @details Create new SharePointer from other SharedPointer by
     *          copying pointer to original object and pointer to counter.
     * @param source Object being copied from.
     */
    SharedPointer(const SharedPointer& source): pointer(source.pointer), counter(source.counter) {
        // increment reference counter
        if (counter) {
            (*counter)++;
        }
    }

    /**
     * @brief Assignment operator.
     * @details Cleanup previous reference and assign new pointer and counter.
     * @param source Object being assigned from.
     * @return Object being assigned.
     */
    SharedPointer operator=(const SharedPointer& source) {
        if (this != &source) {
            // clean up by decrementing counter
            decrementCounter();

            // assign new values
            pointer = source.get();
            counter = source.getCounter();

            // increment new counter
            if (counter) {
                (*counter)++;
            }
        }

        return *this;
    }

    /**
     * @brief Raw pointer accessor.
     * @details Get raw pointer to object pointed to.
     * @return Pointer.
     */
    T* get() const {
        return pointer;
    }

    /**
     * @brief Reference count accessor.
     * @return Reference count.
     */
    uint32_t use_count() const {
        if (counter) {
            return *counter;
        } else {
            return 0;
        }
    }

    /**
     * @brief Dereference object operator.
     * @details Override to return the object pointed to.
     */
    T& operator*() const {
        return *pointer;
    }

    /**
     * @brief Dereference object member operator.
     * @details Override to return return member in object pointed to.
     */
    T* operator->() const {
        return pointer;
    }

    /**
     * @brief Boolean conversion operator.
     * @return Whether or not the pointer is NULL.
     */
    operator bool() const {
        return (pointer != 0);
    }

private:
    /**
     * @brief Get pointer to reference counter.
     * @return Pointer to reference counter.
     */
    uint32_t* getCounter() const {
        return counter;
    }

    /**
     * @brief Decrement reference counter.
     * @details If count reaches zero, free counter and delete object pointed to.
     */
    void decrementCounter() {
        if (counter) {
            if (*counter == 1) {
                free(counter);
                delete pointer;
            } else {
                (*counter)--;
            }
        }
    }

private:
    // pointer to shared object
    T* pointer;

    // pointer to shared reference counter
    uint32_t* counter;
};

/** Non-member relational operators.
  */
template <class T, class U>
bool operator== (const SharedPointer<T>& lhs, const SharedPointer<U>& rhs) {
    return (lhs.get() == rhs.get());
}

template <class T, typename U>
bool operator== (const SharedPointer<T>& lhs, U rhs) {
    return (lhs.get() == (T*) rhs);
}

template <class T, typename U>
bool operator== (U lhs, const SharedPointer<T>& rhs) {
    return ((T*) lhs == rhs.get());
}

/** Non-member relational operators.
  */
template <class T, class U>
bool operator!= (const SharedPointer<T>& lhs, const SharedPointer<U>& rhs) {
    return (lhs.get() != rhs.get());
}

template <class T, typename U>
bool operator!= (const SharedPointer<T>& lhs, U rhs) {
    return (lhs.get() != (T*) rhs);
}

template <class T, typename U>
bool operator!= (U lhs, const SharedPointer<T>& rhs) {
    return ((T*) lhs != rhs.get());
}

} // namespace util
} // namespace mbed

#endif // __CORE_UTIL_SHAREDPOINTER_H__
