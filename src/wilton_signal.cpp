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

std::atomic_flag signal_waiter_registered = ATOMIC_FLAG_INIT;

// called from wilton_signal_initialize
sl::utils::signal_ctx& static_ctx() {
    static sl::utils::signal_ctx ctx;
    return ctx;
}

} // namespace

// note: sl::utils::initialize_signals() cannot be called directly from another DLL/EXE
char* wilton_signal_initialize() {
    try {
        sl::utils::initialize_signals(static_ctx());
        sl::utils::register_signal_listener(static_ctx(), [] {
            if (!signal_waiter_registered.test_and_set(std::memory_order_acq_rel)) {
#ifdef STATICLIB_WINDOWS
                std::abort();
#else // !STATICLIB_WINDOWS
                ::_Exit(130);
#endif // STATICLIB_WINDOWS
            }
        });
        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }
}

char* wilton_signal_await() {
    try {
        if(signal_waiter_registered.test_and_set(std::memory_order_acq_rel)) {
            throw wilton::support::exception(TRACEMSG(
                    "Signal waiting thread is already registered"));
        }
        sl::utils::wait_for_signal(static_ctx());
        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }
}

char* wilton_signal_fire() {
    try {
        sl::utils::fire_signal(static_ctx());
        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }

}
