#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include "display_utils.h"
#include "lora_utils.h"

#define LORA_SCK   5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_SS   18
#define LORA_RST  23
#define LORA_DIO0 26
#define LORA_FREQ  868E6

void setup() {
  Serial.begin(9600);

  if (!initDisplay()) {
    Serial.println("SSD1306 failed");
    while (1);
  }

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  LoRa.setSpreadingFactor(9);
  LoRa.setSignalBandwidth(250E3);
  LoRa.setCodingRate4(6);

  if (!LoRa.begin(LORA_FREQ)) {
    showMessage("LoRa FALLO");
    while (1);
  }

  LoRa.receive();
  showMessage("868 MHz\nEsperando...");
  Serial.println("LoRa listo @ 868 MHz - ID: " DEVICE_ID);
}

void loop() {
  receiveMessage();

  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();
    if (msg.length() > 0)
      sendMessage(msg);
  }
}