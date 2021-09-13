#ifndef APP_EVENT_HANDLER_H
#define APP_EVENT_HANDLER_H

#include <logging/log.h>
#include <drivers/uart.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <inttypes.h>
#include <device.h>

#define SLEEP_TIME_MS 100
#define STACKSIZE 1024

extern struct k_msgq my_msgq;

#endif