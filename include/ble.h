#pragma once

#include <Arduino.h>
// #include <BLEDevice.h>
// #include <BLEServer.h>
#include <BLEUtils.h>
// #include <BLE2902.h>


extern BLEServer *pServer;
extern BLECharacteristic *pTxCharacteristic;
extern bool deviceConnected;
extern bool oldDeviceConnected;
extern uint8_t txValue;
extern String macAddress;


//String chipId;
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


class MyServerCallbacks: public BLEServerCallbacks {
	public:
		void onConnect(BLEServer* pServer) override;
		void onDisconnect(BLEServer* pServer) override;
};


extern String resStr;
extern String chipId;
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) override;
};

// ble初始化
void ble_init();

// ble重新连接
void ble_reconnect();