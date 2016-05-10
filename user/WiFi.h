#ifndef USER_WIFI_H_
#define USER_WIFI_H_


#include <osapi.h>
#include <os_type.h>
#include <c_types.h>
#include <user_interface.h>
#include "user_config.h"
#include "uart.h"


void WiFi_Init(void);
void WiFi_CheckConnStatus(void);


#endif /* USER_WIFI_H_ */
