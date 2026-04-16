#pragma once
#include <LoRa.h>

#define DEVICE_ID "A"  // Cambia a "B" en el otro módulo

void sendMessage(String msg);
void receiveMessage();