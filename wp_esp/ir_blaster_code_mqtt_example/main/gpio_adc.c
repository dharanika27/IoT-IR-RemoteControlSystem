/*
 * gpio_adc.c
 *
 *  Created on: 05-Jul-2022
 *      Author: Blaze
 */

#include "gpio_adc.h"
#include "string.h"
#include "include/ir_func.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/adc_common.h"
#include "esp_adc_cal.h"
#include "mqtt_client.h"
#include "gpio_adc.h"

int16_t temp_change_config_val = 2,ldr_change_config_val = 500;

float calib_temp_int;
uint8_t pr_test_pos=0;
//int_fast16_t temperatureC=0;
//int_fast16_t LdrValue=0;
float tempC = 0, tempCen_prev = 0;
uint16_t LdrV = 0,LdrV_prev = 0;
unsigned int tctLI = 0,tct = 0;
unsigned char luxtemp=0;
char Prod_Report[100];

static esp_adc_cal_characteristics_t *adc_chars;
const adc_channel_t channel0 = ADC_CHANNEL_0;
const adc_channel_t channel4 = ADC_CHANNEL_4;
const adc_atten_t atten1 = ADC_ATTEN_DB_0;

uint64_t mill_time_button_press = 0;
char button_press_count = 0;

static void IRAM_ATTR gpio_button_isr(void* arg)
{
    button_press_count++;
    if(button_press_count == 1){
		mill_time_button_press = esp_timer_get_time();
	}
}


int init_gpio_pins(void){
	esp_err_t esp_gpio_dir;

/////////////////////////////////////////GPIO_SWITCH/////////////////////////////////////////////

	gpio_config_t io_conf = {};
	io_conf.pin_bit_mask = GPIO_SWITCH;
	//set as input mode
	io_conf.mode = GPIO_MODE_INPUT;
	//enable pull-up mode
	io_conf.pull_up_en = 1;
	gpio_config(&io_conf);

	//change gpio intrrupt type for one pin
	gpio_set_intr_type(GPIO_SWITCH, GPIO_INTR_POSEDGE);

	//install gpio isr service
	gpio_install_isr_service(0);
	//hook isr handler for specific gpio pin
	gpio_isr_handler_add(GPIO_SWITCH, gpio_button_isr, (void*) GPIO_SWITCH);

	printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

//////////////////////////////GPIO_RED_LED//////////////////////////////////////////////
	gpio_reset_pin(GPIO_RED_LED);
	esp_gpio_dir = gpio_set_direction(GPIO_RED_LED, GPIO_MODE_OUTPUT);
	if(esp_gpio_dir!=ESP_OK){
		return 2;
	}
	gpio_set_level(GPIO_RED_LED, 1);// led on when device is ON
	if(esp_gpio_dir!=ESP_OK){
		return 2;
	}
////////////////////////////////GPIO_GRE_LED////////////////////////////////////////////
	gpio_reset_pin(GPIO_GRE_LED);
	esp_gpio_dir = gpio_set_direction(GPIO_GRE_LED, GPIO_MODE_OUTPUT);
	if(esp_gpio_dir!=ESP_OK){
		return 2;
	}
	gpio_set_level(GPIO_GRE_LED, 0);
	if(esp_gpio_dir!=ESP_OK){
		return 2;
	}
////////////////////////////////////////////////////////////////////////////
	return 0;
}

void setRGB(unsigned char ch)
{
    switch(ch)
    {
    	case 0: gpio_set_level(GPIO_RED_LED, 0); //OFF
				gpio_set_level(GPIO_GRE_LED, 0);
				break;

		case 1: gpio_set_level(GPIO_GRE_LED, 1); //green blue
				gpio_set_level(GPIO_RED_LED, 0);
				break;

		case 2: gpio_set_level(GPIO_GRE_LED, 0); //red
				gpio_set_level(GPIO_RED_LED, 1);
				break;

		case 3: gpio_set_level(GPIO_GRE_LED, 1);
				gpio_set_level(GPIO_RED_LED, 1);
				break;
    }
}


static void check_efuse(void)
{
#if CONFIG_IDF_TARGET_ESP32
    //Check if TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }
    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
#elif CONFIG_IDF_TARGET_ESP32S2
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("Cannot retrieve eFuse Two Point calibration values. Default calibration values will be used.\n");
    }
#elif CONFIG_IDF_TARGET_ESP32C3
	if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
		//printf("eFuse Two Point: Supported\n");
	} else {
		printf("Cannot retrieve eFuse Two Point calibration values. Default calibration values will be used.\n");
	}
