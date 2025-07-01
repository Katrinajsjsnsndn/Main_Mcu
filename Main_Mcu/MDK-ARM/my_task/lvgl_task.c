#include "lvgl_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "lcd.h"
#include "test code.h"
#include "lvgl.h" 
#include "lv_port_disp_template.h"
#include "semphr.h"


/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO     1           /* 任务优先级 */
#define START_STK_SIZE      128         /* 任务堆栈大小 */
TaskHandle_t StartTask_Handler;         /* 任务句柄 */
void start_task(void *pvParameters);    /* 任务函数 */

/* LV_DEMO_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LV_DEMO_TASK_PRIO   3           /* 任务优先级 */
#define LV_DEMO_STK_SIZE    1024        /* 任务堆栈大小 */
TaskHandle_t LV_DEMOTask_Handler;       /* 任务句柄 */
void lv_demo_task(void *pvParameters);  /* 任务函数 */

/* LED_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_TASK_PRIO       4           /* 任务优先级 */
#define LED_STK_SIZE        128         /* 任务堆栈大小 */
TaskHandle_t LEDTask_Handler;           /* 任务句柄 */
void led_task(void *pvParameters);      /* 任务函数 */
/******************************************************************************************************/

void lvgl_task()
{
		Lvgl_init();
	  xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传递给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */

    vTaskStartScheduler();                              /* 开启任务调度 */
	
}


/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    taskENTER_CRITICAL();           /* 进入临界区 */

    /* 创建LVGL任务 */
    xTaskCreate((TaskFunction_t )lv_demo_task,
                (const char*    )"lv_demo_task",
                (uint16_t       )LV_DEMO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LV_DEMO_TASK_PRIO,
                (TaskHandle_t*  )&LV_DEMOTask_Handler);

    /* LED测试任务 */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);

    taskEXIT_CRITICAL();            /* 退出临界区 */
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
}

/**
 * @brief       LVGL运行例程
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void lv_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
   /* 获取屏幕对象 */
    lv_obj_t * scr = lv_scr_act();

    /* 创建左侧按钮 */
    lv_obj_t * btn_left = lv_btn_create(scr);               /* 在当前屏幕上创建一个按钮 */
    lv_obj_set_pos(btn_left, 50, 50);    /* 设置按钮的位置 */
    lv_obj_set_size(btn_left, 60, 30);                     /* 设置按钮的大小 */

    /* 创建左侧按钮的标签 */
    lv_obj_t * label_left = lv_label_create(btn_left);      /* 在按钮上创建一个标签 */
    lv_label_set_text(label_left, "run");              /* 设置标签的文本为“左侧按钮” */
    lv_obj_align(label_left, LV_ALIGN_CENTER, 0, 0);        /* 将标签对齐到按钮的中心 */


    /* 创建右侧按钮 */
    lv_obj_t * btn_right = lv_btn_create(scr);              /* 在当前屏幕上创建一个按钮 */
    lv_obj_set_pos(btn_right, 150, 50); /* 设置按钮的位置 */
    lv_obj_set_size(btn_right, 60, 30);                    /* 设置按钮的大小 */

    /* 创建右侧按钮的标签 */
    lv_obj_t * label_right = lv_label_create(btn_right);    /* 在按钮上创建一个标签 */
    lv_label_set_text(label_right, "stop");             /* 设置标签的文本为“右侧按钮” */
    lv_obj_align(label_right, LV_ALIGN_CENTER, 0, 0);       /* 将标签对齐到按钮的中心 */
    while(1)
    {
        lv_timer_handler(); /* LVGL计时器 */
        vTaskDelay(5);
    }
}

/**
 * @brief       led_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void led_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    while(1)
    {
        //LED0_TOGGLE();
        vTaskDelay(5);
    }
}
void Lvgl_init()
{
 
	 	LCD_Init();			   	//
		LCD_Display_Dir(USE_LCM_DIR);		 		//

		LCD_Clear(WHITE);		

		lv_init();
		lv_port_disp_init();


}
