/* MQTT (over TCP) Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "include/ir_func.h"
#include "gpio_adc.h"

static const char *TAG = "MQTT_EXAMPLE";

esp_mqtt_client_handle_t client;

#define MAX_SIZE 2100
char pubBuff[MAX_SIZE];
char ir_buf[MAX_SIZE];

char ir_repeat_fun = 0;

char mqtt_pub_data[2000];

void ir_key_learn_status(int status,char *IR_Data){
	if(status == 1)
	{
	   printf("Ir data:%s\n", IR_Data);
	   snprintf(mqtt_pub_data,2000,"{\"Ir\":\"LearnData\",\"data\":\"%s\"}",IR_Data);
	   //		if(isMQTT == 1){
	   		esp_mqtt_client_publish(client, "mqtt/learn", mqtt_pub_data, 0, 1, 0);
	}
	else
	{
		printf("\nLearn_fail\n");
		esp_mqtt_client_publish(client, "mqtt/learn", "Learn Fail or time out", 0, 1, 0);
		snprintf(mqtt_pub_data,2000,"{\"Ir\":\"LearnData\",\"data\":\"Learn fail-%d\"}",status);
	}


}





static void log_error_if_nonzero(const char * message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
	int startver = 0;
    client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
//            msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
//            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/mqtt/receive", 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
//            msg_id = esp_mqtt_client_publish(client, "/mqtt/connected", "data", 0, 0, 0);
//            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);


            if(event->data_len == event->total_data_len)
			{
				strncpy((char *)pubBuff,(const void*) event->data,event->data_len);
				pubBuff[event->data_len] = '\0';
				startver=1;
			}
			else
			{
				if(event->data_len + event->current_data_offset == event->total_data_len)
				{
					strncpy(((char *)pubBuff) + (event->current_data_offset),(const void*) event->data,event->data_len);
					pubBuff[event->total_data_len] = '\0';
					startver=1;
				}
				else
				{
					strncpy(((char *)pubBuff) + (event->current_data_offset),(const void*) event->data,event->data_len);
				}

			}
			if(startver == 1){
				if(pubBuff[0] == 'I' && pubBuff[1] == 'R' && (pubBuff[2] == 'C' || pubBuff[2] == 'R')){
					ir_repeat_fun = 1;
				}
				else if(pubBuff[0] == 'I' && pubBuff[1] == 'R' && (pubBuff[2] == 'L')){
					ir_repeat_fun = 2;
					printf("learn mode\n");
				}
				else if(pubBuff[0] == 'R' && pubBuff[1] == 'T' && (pubBuff[2] == 'L')){
					read_temp_ldr(1);
					printf("read temp and LDR\n");
				}
			}
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
                ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

            }
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

static void mqtt_app_start(void)
{
	char tmp[30];
	printf("\n MQTT mqtt_init\n");
	sprintf(tmp, "ESPWR32ABCDS%08X",esp_random());
	printf("%s",tmp);
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_BROKER_URL,
		.client_id = tmp,
    };
#if CONFIG_BROKER_URL_FROM_STDIN
    char line[128];

    if (strcmp(mqtt_cfg.uri, "FROM_STDIN") == 0) {
        int count = 0;
        printf("Please enter url of mqtt broker\n");
        while (count < 128) {
            int c = fgetc(stdin);
            if (c == '\n') {
                line[count] = '\0';
                break;
            } else if (c > 0 && c < 127) {
                line[count] = c;
                ++count;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        mqtt_cfg.uri = line;
        printf("Broker url: %s\n", line);
    } else {
        ESP_LOGE(TAG, "Configuration mismatch: wrong broker url");
        abort();
    }
#endif /* CONFIG_BROKER_URL_FROM_STDIN */

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    init_gpio_pins();

	char sdk_ver[5];
	get_ir_idf_version(sdk_ver);
	printf("IR SDK version:%s",sdk_ver);

    init_ir_func(0);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
//    IO_init();

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    mqtt_app_start();

    while(1){

    	if(button_press_count > 0){
    			printf("button pressed:%d\n",button_press_count);
    			button_press_count = 0;
    			printf("\n");
    			gpio_set_level(GPIO_RED_LED, 0);
    			vTaskDelay(1000 / portTICK_PERIOD_MS);
    			gpio_set_level(GPIO_RED_LED, 1);
    	}
    	if(ir_repeat_fun == 1){

    		for(int x=0;x<strlen(pubBuff)-3;x++){
				ir_buf[x] = pubBuff[x+3];
			}
    		printf("ir_data :%s\n",ir_buf);
//    		vTaskSuspendAll();
    		int is_key = 0;
    		if(pubBuff[2] == 'C'){
    			is_key = IR_Repeat(ir_buf,0);
    		}
    		else{
    			is_key = IR_Repeat(ir_buf,1);
    		}
//			xTaskResumeAll();
			if(is_key==1)
			{
				ESP_LOGI(TAG, "Key fire success ");
			}
			else
			{
				ESP_LOGI(TAG, "Fire fail - err(%d)",is_key);
			}
			ir_repeat_fun = 0;
    	}else if(ir_repeat_fun == 2){
    		IR_Learn(15);
    		ir_repeat_fun = 0;
    	}
//    	read_temp_ldr(2);
    	vTaskDelay(100 / portTICK_PERIOD_MS);
    }

}
