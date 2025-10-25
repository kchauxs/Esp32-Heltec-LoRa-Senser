#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <Arduino.h>

//----------------------------------------------
// DEVICE CONFIG
//----------------------------------------------
#define SERIAL_DEBUG !false
//----------------------------------------------
#if SERIAL_DEBUG
#define DEBUG_PRINT(msg) Serial.print(msg)
#define DEBUG_PRINTLN(msg) Serial.println(msg)
#else
#define DEBUG_PRINT(msg)
#define DEBUG_PRINTLN(msg)
#endif

#define LED_NATIVE LED_BUILTIN

// ----------------------------------------------
// loRa
// ----------------------------------------------
#define LORA_SS 18
#define LORA_RST 14
#define LORA_DIO0 26
#define LORA_BAND 433E6 // Frecuencia 433 MHz

//----------------------------------------------
// WiFi AP
//----------------------------------------------
#define DEFAULT_WIFI_AP_SSID "LoRa-Sender-Server"
#define DEFAULT_WIFI_AP_PASSWORD "12345678"

#endif // __CONFIG_H__