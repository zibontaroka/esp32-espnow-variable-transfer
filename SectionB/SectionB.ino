#include "ESPNowCommB.h"

// ESP32 A MAC Address (replace with actual MAC address of Section A)
uint8_t peerAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Data instances
DemoPacketAtoB packetAtoB;
DemoPacketBtoA packetBtoA;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sent: OK" : "Sent: Fail");
}

void OnDataRecv(const esp_now_recv_info *recv_info, const uint8_t *incomingData, int len) {
  if (len == sizeof(DemoPacketAtoB)) {
    memcpy(&packetAtoB, incomingData, sizeof(packetAtoB));
    if (packetAtoB.packetType == PACKET_TYPE_A_TO_B) {
      Serial.println("Received Packet from Section A:");
      Serial.print("Demo Int: "); Serial.println(packetAtoB.demoInt);
      Serial.print("Demo Float: "); Serial.println(packetAtoB.demoFloat);
      Serial.print("Demo Char: "); Serial.println(packetAtoB.demoChar);
      Serial.print("Demo Bool: "); Serial.println(packetAtoB.demoBool ? "true" : "false");
      
      // Respond with a demo packet back to Section A
      int responseInt = packetAtoB.demoInt + 1; // Example modification
      float responseFloat = packetAtoB.demoFloat * 2.0;
      char responseChar = packetAtoB.demoChar + 1; // Next character
      bool responseBool = !packetAtoB.demoBool; // Toggle boolean
      
      sendDemoPacketBtoA(responseInt, responseFloat, responseChar, responseBool);
    }
  }
}

void setupESPNow() {
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(1);
  WiFi.begin();
  delay(500);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    while (1);
  }

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peerAddress, 6);
  peerInfo.channel = 1;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Peer add failed");
    while (1);
  }
}

void sendDemoPacketBtoA(int demoInt, float demoFloat, char demoChar, bool demoBool) {
  packetBtoA.packetType = PACKET_TYPE_B_TO_A;
  packetBtoA.demoInt = demoInt;
  packetBtoA.demoFloat = demoFloat;
  packetBtoA.demoChar = demoChar;
  packetBtoA.demoBool = demoBool;

  esp_now_send(peerAddress, (uint8_t *)&packetBtoA, sizeof(packetBtoA));
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 Section B Starting...");

  setupESPNow();
}

void loop() {
  // Section B primarily responds to received packets
  delay(100);
}