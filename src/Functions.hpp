#include <ArduinoJson.h>

void VextON(void)
{
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW);
}

void drawProgressBarDemo(int progress)
{
    if (progress < 0)
        progress = 0;

    if (progress > 100)
        progress = 100;

    display.drawProgressBar(0, 52, 120, 10, progress);
}

void updateDisplay()
{
    static unsigned long lastDisplayUpdate = 0;

    if (millis() - lastDisplayUpdate > 1000)
    {
        lastDisplayUpdate = millis();

        display.clear();
        display.setFont(ArialMT_Plain_10);
        display.setTextAlignment(TEXT_ALIGN_LEFT);

        display.drawString(0, 0, "LoRa Sender");
        display.drawLine(0, 12, 128, 12);

        display.drawString(0, 14, "SSID: " + String(DEFAULT_WIFI_AP_SSID));
        display.drawString(0, 24, "IP: " + WiFi.softAPIP().toString());
        display.drawString(0, 34, "ID: " + String(ctx.deviceID));
        display.drawString(0, 44, "Web Clients: " + String(WiFi.softAPgetStationNum()));
        display.drawString(0, 54, "TX: " + String(ctx.sentPackets) + "  SF9  BW125");
        display.display();
    }
}

void handlePostData(AsyncWebServerRequest *request, uint8_t *data, size_t len)
{
    digitalWrite(LED_BUILTIN, HIGH);
    String jsonString;
    for (size_t i = 0; i < len; i++)
        jsonString += (char)data[i];

    DEBUG_PRINTLN("📩 Datos recibidos:");
    DEBUG_PRINTLN(jsonString);
    Serial.println("-----------------------------");

    JsonDocument jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, jsonString);

    if (error)
    {
        DEBUG_PRINTLN("❌ Error deserialized JSON: " + String(error.c_str()));
        request->send(400, "application/json", "{\"status\":\"fail\"}");
        return;
    }

    // Extraer campos del JSON
    // String ts = String(jsonDoc["ts"].as<long>());
    String dt = jsonDoc["dt"].as<String>();
    String tm = jsonDoc["tm"].as<String>();
    String ent = jsonDoc["ent"].as<String>();
    String mat = jsonDoc["mat"].as<String>();
    String dest = jsonDoc["dest"].as<String>();
    String orig = jsonDoc["orig"].as<String>();
    String qty = String(jsonDoc["qty"].as<float>(), 2);
    String val = String(jsonDoc["val"].as<long>());
    String drv = jsonDoc["drv"].as<String>();
    String pl = jsonDoc["pl"].as<String>();
    String lp = jsonDoc["lp"].as<String>();
    String mach = jsonDoc["mach"].as<String>();
    String op = jsonDoc["op"].as<String>();
    String obs = jsonDoc["obs"].as<String>();

    String csvData = dt + "," + tm + "," + ent + "," + mat + "," + dest + "," + orig + "," +
                     qty + "," + val + "," + drv + "," + pl + "," + lp + "," +
                     mach + "," + op + "," + obs + "," + ctx.deviceID;
                     
    // String csvData = ts + ","+ dt + "," + tm + "," + ent + "," + mat + "," + dest + "," + orig + "," +
    //                  qty + "," + val + "," + drv + "," + pl + "," + lp + "," +
    //                  mach + "," + op + "," + obs + "," + ctx.deviceID;

    DEBUG_PRINTLN("📤 Enviando datos por LoRa (formato CSV):");
    DEBUG_PRINTLN(csvData);

    LoRa.beginPacket();
    LoRa.print(csvData);
    LoRa.endPacket();

    request->send(200, "application/json", "{\"status\":\"ok\"}");
    ctx.sentPackets++;

    DEBUG_PRINTLN("✅ Datos enviados por LoRa correctamente.");
    Serial.println("=============================");
    digitalWrite(LED_BUILTIN, LOW);
}

void initLoRa()
{
    DEBUG_PRINTLN("\nIniciando emisor LoRa...");
    LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
    if (!LoRa.begin(LORA_BAND))
    {
        DEBUG_PRINTLN("❌ Error inicializando LoRa!");
        while (1)
            ;
    }
    DEBUG_PRINTLN("✅ LoRa inicializado correctamente.");

    LoRa.setSpreadingFactor(9);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);
    LoRa.enableCrc();
}

void initWebServer()
{
    DEBUG_PRINTLN("\nIniciando servidor web...");
    WiFi.softAP(DEFAULT_WIFI_AP_SSID, DEFAULT_WIFI_AP_PASSWORD);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", htmlForm); });

    server.on("/submit", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
              { handlePostData(request, data, len); });

    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->send(404, "text/html", html404); });

    server.begin();
    DEBUG_PRINTLN("✅ Servidor web iniciado correctamente.");
}
