/*
 * @Description:
 * @Blog: saisaiwa.com
 * @Author: ccy
 * @Date: 2024-01-25 15:00:37
 * @LastEditTime: 2024-01-26 13:31:21
 */
#include "user_service.h"
#include "at32_sdio.h"
#include "ff.h"
#include "run_in_spim.h"

// SD卡文件测试
FATFS fs;
FIL file;
BYTE work[FF_MAX_SS];

static void sd_test_error(void) {
    printf("SD卡Test错误\r\n");
}

uint8_t buffer_compare(uint8_t* pbuffer1,
                       uint8_t* pbuffer2,
                       uint16_t buffer_length) {
    while (buffer_length--) {
        if (*pbuffer1 != *pbuffer2) {
            return 0;
        }
        pbuffer1++;
        pbuffer2++;
    }
    return 1;
}

static error_status fatfs_test(void) {
    FRESULT ret;
    char filename[] = "1:/test1.txt";
    const char wbuf[] = "this is my file for test fatfs!\r\n";
    char rbuf[50];
    UINT bytes_written = 0;
    UINT bytes_read = 0;
    DWORD fre_clust, fre_sect, tot_sect;
    FATFS* pt_fs;

    ret = f_mount(&fs, "1:", 1);
    if (ret) {
        printf("fs mount err:%d.\r\n", ret);
        if (ret == FR_NO_FILESYSTEM) {
            printf("create fatfs..\r\n");
            ret = f_mkfs("1:", 0, work, sizeof(work));  // 格式化文件系统
            if (ret) {
                printf("creates fatfs err:%d.\r\n", ret);
                return ERROR;
            } else {
                printf("creates fatfs ok.\r\n");
            }
            // NULL作为第一个参数表示取消挂载之前已经挂载的文件系统对象
            ret = f_mount(NULL, "1:", 1);
            ret = f_mount(&fs, "1:", 1);  // 重新挂载
            if (ret) {
                printf("fs mount err:%d.\r\n", ret);
                return ERROR;
            } else {
                printf("fs mount ok.\r\n");
            }
        } else {
            return ERROR;
        }
    } else {
        printf("fs mount ok.\r\n");
    }

    ret = f_open(&file, filename, FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
    if (ret) {
        printf("open file err:%d.\r\n", ret);
    } else {
        printf("open file ok.\r\n");
    }
    ret = f_write(&file, wbuf, sizeof(wbuf), &bytes_written);
    if (ret) {
        printf("write file err:%d.\r\n", ret);
    } else {
        printf("write file ok, byte:%u.\r\n", bytes_written);
    }
    // 移动文件的读写指针
    f_lseek(&file, 0);
    ret = f_read(&file, rbuf, sizeof(rbuf), &bytes_read);
    if (ret) {
        printf("read file err:%d.\r\n", ret);
    } else {
        printf("read file ok, byte:%u.\r\n", bytes_read);
    }
    ret = f_close(&file);
    if (ret) {
        printf("close file err:%d.\r\n", ret);
    } else {
        printf("close file ok.\r\n");
    }

    pt_fs = &fs;
    /* get volume information and free clusters of drive 1 */
    ret = f_getfree("1:", &fre_clust, &pt_fs);
    if (ret == FR_OK) {
        /* get total sectors and free sectors */
        tot_sect = (pt_fs->n_fatent - 2) * pt_fs->csize;
        fre_sect = fre_clust * pt_fs->csize;

        /* print the free space (assuming 512 bytes/sector) */
        printf("%10u KiB total drive space.\r\n%10u KiB available.\r\n",
               tot_sect / 2, fre_sect / 2);
    }

    ret = f_mount(NULL, "1:", 1);

    if (1 == buffer_compare((uint8_t*)rbuf, (uint8_t*)wbuf, sizeof(wbuf))) {
        printf("r/w file data test ok.\r\n");
    } else {
        printf("r/w file data test fail.\r\n");
        return ERROR;
    }

    return SUCCESS;
}
// END

static u32 counter = 0;

void led_task_function(void* pvParameters);
void print_task_function(void* pvParameters);

void us_service_init() {
    if (xTaskCreate(led_task_function, "LED", 512, NULL, 2, NULL) != pdPASS) {
        printf("Create led task fail\r\n");
    }

    if (xTaskCreate(print_task_function, "PRINT", 1024, NULL, 1, NULL) !=
        pdPASS) {
        printf("Create print_task_function task fail\r\n");
    }

    if (SUCCESS != fatfs_test()) {
        sd_test_error();
    }
}

void led_task_function(void* pvParameters) {
    while (1) {
        LED_GPIO_PORT->odt ^= LED_PIN;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
void print_task_function(void* pvParameters) {
    static float float_num = 0.00;
    while (1) {
        float_num += 0.01f;
        printf("float_num = %.4f counter = %ld\r\n", float_num, counter);
        run_fun_spim_code();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void IRQ_TIM2_Handler(void) {
    counter++;
}

void IRQ_INT0_Handler(void) {
    if (gpio_input_data_bit_read(LED_GPIO_PORT, LED_PIN) == SET) {
        printf("irq button\n");
    }
}