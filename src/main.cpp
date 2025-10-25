#include <Arduino.h>
#include <Wire.h>
#include "HT_SSD1306Wire.h"
#include <SPI.h>
#include <LoRa.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "Config.h"
#include "Context.h"
#include "Board.h"
#include "htmlPages.h"

AsyncWebServer server(80);
SSD1306Wire display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);

Context Context::instance;
Context &ctx = Context::getInstance();
Board board(&ctx);

#include "Functions.hpp"

void setup()
{
  Serial.begin(115200);
  delay(1000);

  while (!Serial)
    ;

  // VextON();
  // delay(100);

  board.initNativeLed();
  ctx.deviceID = board.getIdUnique();

  display.init();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "LoRa Sender");
  display.drawLine(0, 12, 128, 12);
  display.display();
  drawProgressBarDemo(25);
  delay(1000);

  //------------------------------------------------------------------
  // LORA SETUP
  //------------------------------------------------------------------
  display.drawString(0, 16, "Starting LoRa...");
  drawProgressBarDemo(50);
  display.display();
  delay(1000);

  initLoRa();

  //------------------------------------------------------------------
  // WEB SERVER
  //------------------------------------------------------------------
  display.drawString(0, 28, "Starting WiFi AP...");
  drawProgressBarDemo(75);
  display.display();
  delay(1000);

  initWebServer();

  //------------------------------------------------------------------
  drawProgressBarDemo(100);
  display.display();
  delay(2000);
}

void loop()
{
  updateDisplay();
}
