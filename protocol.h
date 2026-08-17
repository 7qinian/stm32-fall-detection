#ifndef  __PROTOCOL_H
#define  __PROTOCOL_H
typedef enum {
    EVT_FALL = 1,
    EVT_HEARTBEAT = 2,
} ReportEvt_e;

/* 位置信息（模拟） */
typedef struct {
    float lat;
    float lon;
} Location_t;

void Protocol_ReportFall(const Location_t *loc);
void Protocol_ReportHeartbeat(void);
#endif
