#include <Arduino.h>
#include <config.h>

void time_utp_init()
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

String time_get_time_local()
{
	char* timeString;
	time_t now = time(nullptr);
	timeString = ctime(&now);
	return timeString;	
}

long time_get_time_unix()
{
	time_t now = time(nullptr);
	long unixTimestamp = static_cast<long>(now);  //获取unix时间戳
	return unixTimestamp;
}