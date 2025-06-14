#include <stdio.h>
#include <unistd.h>

#include "camera.h"
#include "http.h"

#include "esp_wifi.h"
#include "nvs_flash.h"

EventGroupHandle_t s_wifi_event_group;
// int s_retry_num = 0;

void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
	if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START){
		esp_wifi_connect();
	}else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED){
		// if (s_retry_num < 5){
			esp_wifi_connect();
			// s_retry_num++;
			ESP_LOGI("WIFI", "retry to connect to the AP");
		// }else{
		// 	xEventGroupSetBits(s_wifi_event_group, BIT1);
		// }
		ESP_LOGI("WIFI", "connect to the AP fail");
	}else if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP){
		ip_event_got_ip_t *event = (ip_event_got_ip_t*) event_data;
		ESP_LOGI("WIFI", "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
		// s_retry_num = 0;
		xEventGroupSetBits(s_wifi_event_group, BIT0);
	}
}

void wifi_init_sta(void){
	s_wifi_event_group = xEventGroupCreate();

	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_t *netif = esp_netif_create_default_wifi_sta();
	ESP_ERROR_CHECK(esp_netif_set_hostname(netif, "esp"));

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	esp_event_handler_instance_t instance_any_id;
	esp_event_handler_instance_t instance_got_ip;
	ESP_ERROR_CHECK(esp_event_handler_instance_register(
			WIFI_EVENT,
			ESP_EVENT_ANY_ID,
			&event_handler,
			NULL,
			&instance_any_id));
	ESP_ERROR_CHECK(esp_event_handler_instance_register(
			IP_EVENT,
			IP_EVENT_STA_GOT_IP,
			&event_handler,
			NULL,
			&instance_got_ip));

	wifi_config_t wifi_config = {
		.sta = {
			.ssid = "jpc-rpi", // TO BE MODIFIED
			.password = "52VdXfgUEnrSiKtxoi9fOXnSi", // TO BE MODIFIED
		}
	};

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());
}

void app_main(void)
{

	esp_err_t ret = nvs_flash_init();
	if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
	wifi_init_sta();


	jpcam_init();
	/*jpcam_capture();*/

	start_webserver();
}

