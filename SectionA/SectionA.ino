#include "ESPNowCommA.h"

// ESP32 B MAC Address (replace with actual MAC address of Section B)
uint8_t peerAddress[6] = {0x08, 0xA6, 0xF7, 0xBD, 0x24, 0x64};

// Data instances
DemoPacketAtoB packetAtoB;
DemoPacketBtoA packetBtoA;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sent: OK" : "Sent: Fail");
}

void OnDataRecv(const esp_now_recv_info *recv_info, const uint8_t *incomingData, int len) {
  if (len == sizeof(DemoPacketBtoA)) {
    memcpy(&packetBtoA, incomingData, sizeof(packetBtoA));
    if (packetBtoA.packetType == PACKET_TYPE_B_TO_A) {
      Serial.println("Received Packet from Section B:");
      Serial.print("Demo Int: "); Serial.println(packetBtoA.demoInt);
      Serial.print("Demo Float: "); Serial.println(packetBtoA.demoFloat);
      Serial.print("Demo Char: "); Serial.println(packetBtoA.demoChar);
      Serial.print("Demo Bool: "); Serial.println(packetBtoA.demoBool ? "true" : "false");
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

void sendDemoPacketAtoB(int demoInt, float demoFloat, char demoChar, bool demoBool) {
  packetAtoB.packetType = PACKET_TYPE_A_TO_B;
  packetAtoB.demoInt = demoInt;
  packetAtoB.demoFloat = demoFloat;
  packetAtoB.demoChar = demoChar;
  packetAtoB.demoBool = demoBool;

  esp_now_send(peerAddress, (uint8_t *)&packetAtoB, sizeof(packetAtoB));
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 Section A Starting...");

  setupESPNow();
}

void loop() {
  // Check for serial input
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Read input until newline
    input.trim(); // Remove any whitespace
    
    // Example: Parse input as "int,float,char,bool" (e.g., "42,3.14,A,true")
    int demoInt;
    float demoFloat;
    char demoChar;
    bool demoBool;
    
    // Split input by commas
    int comma1 = input.indexOf(',');
    int comma2 = input.indexOf(',', comma1 + 1);
    int comma3 = input.indexOf(',', comma2 + 1);
    
    if (comma1 > 0 && comma2 > comma1 && comma3 > comma2) {
      demoInt = input.substring(0, comma1).toInt();
      demoFloat = input.substring(comma1 + 1, comma2).toFloat();
      demoChar = input.substring(comma2 + 1, comma3).charAt(0);
      String boolStr = input.substring(comma3 + 1);
      demoBool = (boolStr == "true" || boolStr == "1") ? true : false;
      
      Serial.println("Sending Demo Packet to Section B:");
      Serial.print("Demo Int: "); Serial.println(demoInt);
      Serial.print("Demo Float: "); Serial.println(demoFloat);
      Serial.print("Demo Char: "); Serial.println(demoChar);
      Serial.print("Demo Bool: "); Serial.println(demoBool ? "true" : "false");
      
      sendDemoPacketAtoB(demoInt, demoFloat, demoChar, demoBool);
    } else {
      Serial.println("Invalid input! Use format: int,float,char,bool (e.g., 42,3.14,A,true)");
    }
  }

  delay(100);
}