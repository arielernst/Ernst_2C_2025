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
#include "switch.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/



#define CONFIG_MEASURE_PERIOD 1000
#define CONFIG_READING_PERIOD 20

/*==================[internal data definition]===============================*/
TaskHandle_t led_bcd_task_handle = NULL;
TaskHandle_t switches_task_handle = NULL;

bool measure = true;
bool result = false;

/*==================[internal functions declaration]=========================*/
static void LedsBcdTask(void *pvParameter){
	while(1){
		uint16_t distance = HcSr04ReadDistanceInCentimeters();
		
		if(measure == false && result == false){
			LedOff(LED_3);
			LedOff(LED_2);
			LedOff(LED_1);	
		}else if (distance > 30 && result == false){
			LedOn(LED_1);
			LedOn(LED_2);
			LedOn(LED_3);
		}else if (distance > 20 && result == false){
			LedOff(LED_3);
			LedOn(LED_2);
			LedOn(LED_1);
		}else if(distance >10 && result == false){
			LedOff(LED_3);
			LedOff(LED_2);
			LedOn(LED_1);
		}else if (result==false){
			LedOff(LED_3);
			LedOff(LED_2);
			LedOff(LED_1);			
		}


		if(measure && result==false){
			LcdItsE0803Write(distance);
		}else if(result){
			;
		}else{
			LcdItsE0803Off();
		}
		vTaskDelay(CONFIG_MEASURE_PERIOD / portTICK_PERIOD_MS);
		
	}
}

static void ReadSwitchTask(void *pvParameter){
	while(1){
	uint8_t switchOn = SwitchesRead();
	if (switchOn == SWITCH_1){
		measure = ! measure;
	}else if(switchOn == SWITCH_2){
		result = ! result;
	}
	vTaskDelay(CONFIG_READING_PERIOD / portTICK_PERIOD_MS);
}
}

/*==================[external functions definition]==========================*/
void app_main(void){
    LedsInit();
	HcSr04Init(GPIO_3, GPIO_2);
	LcdItsE0803Init();
	SwitchesInit();
    xTaskCreate(&LedsBcdTask, "LED", 512, NULL, 5, &led_bcd_task_handle);
	xTaskCreate(&ReadSwitchTask, "SWITCH", 512, NULL, 5, &switches_task_handle);
}

/*==================[end of file]============================================*/