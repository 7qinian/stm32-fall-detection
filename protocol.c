#include "stm32f10x.h"                  // Device header
#include "bsp_uart.h"
#include <stdio.h> 
typedef enum {
    EVT_FALL = 1,
    EVT_HEARTBEAT = 2,
} ReportEvt_e;

/* 位置信息（模拟） */
typedef struct {
    float lat;
    float lon;
} Location_t;

static const char *DEV_ID = "BRACELET_001";
void Protocol_ReportFall(const Location_t *loc)
{
	printf("55555\r\n");
	char buf[128];
	snprintf(buf,sizeof(buf),
		"{\"dev\":\"%s\",\"evt\":%d,\"lat\":%.4f,\"lon\":%.4f}\r\n",
		DEV_ID,
		EVT_FALL,
		loc ? loc->lat : 0.0f,
        loc ? loc->lon : 0.0f);
	BSP_UART_SendString(buf);
}
void Protocol_ReportHeartbeat(void)
{
	char buf[128];
	snprintf(buf,sizeof(buf),
		 "{\"dev\":\"%s\",\"evt\":%d}\r\n",
		DEV_ID,
		EVT_HEARTBEAT);
	BSP_UART_SendString(buf);
}
