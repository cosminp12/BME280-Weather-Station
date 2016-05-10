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

#include "WiFi.h"


uint8 connStatus = 0;


void WiFi_Init(void)
{
	struct station_config stationConfig;

	os_memcpy(&stationConfig.ssid, SSID, sizeof(SSID));
	os_memcpy(&stationConfig.password, PASSWORD, sizeof(PASSWORD));
	stationConfig.bssid_set = 0;

	ets_uart_printf("Default mode: %d\r\n", wifi_get_opmode_default());

	//Set the default work mode to Station if not already set
	if(wifi_get_opmode_default() != STATION_MODE)
		wifi_set_opmode(STATION_MODE);

	//Set how will the device appear on the router
	wifi_station_set_hostname(DEVICE_NAME);

	//Configure the AP to connect to
	wifi_station_set_config_current(&stationConfig);	//TODO: Read the config and use the APIs that write it to flash

	wifi_station_connect();
}

void WiFi_CheckConnStatus(void)
{
	struct ip_info DeviceIP;
	uint8 TempBuff[30];

	if(connStatus != wifi_station_get_connect_status())
	{
		connStatus = wifi_station_get_connect_status();
		switch(connStatus)
		{
		case STATION_IDLE:
			ets_uart_printf("Connection Idle!\r\n");
			break;
		case STATION_CONNECTING:
			ets_uart_printf("Connecting...\r\n");
			break;
		case STATION_WRONG_PASSWORD:
			ets_uart_printf("Wrong Password!\r\n");
			break;
		case STATION_NO_AP_FOUND:
			ets_uart_printf("No AP found\r\n");
			break;
		case STATION_CONNECT_FAIL:
			ets_uart_printf("Connection failed\r\n");
			break;
		case STATION_GOT_IP:
			ets_uart_printf("Connected!\r\n");
			wifi_get_ip_info(STATION_IF, &DeviceIP);
			ets_uart_printf("Device IP: %d.%d.%d.%d\r\n",
							(DeviceIP.ip.addr & 0xFF),
							(DeviceIP.ip.addr >> 8 & 0xFF),
							(DeviceIP.ip.addr >> 16 & 0xFF),
							(DeviceIP.ip.addr >> 24));
			break;
		}
	}
}
