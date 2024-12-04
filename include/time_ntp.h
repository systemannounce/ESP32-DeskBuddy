#pragma once

#include <Arduino.h>

// 同步网络时间
void time_utp_init(void);

// 串口打印标准时间和Unix时间
void time_get_time_all(void);

// 返回标准时间
String time_get_time_local(void);

// 返回Unix时间
long time_get_time_unix(void);