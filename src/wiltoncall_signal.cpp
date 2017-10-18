/* 
 * File:   wiltoncall_signal.cpp
 * Author: alex
 *
 * Created on September 22, 2017, 8:03 PM
 */

#include "wilton/wilton_signal.h"

#include "wilton/support/buffer.hpp"
#include "wilton/support/exception.hpp"
#include "wilton/support/registrar.hpp"

namespace wilton {
namespace signal {

support::buffer await(sl::io::span<const char>) {
    // call wilton
    char* err = wilton_signal_await();
    if (nullptr != err) {
        support::throw_wilton_error(err, TRACEMSG(err));
    }
    return support::make_empty_buffer();
}

support::buffer fire(sl::io::span<const char>) {
    // call wilton
    char* err = wilton_signal_fire();
    if (nullptr != err) {
        support::throw_wilton_error(err, TRACEMSG(err));
    }
    return support::make_empty_buffer();
}

} // namespace
}

extern "C" char* wilton_module_init() {
    try {
        wilton::support::register_wiltoncall("signal_await", wilton::signal::await);
        wilton::support::register_wiltoncall("signal_fire", wilton::signal::fire);
        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }
}
