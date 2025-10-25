#include "Board.h"

Board::Board(Context *ctx)
{
    this->_ctx = ctx;
}

void Board::initSerial(unsigned long baudrate)
{
    Serial.begin(baudrate);
}

void Board::initNativeLed()
{
    pinMode(LED_NATIVE, OUTPUT);
    digitalWrite(LED_NATIVE, LOW);
}

String Board::getIdUnique()
{
    uint64_t macAddress = ESP.getEfuseMac();
    uint32_t chip = (uint32_t)(macAddress >> 16);

    char idUnique[7];
    snprintf(idUnique, sizeof(idUnique), "%06X", chip);
    return "ESP32LORA" + (String)idUnique;
}

void Board::restartDevice(int waitTime)
{
    delay(waitTime);
    ESP.restart();
}

float Board::getFreeRam()
{
    uint32_t freeMem = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    uint32_t totalMem = heap_caps_get_total_size(MALLOC_CAP_8BIT);

    return ((float)freeMem / totalMem) * 100.0;
}

float Board::getUsedRam()
{
    uint32_t freeMem = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    uint32_t totalMem = heap_caps_get_total_size(MALLOC_CAP_8BIT);

    return ((float)(totalMem - freeMem) / totalMem) * 100.0;
}

unsigned long Board::getUptimeInMinutes()
{
    return (millis() / 60000);
}

unsigned long Board::getUptimeInSeconds()
{
    return (millis() / 1000);
}

float Board::getCPUTemperature()
{
    float _temperature = ((temprature_sens_read() - 32) / 1.8);
    if (isnan(_temperature) || _temperature <= 0)
        _temperature = 0;

    return _temperature;
}

void Board::updateInfo()
{
    _ctx->cpuTemperature = this->getCPUTemperature();
    _ctx->uptimeInMinutes = this->getUptimeInMinutes();
    _ctx->usedRam = this->getUsedRam();
}
