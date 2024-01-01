#include "Wire.h"
#include "Adafruit_ADS1X15.h"
#include "PacketSerial.h"

const byte REQUEST_START_FLAG = 0xA1;
const byte REQUEST_IS_READY = 0x01;
const byte REQUEST_RESET = 0x02;

const byte RESPONSE_START_FLAG_1 = 0xA1;
const byte RESPONSE_START_FLAG_2 = 0x1A;
const byte RESPONSE_DATA = 0x10;
const byte RESPONSE_READY = 0xFF;

const int loopTime = 20;
unsigned long previousTime = 0;

bool isReady = false;

Adafruit_ADS1015 adsLeft;
Adafruit_ADS1015 adsRight;
PacketSerial serial;

/**
 * Send the identifier response
 */
void responseIdentifier() {
  uint8_t dataResponse[4] = { 0x6C, 0x69, 0x6E, 0x65 }; // spells 'line'

  serial.send(dataResponse, sizeof(dataResponse));
}

/**
 * Send the ready response
 */
void responseReady() {
  uint8_t readyResponse[4] = {
    RESPONSE_START_FLAG_1,
    RESPONSE_START_FLAG_2,
    RESPONSE_READY,
    0x00
  };

  serial.send(readyResponse, sizeof(readyResponse));
  isReady = true;
}

void reset() {
  isReady = false;
}

/**
 * Serial packet received event handler
 * @param {uint8_t} buffer
 * @param {size_t} size
 */
void onPacketReceived(const uint8_t* buffer, size_t size) {
  byte startFlag = buffer[0];
  byte command = buffer[1];

  if (startFlag == REQUEST_START_FLAG) {
    switch (command) {
      case REQUEST_IS_READY: {
        responseReady();
        break;
      }

      case REQUEST_RESET: {
        reset();
        break;
      }
    }
  } else if (startFlag == 0xAA && command == 0xAA && buffer[2] == 0xAA && buffer[3] == 0xAA) {
    responseIdentifier();
  }
}

/**
 * Setup
 */
void setup() {
  Serial.begin(115200);

  serial.setStream(&Serial);
  serial.setPacketHandler(&onPacketReceived);

  adsLeft.begin(0x48);
  adsRight.begin(0x49);

  while (!Serial) {}

  previousTime = millis();
}

/**
 * Loop
 */
void loop() {
  serial.update();

  long now = millis();

  if (isReady && now - previousTime >= loopTime) {
    int16_t ladc0 = adsLeft.readADC_SingleEnded(0);
    int16_t ladc1 = adsLeft.readADC_SingleEnded(1);
    int16_t ladc2 = adsLeft.readADC_SingleEnded(2);
    int16_t ladc3 = adsLeft.readADC_SingleEnded(3);

    int16_t radc0 = adsRight.readADC_SingleEnded(0);
    int16_t radc1 = adsRight.readADC_SingleEnded(1);
    int16_t radc2 = adsRight.readADC_SingleEnded(2);
    int16_t radc3 = adsRight.readADC_SingleEnded(3);

    byte ladc0Part1 = ladc0 >> 8;
    byte ladc0Part2 = ladc0;
    byte ladc1Part1 = ladc1 >> 8;
    byte ladc1Part2 = ladc1;
    byte ladc2Part1 = ladc2 >> 8;
    byte ladc2Part2 = ladc2;
    byte ladc3Part1 = ladc3 >> 8;
    byte ladc3Part2 = ladc3;

    byte radc0Part1 = radc0 >> 8;
    byte radc0Part2 = radc0;
    byte radc1Part1 = radc1 >> 8;
    byte radc1Part2 = radc1;
    byte radc2Part1 = radc2 >> 8;
    byte radc2Part2 = radc2;
    byte radc3Part1 = radc3 >> 8;
    byte radc3Part2 = radc3;

    uint8_t dataResponse[20] = {
      RESPONSE_START_FLAG_1,
      RESPONSE_START_FLAG_2,
      RESPONSE_DATA,
      0x10,
      ladc0Part1,
      ladc0Part2,
      ladc1Part1,
      ladc1Part2,
      ladc2Part1,
      ladc2Part2,
      ladc3Part1,
      ladc3Part2,
      radc0Part1,
      radc0Part2,
      radc1Part1,
      radc1Part2,
      radc2Part1,
      radc2Part2,
      radc3Part1,
      radc3Part2
    };

    serial.send(dataResponse, sizeof(dataResponse));

    previousTime = now;
  }
}
