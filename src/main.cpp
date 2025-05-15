#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include "bogus_data.h"

const char* ssid = "#######"; // Your network SSID (name)
const char* password = "#######"; // Your network password

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Battery monitoring pins
const int batteryVoltagePin = 34;         // (BLUE Wire) Analog pin to read battery voltage
const int currentSensePin = 35;           // (GREEN Wire) Analog pin to read current for resistance calculation
const int loadControlPin = 26;            // (ORANGE Wire) Digital pin to control load for tests
const int chargingControlPin = 27;        // Digital pin to control charging

const float ADC_REF_VOLTAGE = 3.3;        // ESP32 ADC reference voltage
const float ADC_RESOLUTION = 4095.0;      // 12-bit ADC (0-4095)

const float R1 = 100000.0;                // 100kΩ                - Can be any resistor, this is just a placeholder
const float R2 = 33000.0;                 // 33kΩ                 - Can be any resistor, this is just a placeholder
const float R_SHUNT = 0.1;                // 0.1Ω shunt resistor  - This should be as low as possible
// const float VOLTAGE_DIVIDER_RATIO = 2.0;  // For two 10kΩ resistors (1/2 ratio)


/* Filler Values to test without connection*/
float batteryVoltage = 3.7;
float internalResistance = 50.0;
float minVoltage = 3.5;
float maxVoltage = 4.2;
float minResistance = 45.0;
float maxResistance = 55.0;
float overallHealth = 95.0;
float capacityRetention = 90.0;
float powerCapability = 85.0;
float cellTemp = 30.0;
float estCapacity = 2200.0;
float cycleCount = 300.0;
float selfDischargeRate = 2.0;

void notifyClients() {
  String json = "{\"voltage\":" + String(batteryVoltage, 2) +
                ",\"minVoltage\":" + String(minVoltage, 2) +
                ",\"maxVoltage\":" + String(maxVoltage, 2) +
                ",\"resistance\":" + String(internalResistance, 2) +
                ",\"minResistance\":" + String(minResistance, 2) +
                ",\"maxResistance\":" + String(maxResistance, 2) +
                ",\"overallHealth\":" + String(overallHealth) +
                ",\"capacityRetention\":" + String(capacityRetention) +
                ",\"powerCapability\":" + String(powerCapability) +
                ",\"cellTemp\":" + String(cellTemp, 1) +
                ",\"estCapacity\":" + String(estCapacity) +
                ",\"cycleCount\":" + String(cycleCount) +
                ",\"selfDischargeRate\":" + String(selfDischargeRate, 1) + "}";
  ws.textAll(json);
}

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                      AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("WebSocket client connected");
  }
}

// Function prototypes
float readBatteryVoltage();
float readInternalResistance();

void setup() {
  Serial.begin(115200);
  pinMode(loadControlPin, OUTPUT);
  pinMode(chargingControlPin, OUTPUT);
  pinMode(batteryVoltagePin, INPUT);
  pinMode(currentSensePin, INPUT);
  
  // Set outputs to LOW initially
  digitalWrite(loadControlPin, LOW);
  digitalWrite(chargingControlPin, LOW);
  
  // Initialize voltage reading
  batteryVoltage = readBatteryVoltage();
  internalResistance = readInternalResistance();

  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.begin();
}

float readBatteryVoltage() {
  int rawValue = 0;
  for (int i = 0; i < 10; i++) {
    rawValue += analogRead(batteryVoltagePin);
    delay(5);
  }
  rawValue /= 10;  // Average of 10 readings
  
  // Convert ADC value to actual voltage
  float voltage = (rawValue / ADC_RESOLUTION) * ADC_REF_VOLTAGE * ((R1 + R2) / R2);
  
  return voltage;
}

// Function to read internal resistance
float readInternalResistance() {
  // Read voltage before applying load
  float voltageBeforeLoad = readBatteryVoltage();
  
  // Apply load
  digitalWrite(loadControlPin, HIGH);
  delay(100); // Wait for stabilization
  
  // Read voltage and current under load
  float voltageUnderLoad = readBatteryVoltage();
  
  // Read current (through voltage drop across sense resistor)
  int rawCurrentValue = analogRead(currentSensePin);
  float voltageDrop = (rawCurrentValue / ADC_RESOLUTION) * ADC_REF_VOLTAGE;
  
  // Calculate current (I = V/R)
  float current = voltageDrop / R_SHUNT;
  
  // Remove load
  digitalWrite(loadControlPin, LOW);
  
  // Calculate internal resistance (R = ΔV/I)
  float voltageChange = voltageBeforeLoad - voltageUnderLoad;
  
  // Prevent division by zero or meaningless readings
  if (current < 0.01 || voltageChange < 0.01) {
    return internalResistance; // Return last valid reading
  }
  
  return (voltageChange / current) * 1000.0; // Convert to milliohms
}

#define CHARGING 0
#define DISCHARGING 1

bool state = CHARGING; // Initial state charging
uint8_t table_index = 0;

void loop() {
  /*LOTS OF PLACEHOLDERS, ONLY TO SHOW UPDATING IN REAL-TIME*/

  if (state == CHARGING) {
    batteryVoltage = battery_charge_voltage[table_index];
    internalResistance = battery_charge_resistance[table_index];
    cellTemp = battery_charge_temperature[table_index];
  } else { // DISCHARGING
    batteryVoltage = battery_discharge_voltage[table_index];
    internalResistance = battery_discharge_resistance[table_index];
    cellTemp = battery_discharge_temperature[table_index];
  }
  table_index = (table_index + 1) % PROFILE_SIZE; // increment through the profile array then wrap around

  if (batteryVoltage < minVoltage) {
    minVoltage = batteryVoltage;
  }
  if (batteryVoltage > maxVoltage) {
    maxVoltage = batteryVoltage;
  }
  if (internalResistance < minResistance) {
    minResistance = internalResistance;
  }
  if (internalResistance > maxResistance) {
    maxResistance = internalResistance;
  }

  overallHealth = get_battery_health(cycleCount);
  capacityRetention = get_capacity_retention(cycleCount);
  powerCapability = get_power_capability(cycleCount);
  estCapacity = get_estimated_capacity(cycleCount);
  selfDischargeRate = get_self_discharge_rate(cycleCount);
  if (state == DISCHARGING && table_index == 49) cycleCount++;

  //batteryVoltage = readBatteryVoltage();
  //internalResistance = readInternalResistance();
  //minVoltage += 0.1;
  //maxVoltage += 0.1;
  //minResistance += 0.1;
  //maxResistance += 0.1;
  //overallHealth += 0.1;
  //capacityRetention += 0.1;
  //powerCapability += 0.1;
  //cellTemp += 0.1;
  //estCapacity += 0.1;
  //cycleCount += 1;
  //selfDischargeRate += 0.1;
  notifyClients();
  delay(4800); // a charging/discharging cycle should take 8 mins
}
