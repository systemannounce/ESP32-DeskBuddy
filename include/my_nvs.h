#pragma once
#include <memory>
#include "nvs_flash.h"
#include "nvs.h"

void nvs_inits();

class NvsManager {
public:
    // 构造函数：初始化 NVS 命名空间
    NvsManager(const char* namespace_name);

    // 析构函数：释放资源
    ~NvsManager();

	// nvs设置存储的string键值对
	void set_string(const char* key, const char* value);

	// nvs读取设置的string键值对
	std::unique_ptr<char[]> get_string(const char* key);

    // 存储数组到 NVS
    void set_array(const char* key, const void* array, size_t size);

    // 从 NVS 获取数组，支持模板
    template <typename T>
    std::unique_ptr<T[]> get_array(const char* key);

	// 存储各种类型的变量，模板
	template<typename T>
	void set_variable(const char* key, T value);

	// 读取各种类型的变量，模板
	template <typename T>
	T get_variable(const char* key);

private:
    nvs_handle_t nvs_handle = 0; // NVS 句柄
};

// 模板实现读取各种类型数组
template <typename T>
std::unique_ptr<T[]> NvsManager::get_array(const char* key) {
    size_t required_size = 0;
	size_t count = 0;
    esp_err_t err = nvs_get_blob(nvs_handle, key, nullptr, &required_size);
    if (err != ESP_OK) {
        Serial.println("Failed to get blob size in NVS");
		return nullptr;
    }

    if (required_size % sizeof(T) != 0) {
        Serial.println("Data size mismatch in NVS");
		return nullptr;
    }

    count = required_size / sizeof(T);
    std::unique_ptr<T[]> value(new T[count]);

    err = nvs_get_blob(nvs_handle, key, value.get(), &required_size);
    if (err != ESP_OK) {
        Serial.println("Failed to get array from NVS");
		return nullptr;
    }

    return value;
}

// 模板实现存储各种类型变量
template<typename T>
void NvsManager::set_variable(const char* key, T value) {
    static_assert(sizeof(T) == 0, "Unsupported type for set_variable");
}

template<>
void NvsManager::set_variable<uint32_t>(const char* key, uint32_t value);

template<>
void NvsManager::set_variable<uint8_t>(const char* key, uint8_t value);

// 模板实现读取各种类型变量
template <typename T>
T NvsManager::get_variable(const char* key) {
    static_assert(sizeof(T) == 0, "Unsupported type for get_variable");
}

template<>
uint8_t NvsManager::get_variable<uint8_t>(const char* key);

template<>
uint32_t NvsManager::get_variable<uint32_t>(const char* key);