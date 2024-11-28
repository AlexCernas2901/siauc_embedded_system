#include <MFRC522.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const uint8_t 
  PINS_RGB[2] = {26, 33}, // Pines RGB
  SS_PIN = 5,             // GPIO 5
  RST_PIN = 27,           // GPIO 27
  BUZZER = 17;            // GPIO 17

bool 
  wifi_status = false,      // Estado actual de la conexión WiFi
  last_wifi_status = false; // Estado anterior de la conexión WiFi

uint64_t 
  millis_last_connection_attempt_time = 0,
  millis_current_time = 0;

const char* SSID = "";           // Nombre de la red
const char* PASSWORD = ""; // Contraseña de la red

const String API_URL = ""; // URL de la API

String 
  read_uid = "", 
  json_uid = "";

MFRC522 mfrc522(SS_PIN, RST_PIN);
StaticJsonDocument<18> uid_doc;

void setup(void) {
  Serial.begin(115200); 
  while (!Serial);
  SPI.begin(); 
  mfrc522.PCD_Init(); 
  connectToWiFi();
  initializeRGBPins();
  turnOffRGB();
}

void loop(void) {
  millis_current_time = millis();

  // Verificar el estado de la conexión WiFi
  wifi_status = (WiFi.status() == WL_CONNECTED);
  
  // Solo imprimir si hay un cambio de estado
  if (wifi_status != last_wifi_status) {
    if (wifi_status) {
      Serial.println("WiFi conectado exitosamente.");
    } else {
      Serial.println("WiFi desconectado, intentando reconecta.");
    }
    last_wifi_status = wifi_status; // Actualizar el estado anterior
  }

  // Intentar reconexión WiFi cada 5 segundos si no está conectado
  if (!wifi_status && (millis_current_time - millis_last_connection_attempt_time) > 5000) {
    connectToWiFi();
    millis_last_connection_attempt_time = millis_current_time;
    return;
  }

  if (!wifi_status) return;

  // Verificar si hay una nueva tarjeta presente
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return; 

  // Leer el UID
  for (byte i = 0; i < mfrc522.uid.size; i++) 
    read_uid += String(mfrc522.uid.uidByte[i], HEX);

  Serial.println("UID de la tarjeta: " + read_uid);

  // Realizar la petición HTTP
  HTTPClient http;
  http.begin(API_URL + "/attendances/check");
  http.addHeader("Content-Type", "application/json");

  uid_doc["uid"] = read_uid; 
  serializeJson(uid_doc, json_uid);

  int httpResponseCode = http.POST(json_uid);
  String responseBody = http.getString();

  // Restablecer UID después de usarlo
  read_uid = "";

  Serial.println(httpResponseCode);
  Serial.println(responseBody);

  if (httpResponseCode != 200) {
    turnOnRGB(0);
    tone(BUZZER, 500, 200);  
  } else {
    turnOnRGB(1); 
    tone(BUZZER, 5000, 400);  
  } 

  http.end();

  delay(2000);
  turnOffRGB();
}

void connectToWiFi(void) {
  WiFi.begin(SSID, PASSWORD);
}

void turnOnRGB (uint8_t position) {
  for (uint8_t i = 0; i <= (sizeof(PINS_RGB) / sizeof(PINS_RGB[0])); i++) 
    digitalWrite(PINS_RGB[i], (i == position) ? HIGH : LOW);
}

void turnOffRGB(void) {
  for (uint8_t i = 0; i <= (sizeof(PINS_RGB) / sizeof(PINS_RGB[0])); i++) 
    digitalWrite(PINS_RGB[i], LOW);
}

void initializeRGBPins(void) {
  for (uint8_t i = 0; i <= (sizeof(PINS_RGB) / sizeof(PINS_RGB[0])); i++) 
    pinMode(PINS_RGB[i], OUTPUT);
}
