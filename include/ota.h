#pragma once
#include <HTTPUpdate.h>

t_httpUpdate_return updateBin(const char *update_url);
void ota_update(const char* update_link);