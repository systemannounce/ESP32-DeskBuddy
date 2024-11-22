#pragma once

// nvs初始化
void nvs_inits(const char* list);

// nvs设置存储的string键值对
void nvs_set_string(const char* key, const char* value);

// nvs读取设置的string键值对
void nvs_get_string(const char* key);