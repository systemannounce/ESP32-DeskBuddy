#include <Arduino.h>
#include <config.h>
#include "main.h"

bool time_ntp_init(uint32_t timeout_ms)
{
    uint32_t start_time_ms = millis(); // 记录开始时间
	Serial.print("Waiting for time sync");
	configTime(utcOffsetInSeconds, 0, ntpServer);
	while (time(nullptr) < develop_time) {
        if (millis() - start_time_ms >= timeout_ms)
        {
            Serial.println("\nTime Sync timeout!");
            return false; // 超时
        }
		delay(100);
        Serial.print(".");
	}
	Serial.println("");
	Serial.println("Time synced successfully");
	return true;
}

void time_get_time_all()
{
	
	time_t now = time(nullptr);
	Serial.print("Current time is: ");
	Serial.println(ctime(&now));  //打印时间
	// Convert current time to Unix timestamp
	long unixTimestamp = static_cast<long>(now);  //获取unix时间戳 
	Serial.print("Unix timestamp is: ");
	Serial.println(unixTimestamp);
}

const char* time_get_time_local(const char* format)
{
    static char buffer[80];
    time_t now = time(nullptr);
    struct tm timeinfo;

    if (localtime_r(&now, &timeinfo)) {
        strftime(buffer, sizeof(buffer), format, &timeinfo);
        return buffer;
    }

    return "Error";  // 返回空指针以指示错误
}

long time_get_time_unix()
{
	time_t now = time(nullptr);
	long unixTimestamp = static_cast<long>(now);  //获取unix时间戳
	return unixTimestamp;
}

const char* unix_to_str(unsigned long seconds) {
    static char buffer[32];  // 静态缓冲区，用于存储格式化后的字符串

    if (seconds >= 3600) {
        // 转换为小时和分钟
        unsigned int hours = seconds / 3600;
        unsigned int minutes = (seconds % 3600) / 60;
        snprintf(buffer, sizeof(buffer), "%uh%um", hours, minutes);
    } else if (seconds >= 60) {
        // 转换为分钟
        unsigned int minutes = seconds / 60;
        snprintf(buffer, sizeof(buffer), "%um", minutes);
    } else {
        // 直接使用秒数
        snprintf(buffer, sizeof(buffer), "%lus", seconds);
    }

    return buffer;  // 返回格式化后的字符串
}