/* add user code begin Header */
/**
 **************************************************************************
 * @file     main.c
 * @brief    main program
 **************************************************************************
 *                       Copyright notice & Disclaimer
 *
 * The software Board Support Package (BSP) that is made available to
 * download from Artery official website is the copyrighted work of Artery.
 * Artery authorizes customers to use, copy, and distribute the BSP
 * software and its related documentation for the purpose of design and
 * development in conjunction with Artery microcontrollers. Use of the
 * software is governed by this copyright notice and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
 * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
 * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
 * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
 * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
 *
 **************************************************************************
 */
/* add user code end Header */

/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407_wk_config.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "user_service.h"
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */

/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */


/* add user code end 0 */

/**
 * @brief main function.
 * @param  none
 * @retval none
 */
int main(void) {
    /* add user code begin 1 */

    /* add user code end 1 */

    /* system clock config. */
    wk_system_clock_config();

    /* config periph clock. */
    wk_periph_clock_config();

    /* init debug function. */
    wk_debug_config();

    /* nvic config. */
    wk_nvic_config();

    /* init spim function. */
    wk_spim_init();

    /* init usart1 function. */
    wk_usart1_init();

    /* init exint function. */
    wk_exint_config();

    /* init sdio2 function. */
    wk_sdio2_init();

    /* init tmr2 function. */
    wk_tmr2_init();

    /* init gpio function. */
    wk_gpio_config();

    /* add user code begin 2 */

    printf("Run System\n");

    delay_init();

    // tmr_counter_enable(TMR2, FALSE);
    // tmr_interrupt_enable(TMR2, TMR_OVF_INT, TRUE);
    // tmr_counter_enable(TMR2, TRUE);

    /* enter critical */
    taskENTER_CRITICAL();
    us_service_init();
    /* exit critical */
    taskEXIT_CRITICAL();

    /* start scheduler */
    vTaskStartScheduler();
    /* add user code end 2 */

    while (1) {
        /* add user code begin 3 */
        // delay_ms(1000);
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("Loop\n");
        /* add user code end 3 */
    }
}
