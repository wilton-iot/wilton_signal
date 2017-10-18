/* 
 * File:   wilton_process.h
 * Author: alex
 *
 * Created on October 17, 2017, 8:57 PM
 */

#ifndef WILTON_SIGNAL_H
#define WILTON_SIGNAL_H

#include "wilton/wilton.h"

#ifdef __cplusplus
extern "C" {
#endif

char* wilton_signal_initialize();

char* wilton_signal_await();

char* wilton_signal_fire();

#ifdef __cplusplus
}
#endif

#endif /* WILTON_SIGNAL_H */

