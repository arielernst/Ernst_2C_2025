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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/

/*==================[external functions definition]==========================*/



struct leds
{
    uint8_t mode;       //ON, OFF, TOGGLE
	uint8_t n_led;        //indica el número de led a controlar
	uint8_t n_ciclos;   //indica la cantidad de ciclos de encendido/apagado
	uint16_t periodo;    //indica el tiempo de cada ciclo
} my_leds; 

enum {ON, OFF, TOGGLE};


void controlaLeds(struct leds *led){
	switch(led->mode){
		case ON:
			LedOn(led->n_led);
		break;
		case OFF:
			LedOff(led->n_led);
		break;
		case TOGGLE:
		#define CONFIG_BLINK_PERIOD led->periodo
		for (int i=0; i<led->n_ciclos; i++){
			LedToggle(led->n_led);
			vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
			LedToggle(led->n_led);
			vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
			}
		break;
	}
}

void app_main(void){
	LedsInit();
	struct leds Led={TOGGLE, LED_1, 10, 500};
	controlaLeds(&Led);
}

/*==================[end of file]============================================*/