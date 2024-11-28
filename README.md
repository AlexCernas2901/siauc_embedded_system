# Embedded System for SIAUC API

This embedded system is part of a project designed to manage attendance using RFID technology and WiFi connectivity. It works alongside SIAUC API to validate user identifiers (UIDs) and provide feedback on attendance events.

The system is built on an ESP32 microcontroller and utilizes an RFID reader (MFRC522) to scan RFID tags. When a tag is detected, the system reads its unique identifier (UID), sends it to the SIAUC API for validation, and processes the response to determine attendance status. Visual and auditory feedback is provided via RGB LEDs and a buzzer.

---

## How It Works

### RFID Scanning
- The system uses an RFID reader (MFRC522) to detect and read the UID of a tag presented near the reader.

### WiFi Connectivity
- The ESP32 connects to a WiFi network and communicates with the SIAUC API. If the WiFi connection is lost, the system attempts to reconnect periodically.

### API Communication
- The system sends the UID to the SIAUC API in a JSON payload via an HTTP POST request.
- The SIAUC API processes this information and returns a response indicating success or failure.
- Is nessesary to send the data to the route /attendances/check of the SIAUC API

### User Feedback
Based on the SIAUC API's response:
- **Green LED:** Indicates successful validation.
- **Red LED:** Signals a validation error.
- **Buzzer:** Emits distinct tones for success and failure events.

---

## Features

### Automated Attendance Management
- Seamless integration with an SIAUC API for tracking attendance events.

### WiFi Auto-Reconnect
- Ensures stable operation by attempting to reconnect if the network is unavailable.

### Compact and Efficient
- Designed for low-power consumption and minimal hardware requirements.

---

## Use Case
This system can be deployed in various scenarios, such as:
- Managing student or staff attendance in educational institutions.
- Automating access control in secured areas.
- Tracking presence in events or workshops.

---

## Requirements

### Hardware
- ESP32 microcontroller.
- RFID reader (MFRC522).
- RGB LEDs and buzzer for feedback.

### Software
- Arduino IDE with the following libraries:
  - `MFRC522`
  - `ArduinoJson`
  - `WiFi`
  - `HTTPClient`
