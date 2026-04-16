#include "lora_utils.h"
#include "display_utils.h"

static String lastMsgID = "";

void sendMessage(String msg) {
  String msgID = String(random(1000, 9999));
  String packet = msgID + " " + DEVICE_ID + ": " + msg;
  LoRa.beginPacket();
  LoRa.print(packet);
  LoRa.endPacket();
  LoRa.receive();
  Serial.println("TX: " + packet);
}

void receiveMessage() {
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return;

  String incoming = "";
  while (LoRa.available())
    incoming += (char)LoRa.read();

  // Extraer ID del mensaje (primeros caracteres hasta el espacio)
  String msgID = "";
  int i = 0;
  while (i < incoming.length() && incoming[i] != ' ')
    msgID += incoming[i++];

  // Ignorar si es un duplicado o mensaje propio
  if (msgID == lastMsgID) return;
  lastMsgID = msgID;

  // Ignorar si lo enviamos nosotros
  String sender = String(incoming[i+1]);  // carácter tras el espacio
  if (sender == DEVICE_ID) return;

  int rssi = LoRa.packetRssi();
  Serial.println("RX: " + incoming + "  RSSI: " + String(rssi));
  showMessage(incoming.substring(i + 1)); // muestra "A: mensaje"
}