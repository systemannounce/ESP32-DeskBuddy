/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE"
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second.
*/
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "ble.h"
#include "main.h"

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;
String macAddress = "INIT BLE...";


void MyServerCallbacks::onConnect(BLEServer* pServer) {
	deviceConnected = true;
};

void MyServerCallbacks::onDisconnect(BLEServer* pServer) {
	deviceConnected = false;
}


String resStr;
String chipId;
void MyCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
	std::string rxValue = pCharacteristic->getValue();

	if (rxValue.length() > 0) {
	Serial.println("*********");
	Serial.print("Received Value: ");
	for (int i = 0; i < rxValue.length(); i++){
		Serial.print(rxValue[i]);
		resStr += rxValue[i];
	}
	Serial.println();
	Serial.println("*********");

	// 做什么
	ble_do_once_what();
	resStr = "";
	}
}

void ble_init()
{
	// chipId 芯片唯一标识符
	chipId = String((uint32_t)ESP.getEfuseMac(), HEX);
	chipId.toUpperCase();
	Serial.println(chipId);

	// Create the BLE Device
	BLEDevice::init("Desk Buddy");	// 蓝牙名称

	// 获取 BLE MAC 地址
    macAddress = BLEDevice::getAddress().toString().c_str();
    Serial.println("BLE Device MAC Address: " + macAddress);

	// Create the BLE Server
	pServer = BLEDevice::createServer();
	pServer->setCallbacks(new MyServerCallbacks());
	// Create the BLE Service
	BLEService *pService = pServer->createService(SERVICE_UUID);
	// Create a BLE Characteristic
	pTxCharacteristic = pService->createCharacteristic(
							CHARACTERISTIC_UUID_TX,
							BLECharacteristic::PROPERTY_NOTIFY
						);

	pTxCharacteristic->addDescriptor(new BLE2902());

	BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
		CHARACTERISTIC_UUID_RX,
		BLECharacteristic::PROPERTY_WRITE
											);

	pRxCharacteristic->setCallbacks(new MyCallbacks());

	// Start the service
	pService->start();

	// Start advertising
	pServer->getAdvertising()->start();
	Serial.println("Waiting a client connection to notify...");
}

void ble_reconnect()
{
	// disconnecting
	if (!deviceConnected && oldDeviceConnected) {
		delay(500); // give the bluetooth stack the chance to get things ready
		pServer->startAdvertising(); // restart advertising
		Serial.println("start advertising");
		oldDeviceConnected = deviceConnected;
	}
  // connecting
	if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
		oldDeviceConnected = deviceConnected;
	}
}
