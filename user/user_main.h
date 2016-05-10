#ifndef USER_USER_MAIN_H_
#define USER_USER_MAIN_H_

#include <osapi.h>
#include <os_type.h>
#include <c_types.h>
#include <user_interface.h>
#include "user_config.h"
#include "uart.h"
#include "I2C.h"
#include "BME280.h"
#include "WiFi.h"
#include "espconn.h"

#define UPLOAD_PERIOD		60000		//ms
#define SEC_TASK_PERIOD		1000		//ms

static void ICACHE_FLASH_ATTR UploadData(void *arg);
static void ICACHE_FLASH_ATTR SecTask	(void *arg);

#endif /* USER_USER_MAIN_H_ */
