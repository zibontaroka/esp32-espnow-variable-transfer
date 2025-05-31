#include <esp_now.h>
#include <WiFi.h>

// ESP32 B MAC Address
uint8_t peerAddress[] = {0x08, 0xA6, 0xF7, 0xBD, 0x24, 0x64};

// Data structure
typedef struct struct_message {
  int counter;
  float temperature;
  char status;
} struct_message;

struct_message myData;
struct_message receivedData;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sent: OK" : "Sent: Fail");
}

void OnDataRecv(const esp_now_recv_info *recv_info, const uint8_t *incomingData, int len) {
  if (len == sizeof(receivedData)) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    Serial.print("From B: Counter=");
    Serial.print(receivedData.counter);
    Serial.print(", Temp=");
    Serial.print(receivedData.temperature);
    Serial.print(", Status=");
    Serial.println(receivedData.status);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
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

void loop() {
  myData.counter++;
  myData.temperature = random(2000, 3500) / 100.0;
  myData.status = (myData.counter % 2 == 0) ? 'A' : 'B';

  Serial.print("To B: Counter=");
  Serial.print(myData.counter);
  Serial.print(", Temp=");
  Serial.print(myData.temperature);
  Serial.print(", Status=");
  Serial.println(myData.status);

  esp_now_send(peerAddress, (uint8_t *) &myData, sizeof(myData));
  delay(2000);
}