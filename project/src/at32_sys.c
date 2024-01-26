/*
 * @Description:
 * @Blog: saisaiwa.com
 * @Author: ccy
 * @Date: 2024-01-23 15:03:42
 * @LastEditTime: 2024-01-24 13:20:43
 */
#include "at32_sys.h"

/* delay variable */
#define STEP_DELAY_MS 50
static __IO uint32_t fac_us;
static __IO uint32_t fac_ms;

/* support printf function, usemicrolib is unnecessary */
#if (__ARMCC_VERSION > 6000000)
__asm(".global __use_no_semihosting\n\t");
void _sys_exit(int x) {
    x = x;
}
/* __use_no_semihosting was requested, but _ttywrch was */
void _ttywrch(int ch) {
    ch = ch;
}
FILE __stdout;
#else
#ifdef __CC_ARM
#pragma import(__use_no_semihosting)
struct __FILE {
    int handle;
};
FILE __stdout;
void _sys_exit(int x) {
    x = x;
}
/* __use_no_semihosting was requested, but _ttywrch was */
void _ttywrch(int ch) {
    ch = ch;
}
#endif
#endif

#if defined(__GNUC__) && !defined(__clang__)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE* f)
#endif

/**
 * @brief  retargets the c library printf function to the usart.
 * @param  none
 * @retval none
 */
PUTCHAR_PROTOTYPE {
    while (usart_flag_get(USART1, USART_TDBE_FLAG) == RESET)
        ;
    usart_data_transmit(USART1, (uint16_t)ch);
    while (usart_flag_get(USART1, USART_TDC_FLAG) == RESET)
        ;
    return ch;
}

#if (defined(__GNUC__) && !defined(__clang__)) || (defined(__ICCARM__))
#if defined(__GNUC__) && !defined(__clang__)
int _write(int fd, char* pbuffer, int size)
#elif defined(__ICCARM__)
#pragma module_name = "?__write"
int __write(int fd, char* pbuffer, int size)
#endif
{
    for (int i = 0; i < size; i++) {
        while (usart_flag_get(USART1, USART_TDBE_FLAG) == RESET)
            ;
        usart_data_transmit(USART1, (uint16_t)(*pbuffer++));
        while (usart_flag_get(USART1, USART_TDC_FLAG) == RESET)
            ;
    }

    return size;
}
#endif

void delay_init(void) {
    systick_clock_source_config(SYSTICK_CLOCK_SOURCE_AHBCLK_DIV8);
    fac_us = system_core_clock / (8000000U);
    fac_ms = fac_us * (1000U);
}

/**
 * @brief  inserts a delay time.
 * @param  nus: specifies the delay time length, in microsecond.
 * @retval none
 */
void delay_us(uint32_t nus) {
    uint32_t temp = 0;
    SysTick->LOAD = (uint32_t)(nus * fac_us);
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
}

/**
 * @brief  inserts a delay time.
 * @param  nms: specifies the delay time length, in milliseconds.
 * @retval none
 */
void delay_ms(uint16_t nms) {
    uint32_t temp = 0;
    while (nms) {
        if (nms > STEP_DELAY_MS) {
            SysTick->LOAD = (uint32_t)(STEP_DELAY_MS * fac_ms);
            nms -= STEP_DELAY_MS;
        } else {
            SysTick->LOAD = (uint32_t)(nms * fac_ms);
            nms = 0;
        }
        SysTick->VAL = 0x00;
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
        do {
            temp = SysTick->CTRL;
        } while ((temp & 0x01) && !(temp & (1 << 16)));

        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
        SysTick->VAL = 0x00;
    }
}

/**
 * @brief  inserts a delay time.
 * @param  sec: specifies the delay time, in seconds.
 * @retval none
 */
void delay_sec(uint16_t sec) {
    uint16_t index;
    for (index = 0; index < sec; index++) {
        delay_ms(500);
        delay_ms(500);
    }
}