#else
#error "This example is configured for ESP32/ESP32S2."
#endif
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        //printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}
void IO_init(void)
{
	ESP_LOGI("IO init", "Minimum free heap size xx: %d bytes\n",esp_get_minimum_free_heap_size());
	//////////IR Transmit end/////////////

	//Check if Two Point or Vref are burned into eFuse
	check_efuse();

	//Configure ADC
	if (unit == ADC_UNIT_1) {
		adc1_config_width(width);
		adc1_config_channel_atten(channel, atten);
	} else {
		adc2_config_channel_atten((adc2_channel_t)channel, atten);
	}

	//Characterize ADC
	adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
	esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);
	print_char_val_type(val_type);

	//Configure ADC
	if (unit1 == ADC_UNIT_1) {
		adc1_config_width(width1);
		adc1_config_channel_atten(channel1, atten);
	} else {
		adc2_config_channel_atten((adc2_channel_t)channel, atten);
	}

	//Characterize ADC
	adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
	esp_adc_cal_value_t val_type1 = esp_adc_cal_characterize(unit1, atten, width1, DEFAULT_VREF, adc_chars);
	print_char_val_type(val_type1);
}

void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    reverse(str, i);

    while (i < d)
    	str[i++] = '0';

    str[i] = '\0';
    return i;
}

void ftoa(float n, char *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;
    fpart = fpart*100;
    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

float read_lm35_temp_value(){
	uint32_t adc_reading = 0;
	//Multisampling
	for (int i = 0; i < NO_OF_SAMPLES; i++) {
		int raw;
		adc2_get_raw((adc2_channel_t)channel, width, &raw);
		adc_reading += raw;
	}
	//Convert adc_reading to voltage in mV
	return (esp_adc_cal_raw_to_voltage(adc_reading /= NO_OF_SAMPLES, adc_chars))/10.f;
}

uint32_t read_lrd_value(){
	uint32_t adc_reading1 = 0;
	for (int i = 0; i < NO_OF_SAMPLES; i++) {
		adc_reading1 += adc1_get_raw((adc1_channel_t)channel1);
	}
	//Convert adc_reading to voltage in mV
	return esp_adc_cal_raw_to_voltage(adc_reading1 /= NO_OF_SAMPLES, adc_chars);
}



void read_temp_ldr(unsigned char type){
//	int k=0;
	char mqtt_pub_data[200];
	char buffer[6];
//	uint32_t voltage_val_temp = (read_lm35_temp_value());
	tempC = (read_lm35_temp_value())+calib_temp_int;
//	printf(" Temp test:%.2f C,%.2f C\n", tempC,tempCen_prev);
	LdrV = read_lrd_value()*10;
	if(LdrV < 500)
	{
		LdrV = 500;
	}
	else if(LdrV > 10000)
	{
		LdrV = 10000;
	}
//	LdrValue = 0;

//	printf(" Lux: %d, %d\n", LdrV,LdrV_prev);


	if(abs(tempCen_prev - tempC) >= temp_change_config_val)
	{
		tct++;
	}
	else
	{
		tct = 0;
	}
	if(abs(LdrV_prev - LdrV) > ldr_change_config_val)
	{
		tctLI++;
	}
	else
	{
		tctLI = 0;
	}
	if (tctLI >= 2 || tct >= 2)
	{
		tempCen_prev = tempC;
		LdrV_prev = LdrV;
		snprintf(mqtt_pub_data,200,"{\"event_type\":\"HubDetailsEvent\",\"data\":{\"temp\":\"%.2f\",\"lux\":\"%d\"}}",tempC,LdrV);
//		if(isMQTT == 1){
		esp_mqtt_client_publish(client, "mqtt/temp_data", mqtt_pub_data, 0, 1, 0);
		printf("%s\n",mqtt_pub_data);
//		}
		tct = 0;
		tctLI = 0;
	}

	if(type == 1){
//		int k=0;
		ftoa((float)tempC,buffer,2);
		snprintf(mqtt_pub_data,200,"{\"event_type\":\"HubDetailsEvent\",\"data\":{\"temp\":\"%.2f\",\"lux\":\"%d\"}}",LdrV,tempC);
//		if(isMQTT == 1){
		esp_mqtt_client_publish(client, "mqtt/temp_data", mqtt_pub_data, 0, 1, 0);
		printf("%s\n",mqtt_pub_data);
//		}
	}
}
