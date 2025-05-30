# ESP32 ESP-NOW Demo: Bidirectional Variable Transfer

## Overview
This project demonstrates bidirectional communication between two ESP32 devices using the ESP-NOW protocol. Section A sends a packet containing four demo variables (`int`, `float`, `char`, `bool`) to Section B, which responds with modified values. The code uses a shared header file and separate Arduino sketches for each section, with Serial monitor support for input and output.

## Features
- **Bidirectional Communication**: Section A sends data to Section B, and Section B responds to Section A.
- **Data Types**: Transfers `int`, `float`, `char`, and `bool` variables.
- **Serial Input/Output**: Section A accepts input via Serial monitor (format: `int,float,char,bool`), and both sections print received data.
- **ESP-NOW**: Leverages the ESP-NOW protocol for wireless communication.

## Requirements
- Two ESP32 development boards (e.g., ESP32 DevKitC).
- Arduino IDE with ESP32 board support installed.
- Serial monitor for input (Section A) and output (both sections).

## Setup Instructions
1. **Install Arduino IDE and ESP32 Support**:
   - Install the Arduino IDE.
   - Add ESP32 board support via Boards Manager (see [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)).
2. **Clone or Download the Repository**:
   - Clone this repository or download the files.
3. **Update MAC Address**:
   - In `SectionB.ino`, replace the placeholder MAC address (`{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}`) with Section A's actual MAC address.
   - To find Section A's MAC address, upload `SectionA.ino`, open the Serial monitor (115200 baud), and note the address printed by `WiFi.macAddress()`.
4. **Upload Code**:
   - Upload `SectionA.ino` to one ESP32 (Section A).
   - Upload `SectionB.ino` to the other ESP32 (Section B).
5. **Test the Setup**:
   - Open the Serial monitor (115200 baud) for both ESP32s.
   - In Section A's Serial monitor, enter input in the format `int,float,char,bool` (e.g., `42,3.14,A,true`).
   - Observe the output in both Serial monitors to confirm data transfer.

## File Structure
- `ESPNowComm.h`: Shared header file with packet structures and function prototypes.
- `SectionA.ino`: Arduino sketch for Section A (sends and receives packets).
- `SectionB.ino`: Arduino sketch for Section B (receives and responds).
- `.gitignore`: Excludes build artifacts and IDE-specific files.

## Usage
1. **Section A**:
   - Accepts Serial input in the format `int,float,char,bool` (e.g., `42,3.14,A,true`).
   - Sends a `DemoPacketAtoB` to Section B.
   - Prints received `DemoPacketBtoA` from Section B.
2. **Section B**:
   - Receives `DemoPacketAtoB` from Section A and prints the values.
   - Sends a `DemoPacketBtoA` back to Section A with modified values (e.g., `int + 1`, `float * 2`, next `char`, toggled `bool`).
3. **Example Output**:
   - Section A input: `42,3.14,A,true`
   - Section B receives: `Demo Int: 42, Demo Float: 3.14, Demo Char: A, Demo Bool: true`
   - Section A receives: `Demo Int: 43, Demo Float: 6.28, Demo Char: B, Demo Bool: false`

## Notes
- Ensure both ESP32s are on the same Wi-Fi channel (default: channel 1).
- Replace the MAC address in `SectionB.ino` before testing.
- Invalid Serial input in Section A will trigger an error message.
- The code assumes no encryption for simplicity; modify `peerInfo.encrypt` if needed.

## License
This project is licensed under the [MIT License](LICENSE) © 2025 Md Shaifulla Zibon.

## Contributing
Feel free to open issues or submit pull requests for improvements or bug fixes.

## 🧠 Author  
**Md Shaifulla Zibon**  
Electrical and Electronics Engineering  
European University of Bangladesh  
GitHub: [@zibontaroka](https://github.com/zibontaroka)
