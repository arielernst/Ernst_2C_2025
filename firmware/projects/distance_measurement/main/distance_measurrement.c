/*! @mainpage Template
 *
 * @section genDesc General Description
 *
 * This section describes how the program works.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	GPIO_X		|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 12/09/2023 | Document creation		                         |
 *
 * @author Albano Peñalva (albano.penalva@uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "hc_sr04.h"
#include "lcditse0803.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/



#define CONFIG_MEASURE_PERIOD 1000

/*==================[internal data definition]===============================*/
TaskHandle_t led_task_handle = NULL;
TaskHandle_t bcd_task_handle = NULL;

/*==================[internal functions declaration]=========================*/
static void LedsTask(void *pvParameter){
	while(1){
		uint16_t distance = HcSr04ReadDistanceInCentimeters();
		
		if (distance > 30){
			LedOn(LED_1);
			LedOn(LED_2);
			LedOn(LED_3);
		}else if (distance > 20){
			LedOff(LED_3);
			LedOn(LED_2);
			LedOn(LED_1);
		}else if(distance >10){
			LedOff(LED_3);
			LedOff(LED_2);
			LedOn(LED_1);
		}else{
			LedOff(LED_3);
			LedOff(LED_2);
			LedOff(LED_1);			
		}
		vTaskDelay(CONFIG_MEASURE_PERIOD / portTICK_PERIOD_MS);
		
	}
}


static void BcdTask(void *pvParameter){
	while(1){
		uint16_t distance = HcSr04ReadDistanceInCentimeters();
		LcdItsE0803Write(distance);
		vTaskDelay(CONFIG_MEASURE_PERIOD / portTICK_PERIOD_MS);
	}
}
/*==================[external functions definition]==========================*/
void app_main(void){
    LedsInit();
	HcSr04Init(GPIO_3, GPIO_2);
	LcdItsE0803Init();
    xTaskCreate(&LedsTask, "LED", 512, NULL, 5, &led_task_handle);
	xTaskCreate(&BcdTask, "BCD", 512, NULL, 5, &bcd_task_handle);
}

/*==================[end of file]============================================*/