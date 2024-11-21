#include <nvs_flash.h>
#include <Arduino.h>

nvs_handle_t my_nvs_handle;


void nvs_inits(const char* list)
{
	esp_err_t error = nvs_flash_init();
	if (error == ESP_OK)
	{
		Serial.println("NVS已经成功初始化!");
		esp_err_t error_open = nvs_open(list, NVS_READWRITE, &my_nvs_handle);
		if (error_open == ESP_OK)
		{
			Serial.println("NVS已经成功打开命名空间!");
			if (my_nvs_handle)
			{
				Serial.println("句柄返回成功");
			}
			else
			{
				Serial.println("句柄返回失败，未知原因。");
			}
		}
		else
		{
			Serial.println("NVS打开命名空间失败!!");
		}
	}
	else
	{
		Serial.println(error);
		Serial.println("NVS初始化失败!!");
	}
}

void nvs_set_string(const char* key, const char* value)
{
	esp_err_t err = nvs_set_str(my_nvs_handle, key, value);
	if (err == ESP_OK) Serial.println("NVS字符设置成功");
	nvs_commit(my_nvs_handle);
}

void nvs_get_string(const char* key)
{
	size_t required_size;
	nvs_get_str(my_nvs_handle, key, NULL, &required_size);
	char* server_name = (char*)malloc(required_size);
	nvs_get_str(my_nvs_handle, key, server_name, &required_size);
	Serial.println("nvs读取结果: ");
	Serial.println(server_name);
}