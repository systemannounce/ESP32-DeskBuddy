#include <nvs_flash.h>
#include <Arduino.h>
#include "my_nvs.h"

void nvs_inits()
{
    // 初始化 NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
	}
}

NvsManager::NvsManager(const char* namespace_name)
{
    esp_err_t err = nvs_open(namespace_name, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        Serial.println("Failed to open NVS namespace");
    }
}

NvsManager::~NvsManager()
{
    if (nvs_handle != 0) {
        nvs_close(nvs_handle);
    }
	Serial.println("NVS namespace closed");
}

void NvsManager::set_string(const char* key, const char* value)
{
    esp_err_t err = nvs_set_str(nvs_handle, key, value);
    if (err != ESP_OK) {
        Serial.println("Failed to set array in NVS");
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        Serial.println("Failed to commit changes in NVS");
    }
}

std::unique_ptr<char[]> NvsManager::get_string(const char* key)
{
	size_t required_size;
	esp_err_t err = nvs_get_str(nvs_handle, key, NULL, &required_size);
	if (err != ESP_OK)
	{
		Serial.println("nvs_get_str_length failed");
		return nullptr;
	}
	std::unique_ptr<char[]> value(new char[required_size]);
	err = nvs_get_str(nvs_handle, key, value.get(), &required_size);
	if (err != ESP_OK)
	{
		Serial.println("nvs_get_str failed");
		return nullptr;
	}
	return value;
}


void NvsManager::set_array(const char* key, const void* array, size_t size)
{
    esp_err_t err = nvs_set_blob(nvs_handle, key, array, size);
    if (err != ESP_OK) {
        Serial.println("Failed to set array in NVS");
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        Serial.println("Failed to commit changes in NVS");
    }
}

// 特化模板区
template<>
void NvsManager::set_variable<uint32_t>(const char* key, uint32_t value) {
    esp_err_t err = nvs_set_u32(nvs_handle, key, value);
    if (err != ESP_OK) {
        Serial.println("Failed to set uint8_t variable from NVS");
    }
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        Serial.println("Failed to commit changes in NVS");
    }
}

template<>
void NvsManager::set_variable<uint8_t>(const char* key, uint8_t value) {
    esp_err_t err = nvs_set_u8(nvs_handle, key, value);
    if (err != ESP_OK) {
        Serial.println("Failed to set uint8_t variable from NVS");
    }
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        Serial.println("Failed to commit changes in NVS");
    }
}

template<>
uint8_t NvsManager::get_variable<uint8_t>(const char* key) {
	uint8_t value;
    esp_err_t err = nvs_get_u8(nvs_handle, key, &value);
    if (err != ESP_OK) {
        Serial.println("Failed to get uint8_t variable from NVS");
		return 0;
    }
	return value;
}

template<>
uint32_t NvsManager::get_variable<uint32_t>(const char* key) {
	uint32_t value;
    esp_err_t err = nvs_get_u32(nvs_handle, key, &value);
    if (err != ESP_OK) {
		Serial.println(err);
        Serial.println("Failed to get uint32_t variable from NVS");
		return 0;
    }
	return value;
}