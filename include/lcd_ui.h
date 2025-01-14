#pragma once


void ui_interface_home();

void ui_interface_close();

void ui_interface_ble();

void ui_interface_wlan();

void ui_interface_info();

void ui_interface_ota(const char* title = "Error", const char* code = "Please check.", const char* process = "");
