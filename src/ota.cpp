#include <WiFi.h>
#include <HTTPUpdate.h>
#include "config.h"

void ota_update_started()
{
	Serial.println("HTTP update process started.");
}

void ota_update_finished()
{
	Serial.println("HTTP update process finished.");
}

void ota_update_progress(int cur, int total)
{
    Serial.printf("HTTP update process at %d of %d bytes[%.1f%%]...\n", cur, total, cur * 100.0 / total);
}

void ota_update_error(int err)
{
	Serial.printf("HTTP update fatal errr code %d\n", err);
}

t_httpUpdate_return updateBin(const char *update_url)
{
    Serial.println("start update");
    WiFiClient UpdateClient;

    httpUpdate.onStart(ota_update_started);     //当升级开始时
    httpUpdate.onEnd(ota_update_finished);      //当升级结束时
    httpUpdate.onProgress(ota_update_progress); //当升级中
    httpUpdate.onError(ota_update_error);       //当升级失败时

    t_httpUpdate_return ret = httpUpdate.update(UpdateClient, update_url);

    return ret;
}

void ota_update(const char* update_link)
{
	t_httpUpdate_return ret = updateBin(update_link); //开始升级
    switch (ret)
	{
		case HTTP_UPDATE_FAILED: //当升级失败
			Serial.println("[update] Update failed.");
			break;
		case HTTP_UPDATE_NO_UPDATES: //当无升级
			Serial.println("[update] Update no Update.");
			break;
		case HTTP_UPDATE_OK: //当升级成功
			Serial.println("[update] Update ok.");
			break;
	}
}