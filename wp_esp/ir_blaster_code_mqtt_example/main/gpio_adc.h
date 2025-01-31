/*
 * gpio_adc.h
 *
 *  Created on: 24-May-2022
 *      Author: HP
 */

#ifndef MAIN_GPIO_ADC_H_
#define MAIN_GPIO_ADC_H_

#include "driver/gpio.h"
#include "driver/adc_common.h"
#include "esp_adc_cal.h"
#include "esp_log.h"
#include "esp_system.h"
#include "mqtt_client.h"
//#include "app_main.h"

extern esp_mqtt_client_handle_t client;


//IR Transmit
#define RT_INTX_PIN (GPIO_NUM_6)
#define RT_INRX_PIN (GPIO_NUM_19)

//IR Receive
#define RT_OUTTX_PIN (GPIO_NUM_18)
#define RT_OUTRX_PIN (GPIO_NUM_10)

//IR Led
#define RT_OUT1TX_PIN (GPIO_NUM_8)
#define RT_OUT1RX_PIN (GPIO_NUM_9)

#define GPIO_OUTPUT_PIN_SEL1  (1ULL<<RT_INTX_PIN)
#define GPIO_INPUT_PIN_SEL1  (1ULL<<RT_INRX_PIN)
#define GPIO_OUTPUT_PIN_SEL2  (1ULL<<RT_OUTTX_PIN)
#define GPIO_INPUT_PIN_SEL2  (1ULL<<RT_OUTRX_PIN)
#define GPIO_OUTPUT_PIN_SEL3  (1ULL<<RT_OUT1TX_PIN)
#define GPIO_INPUT_PIN_SEL3  (1ULL<<RT_OUT1RX_PIN)


#define GPIO_IR_RX	 10		//2 dev board//10 main board//2 devboard						//10 in ir blaster -c3
#define GPIO_IR_TX	 6		//10 dev board//6 main board//dev board 10           10 in  in c3 dev board
							//6 in ir blaster -c3

#define GPIO_IR_LED 8
#define GPIO_RED_LED 0
#define GPIO_GRE_LED 1
#define GPIO_SWITCH 9

extern uint64_t mill_time_button_press;
extern char button_press_count;

void setRGB(unsigned char ch);
int init_gpio_pins(void);

extern unsigned char luxtemp;

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
//#define NO_OF_SAMPLES   64          //Multisampling

//static esp_adc_cal_characteristics_t *adc_chars;
#if CONFIG_IDF_TARGET_ESP32
static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
#elif CONFIG_IDF_TARGET_ESP32S2
static const adc_channel_t channel = ADC_CHANNEL_6;     // GPIO7 if ADC1, GPIO17 if ADC2
static const adc_bits_width_t width = ADC_WIDTH_BIT_13;
#endif
static const adc_atten_t atten = ADC_ATTEN_DB_11;
static const adc_unit_t unit = ADC_UNIT_2;
static const adc_channel_t channel = ADC_CHANNEL_0;
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
static const adc_unit_t unit1 = ADC_UNIT_1;
static const adc_channel_t channel1 = ADC_CHANNEL_4;
static const adc_bits_width_t width1 = ADC_WIDTH_BIT_12;


#define NO_OF_SAMPLES   64

//extern static esp_adc_cal_characteristics_t *adc_chars;
extern const adc_channel_t channel0;
extern const adc_channel_t channel4;
extern const adc_atten_t atten1;


extern char Prod_Report[100];
extern uint8_t pr_test_pos;


//extern int_fast16_t temperatureC,LdrValue;
extern float tempC;
extern uint16_t LdrV;
void read_temp_ldr(unsigned char type);
void IO_init(void);

//#endif


#endif /* MAIN_GPIO_ADC_H_ */
