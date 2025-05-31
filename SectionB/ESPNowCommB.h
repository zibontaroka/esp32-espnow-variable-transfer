#ifndef ESPNOW_COMMB_H
#define ESPNOW_COMMB_H

#include <esp_now.h>
#include <WiFi.h>

// Define packet types
#define PACKET_TYPE_A_TO_B 1
#define PACKET_TYPE_B_TO_A 2

// Structure for Demo Packet (Section A -> Section B)
struct DemoPacketAtoB {
  uint8_t packetType;   // Packet type (1 = A to B)
  int demoInt;          // Integer value
  float demoFloat;      // Floating-point value
  char demoChar;        // Character value
  bool demoBool;        // Boolean value
};

// Structure for Demo Packet (Section B -> Section A)
struct DemoPacketBtoA {
  uint8_t packetType;   // Packet type (2 = B to A)
  int demoInt;          // Integer value
  float demoFloat;      // Floating-point value
  char demoChar;        // Character value
  bool demoBool;        // Boolean value
};

// Global variables
extern DemoPacketAtoB packetAtoB;
extern DemoPacketBtoA packetBtoA;

// Function prototypes
void setupESPNow();
void sendDemoPacketBtoA(int demoInt, float demoFloat, char demoChar, bool demoBool);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const esp_now_recv_info *recv_info, const uint8_t *incomingData, int len);

#endif