#include <Arduino.h>
#include <config.h>

void time_ntp_init()
{
	configTime(utcOffsetInSeconds, 0, ntpServer);
	while (!time(nullptr)) {
	delay(1000);
	Serial.println("Waiting for time sync...");
	}
	Serial.println("Time synced successfully");
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