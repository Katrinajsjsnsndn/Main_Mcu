#include "lvgl_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "lcd.h"
#include "test code.h"
#include "lvgl.h" 
#include "lv_port_disp_template.h"
#include "semphr.h"

#include "adc.h"
#include "dac.h"


extern float current;
extern uint16_t dac_set;
/******************************************************************************************************/
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO     1           /* �������ȼ� */
#define START_STK_SIZE      128         /* �����ջ��С */
TaskHandle_t StartTask_Handler;         /* ������ */
void start_task(void *pvParameters);    /* ������ */

/* LV_DEMO_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LV_DEMO_TASK_PRIO   3           /* �������ȼ� */
#define LV_DEMO_STK_SIZE    1024        /* �����ջ��С */
TaskHandle_t LV_DEMOTask_Handler;       /* ������ */
void lv_demo_task(void *pvParameters);  /* ������ */

/* LED_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_TASK_PRIO       4           /* �������ȼ� */
#define LED_STK_SIZE        128         /* �����ջ��С */
TaskHandle_t LEDTask_Handler;           /* ������ */
void led_task(void *pvParameters);      /* ������ */



uint16_t Get_ADC(void);

/******************************************************************************************************/
lv_ui guider_ui;
void lvgl_task()
{
		Lvgl_init();
	  xTaskCreate((TaskFunction_t )start_task,            /* ������ */
                (const char*    )"start_task",          /* �������� */
                (uint16_t       )START_STK_SIZE,        /* �����ջ��С */
                (void*          )NULL,                  /* ���ݸ��������Ĳ��� */
                (UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
                (TaskHandle_t*  )&StartTask_Handler);   /* ������ */

    vTaskStartScheduler();                              /* ����������� */
	
}


/**
 * @brief       start_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    taskENTER_CRITICAL();           /* �����ٽ��� */

    /* ����LVGL���� */
    xTaskCreate((TaskFunction_t )lv_demo_task,
                (const char*    )"lv_demo_task",
                (uint16_t       )LV_DEMO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LV_DEMO_TASK_PRIO,
                (TaskHandle_t*  )&LV_DEMOTask_Handler);

    /* LED�������� */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);

    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
}

/**
 * @brief       LVGL��������
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void lv_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;
		setup_ui(&guider_ui);
		events_init(&guider_ui);
    while(1)
    {
        lv_timer_handler(); /* LVGL��ʱ�� */
        vTaskDelay(5);
    }
}

/**
 * @brief       led_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
uint16_t read_adc;

void led_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    while(1)
    {
			
			  read_adc= Get_ADC();

				HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_2);
        vTaskDelay(10);
    }
}


uint16_t Get_ADC(void)
{
    uint16_t adc_value = 0;

    // 1. ����ADCת��
    HAL_ADC_Start(&hadc1);

    // 2. �ȴ�ת�����
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK)
    {
        // 3. ��ȡת�����
        adc_value = HAL_ADC_GetValue(&hadc1);
    }

    // 4. ֹͣADC
    HAL_ADC_Stop(&hadc1);
		return adc_value;
}

void Current_Set()
{
 	dac_set=4095*((current/5.0f)/2.1f);
	  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_set);//(x/4096)*2.1v
    HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	 


}
void Lvgl_init()
{
 
	 	LCD_Init();			   	//
		LCD_Display_Dir(USE_LCM_DIR);		 		//

		LCD_Clear(WHITE);		

		lv_init();
		lv_port_disp_init();


}
