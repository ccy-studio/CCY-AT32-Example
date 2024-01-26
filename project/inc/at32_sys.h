#ifndef __AT32_SYS_H
#define __AT32_SYS_H

#include "at32f403a_407_wk_config.h"
#include "FreeRTOS.h"
#include "task.h"

/* delay function */
void delay_init(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
void delay_sec(uint16_t sec);

#endif