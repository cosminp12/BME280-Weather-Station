/*********************************************************************************
Copyright (c) 2016, Cosmin Plasoianu
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 ********************************************************************************/

#include "user_main.h"


static os_timer_t uploadTmr, secTaskTmr;
uint8 SecTskCnt = 0, MinTskCnt = 0;


void user_init(void)
{
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	os_delay_us(1000000);

	i2c_master_gpio_init();

	WiFi_Init();

	ets_uart_printf("Started!\r\n");

    if(BME280_Init(BME280_OS_T_16, BME280_OS_P_16, BME280_OS_H_16,
					BME280_FILTER_16, BME280_MODE_NORMAL, BME280_TSB_05))
    	ets_uart_printf("BMP180 init error.\r\n");
    else
    	ets_uart_printf("BMP180 init done.\r\n");

	//Start the Application Task
	os_timer_disarm(&uploadTmr);
	os_timer_setfn(&uploadTmr, (os_timer_func_t *)UploadData, (void*)0);
	os_timer_arm(&uploadTmr, UPLOAD_PERIOD, 1);

	os_timer_disarm(&secTaskTmr);
	os_timer_setfn(&secTaskTmr, (os_timer_func_t *)SecTask, (void*)0);
	os_timer_arm(&secTaskTmr, SEC_TASK_PERIOD, 1);
}

static void ICACHE_FLASH_ATTR httpCallback(char * response_body, int http_status, char * full_response)
{
	if(http_status != 200)
		ets_uart_printf("Request failed!\r\nError %d!\r\n", http_status);
	else
	{
		ets_uart_printf("Request successful\r\nResponse: ");
		ets_uart_printf(response_body);
		ets_uart_printf("\r\n");
	}
}

static void ICACHE_FLASH_ATTR UploadData(void *arg)
{
	sint32 temperature;
	uint32 pressure, humidity;
	char buff[20];
	char req[100];

	if(wifi_station_get_connect_status() != STATION_GOT_IP)
		ets_uart_printf("No network connection!\r\n");
	else
	{
		if(BME280_ReadAll(&temperature, &pressure, &humidity))
			ets_uart_printf("Sensor read error!\r\n");
		else
		{
			os_sprintf(req, "http://api.thingspeak.com/update?api_key=%s&field1=%d.%02u&field2=%u.%02u&field3=%u.%02u",
						TS_KEY,
						temperature / 100, temperature % 100,							//C
						//pressure >> 8, ((pressure & 0x000000FF) * 100) >> 8,			//Pa
						(pressure >> 8) / 100, (pressure >> 8) % 100,					//hPa
						humidity >> 10, ((humidity & 0x000003FF) * 100) >> 10);			//rH
			//ets_uart_printf(req); ets_uart_printf("\r\n");
			http_get(req, "", httpCallback);
		}
	}
}

static void ICACHE_FLASH_ATTR SecTask(void *arg)
{
	WiFi_CheckConnStatus();
	//ets_uart_printf("ConnStatus: %d\r\n", wifi_station_get_connect_status());
	//ets_uart_printf("RSSI: %d\r\n", wifi_station_get_rssi());
	//ets_uart_printf("Server: %d\r\n", espconn_mdns_get_servername());
	//ets_uart_printf("Host: %d\r\n", wifi_station_get_hostname());
}
