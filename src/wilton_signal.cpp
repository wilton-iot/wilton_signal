/*
 * Copyright 2017, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   wilton_signal.cpp
 * Author: alex
 *
 * Created on September 22, 2017, 8:03 PM
 */

#include "wilton/wilton_signal.h"

#include <atomic>

#include "staticlib/utils.hpp"

#include "wilton/support/alloc.hpp"
#include "wilton/support/exception.hpp"

namespace { // anonymous

std::atomic_bool signal_waiter_registered{false};

} // namespace

// note: sl::utils::initialize_signals() cannot be called directly from another DLL/EXE
char* wilton_signal_initialize() {
    try {
        sl::utils::initialize_signals();
        sl::utils::register_signal_listener([] {
            if (!signal_waiter_registered.load(std::memory_order_acquire)) {
                std::abort();
            }
        });
        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }
}

char* wilton_signal_await() {
    try {
        bool the_false = false;
        bool changed = signal_waiter_registered.compare_exchange_strong(the_false, true,
                std::memory_order_acq_rel, std::memory_order_relaxed);
        if(!changed) {
            throw wilton::support::exception(TRACEMSG(
                    "Signal waiting thread is already registered"));
        }
        sl::utils::wait_for_signal();
        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }
}

char* wilton_signal_fire() {
    try {
        sl::utils::fire_signal();
        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }

}
