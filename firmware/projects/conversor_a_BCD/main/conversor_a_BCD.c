/*! @mainpage Template
 *
 * @section genDesc General Description
 *
 * This program recieves a 32 bit number, and its amount of decimal digits, and shows it on a BCD display
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	   D1	 	| 	 GPIO_20	|
 * |       D2       |    GPIO_21    |
 * |       D3       |    GPIO_22    |
 * |       D4       |    GPIO_23    |
 * |      SEL_1     |    GPIO_19    |
 * |      SEL_2     |    GPIO_18    |
 * |      SEL_3     |     GPIO_9    |
 * |      +5V       |      +5V      |
 * |      GND       |      GND      |
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 05/09/2025 | Document creation		                         |
 *
 * @author Ernst, Ariel David (arielernst3@gmail.com)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "switch.h"
#include "gpio_mcu.h"


/*==================[macros and definitions]=================================*/

/** @fn void convertToBcdArray(uint32_t data, uint8_t digits, uint8_t * bcd_number)
 * @brief recibe el dato de 32 bits y lo guarda en un arreglo BCD con cada uno de sus dígitos
 * @param[in] data el dato de 32 bits
 * @param[in] digits el número de dpigitos que tiene el dato
 * @param[in] bcd_number el arreglo BCD
 * @return
*/
void  convertToBcdArray (uint32_t data, uint8_t digits, uint8_t * bcd_number);

/** @fn void setBcdToGpio(uint8_t bcd, gpioConf_t *gpio_vector)
 * @brief recibe un dígito BCD y un vector del tipo gpioConf_t, y configura este último para mostrar en el display el dígito bcd
 * @param[in] bcd el dígito bcd
 * @param[in] gpio_vector el vector gpioConf_t
 * @return
 */
void setBcdToGpio(uint8_t bcd, gpioConf_t *gpio_vector);

/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/

/*==================[external functions definition]==========================*/

void  convertToBcdArray (uint32_t data, uint8_t digits, uint8_t * bcd_number)
{
	for (int8_t i = digits - 1; i >= 0; i--) {
        bcd_number[i] = data % 10;
        data = data /10;
    }
}



		typedef struct
{
	gpio_t pin;			/*!< GPIO pin number */
	io_t dir;			/*!< GPIO direction '0' IN;  '1' OUT*/
} gpioConf_t;



void setBcdToGpio(uint8_t bcd, gpioConf_t *gpio_vector)
{
    for (int i = 0; i < 4; i++) {
        uint8_t bit = (bcd >> i) & 1;  // extraigo el bit i del BCD 0x01
        
		if (bit) {
            GPIOOn(gpio_vector[i].pin);   // función del driver que pone el pin en '1'
        } else {
            GPIOOff(gpio_vector[i].pin);  // función del driver que pone el pin en '0'
        }
	}
}


void from32bitstoGPIO(uint32_t data, uint8_t digits){
	
	uint8_t bcd_number[3]={0,0,0};
	
	gpioConf_t bcd_gpio[4] = {
    	{ GPIO_20, GPIO_OUTPUT },
    	{ GPIO_21, GPIO_OUTPUT },
    	{ GPIO_22, GPIO_OUTPUT },
    	{ GPIO_23, GPIO_OUTPUT }
	};

	gpioConf_t digit[3] = {
		{ GPIO_19, GPIO_OUTPUT }, //primer dígito
		{ GPIO_18, GPIO_OUTPUT }, //segundo dígito
		{ GPIO_9 , GPIO_OUTPUT }  //tercer dígito	
	};

	for (int i = 0; i < 4; i++){
        GPIOInit(bcd_gpio[i].pin, bcd_gpio[i].dir);
    }

    for (int i = 0; i < 3; i++){
        GPIOInit(digit[i].pin, digit[i].dir);
    }

	uint8_t *ptr_bcd_number= bcd_number;

	gpioConf_t *ptr_bcd_gpio = bcd_gpio;

	gpioConf_t *ptr_digit = digit;

	convertToBcdArray(data, digits, ptr_bcd_number);

	while(1){
	for (int i = 0; i < digits; i++){
		
		GPIOOff(ptr_digit[0].pin);
		GPIOOff(ptr_digit[1].pin);
		GPIOOff(ptr_digit[2].pin);
		uint8_t bcd = bcd_number[i];
		setBcdToGpio(bcd, ptr_bcd_gpio);
		GPIOOn(ptr_digit[i].pin);
	}

	}
}

void app_main(void){
	
	uint32_t data = 724;
	
	from32bitstoGPIO(data, 3);
}

/*==================[end of file]============================================*/