#pragma once

#include <Arduino.h>

// 同步网络时间
void time_ntp_init(void);

// 串口打印标准时间和Unix时间
void time_get_time_all(void);

/**************************************
常用的时间格式化符号：
%Y：四位年份。
%m：两位月份（01-12）
%d：两位日期（01-31）
%H：两位小时（00-23）
%M：两位分钟（00-59）
%S：两位秒（00-59）
例如："%Y/%m/%d"
***************************************/
const char* time_get_time_local(const char* format);

// 返回Unix时间
long time_get_time_unix(void);