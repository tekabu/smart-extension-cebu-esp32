#include <Arduino.h>
#include <HardwareSerial.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <cstdlib> 
#include "page.h" // Ensure this file is compatible with ESP32

#define RXPin 16
#define TXPin 17

const char* ssid = "Smart-Outlet";
const char* password = "Password123";
String str;
float voltage1, voltage2, current1, current2, power1, power2, energy1, energy2, temperature1, temperature2;
int s_voltage1, s_voltage2, s_current1, s_current2, s_power1, s_power2, s_energy1, s_energy2, s_temperature1, s_temperature2;

HardwareSerial SerialPort(1);

WebServer server(80);
DNSServer dnsServer;

void handleRoot() {
  server.send(200, "text/html", htmlPage1);
}

void handlePage2() {
  String s = htmlPage2;
  s.replace("@VOLTAGE1", String(s_voltage1));
  s.replace("@CURRENT1", String(s_current1));
  s.replace("@POWER1", String(s_power1));
  s.replace("@ENERGY1", String(s_energy1));
  s.replace("@TEMPERATURE1", String(s_temperature1));
  s.replace("@VOLTAGE2", String(s_voltage2));
  s.replace("@CURRENT2", String(s_current2));
  s.replace("@POWER2", String(s_power2));
  s.replace("@ENERGY2", String(s_energy2));
  s.replace("@TEMPERATURE2", String(s_temperature2));
  server.send(200, "text/html", s);
}

void handleCaptivePortal() {
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", ""); // Redirect to root
}

void saveSettings(int index) {
  double x_voltage = 1;
  double x_current = 1;
  double x_power = 1;
  double x_energy = 1;
  double x_temperature = 1;

  if (server.hasArg("voltage")) {
    x_voltage = server.arg("voltage").toDouble();
  }

  if (server.hasArg("current")) {
    x_current = server.arg("current").toDouble();
  }

  if (server.hasArg("power")) {
    x_power = server.arg("power").toDouble();
  }

  if (server.hasArg("energy")) {
    x_energy = server.arg("energy").toDouble();
  }

  if (server.hasArg("temperature")) {
    x_temperature = server.arg("temperature").toDouble();
  }

  if (index == 1)
  {
    s_voltage1 = x_voltage;
    s_current1 = x_current;
    s_power1 = x_power;
    s_energy1 = x_energy;
    s_temperature1 = x_temperature;
  }
  else
  {
    s_voltage2 = x_voltage;
    s_current2 = x_current;
    s_power2 = x_power;
    s_energy2 = x_energy;
    s_temperature2 = x_temperature;
  }

  SerialPort.print("$");
  SerialPort.println(index);
  SerialPort.print(",");
  SerialPort.println(x_voltage);
  SerialPort.print(",");
  SerialPort.println(x_current);
  SerialPort.print(",");
  SerialPort.println(x_power);
  SerialPort.print(",");
  SerialPort.println(x_energy);
  SerialPort.print(",");
  SerialPort.print(x_temperature);
  SerialPort.println("#");

  Serial.print("Socket: ");
  Serial.println(index);
  Serial.print(", Voltage: ");
  Serial.println(x_voltage);
  Serial.print(", Current: ");
  Serial.println(x_current);
  Serial.print(", Power: ");
  Serial.println(x_power);
  Serial.print(", Energy: ");
  Serial.println(x_energy);
  Serial.print(", Temperature: ");
  Serial.println(x_temperature);

  server.sendHeader("Location", "/settings");
  server.send(303);
}

void handleSocket1Settings() {
  saveSettings(1);
}

void handleSocket2Settings() {
  saveSettings(2);
}

void handleData() {
  JsonDocument doc;
  doc["voltage1"] = voltage1;
  doc["voltage2"] = voltage2;
  doc["current1"] = current1;
  doc["current2"] = current2;
  doc["power1"] = power1;
  doc["power2"] = power2;
  doc["energy1"] = energy1;
  doc["energy2"] = energy2;
  doc["temperature1"] = temperature1;
  doc["temperature2"] = temperature2;

  String jsonString;
  serializeJson(doc, jsonString);

  server.send(200, "application/json", jsonString);
}

void handleDataTest() {
  JsonDocument doc;

  // Generate random data for testing
  doc["voltage1"] = random(110, 130) + random(0, 99) / 100.0; // Random voltage 110-130V
  doc["voltage2"] = random(220, 240) + random(0, 99) / 100.0; // Random voltage 220-240V
  doc["current1"] = random(10, 20) / 10.0 + random(0, 99) / 100.0; // Random current 1-2A
  doc["current2"] = random(5, 15) / 10.0 + random(0, 99) / 100.0; // Random current 0.5-1.5A
  doc["power1"] = doc["voltage1"].as<float>() * doc["current1"].as<float>();
  doc["power2"] = doc["voltage2"].as<float>() * doc["current2"].as<float>();
  doc["energy1"] = random(1, 10) + random(0, 99) / 100.0; // Random energy 1-10 kWh
  doc["energy2"] = random(1, 10) + random(0, 99) / 100.0; // Random energy 1-10 kWh
  doc["temperature1"] = random(20, 40) + random(0, 99) / 100.0; // Random temperature 20-40°C
  doc["temperature2"] = random(20, 40) + random(0, 99) / 100.0; // Random temperature 20-40°C

  String jsonString;
  serializeJson(doc, jsonString);

  server.send(200, "application/json", jsonString);
}

void parseData() {
  int expectedCount = 11;
  int valueCount = 0;
  float values[expectedCount];
  int commaIndex = 0;
  int lastCommaIndex = -1;

  while (commaIndex != -1 && valueCount < expectedCount) {
    commaIndex = str.indexOf(',', lastCommaIndex + 1);
    if (commaIndex == -1) {
      // Last value
      values[valueCount] = str.substring(lastCommaIndex + 1).toFloat();
    } else {
      values[valueCount] = str.substring(lastCommaIndex + 1, commaIndex).toFloat();
    }
    lastCommaIndex = commaIndex;
    valueCount++;
  }

  // Check for correct number of values
  if (valueCount != expectedCount) {
    Serial.println("Error: Incorrect data format");
    return;
  }

  int vtype = values[10];
  Serial.print("V-Type: "); Serial.println(vtype);

  if (vtype == 1)
  {
    voltage1 = values[0];
    voltage2 = values[1];
    current1 = values[2];
    current2 = values[3];
    power1 = values[4];
    power2 = values[5];
    energy1 = values[6];
    energy2 = values[7];
    temperature1 = values[8];
    temperature2 = values[9];

    Serial.print("Voltage1: "); Serial.println(voltage1);
    Serial.print("Voltage2: "); Serial.println(voltage2);
    Serial.print("Current1: "); Serial.println(current1);
    Serial.print("Current2: "); Serial.println(current2);
    Serial.print("Power1: "); Serial.println(power1);
    Serial.print("Power2: "); Serial.println(power2);
    Serial.print("Energy1: "); Serial.println(energy1);
    Serial.print("Energy2: "); Serial.println(energy2);
    Serial.print("Temperature1: "); Serial.println(temperature1);
    Serial.print("Temperature2: "); Serial.println(temperature2);
  }
  else
  {
    s_voltage1 = values[0];
    if (s_voltage1 < 1) {
        s_voltage1 = 1;
    } else if (s_voltage1 > 255) {
        s_voltage1 = 255;
    }

    s_voltage2 = values[1];
    if (s_voltage2 < 1) {
        s_voltage2 = 1;
    } else if (s_voltage2 > 255) {
        s_voltage2 = 255;
    }

    s_current1 = values[2];
    if (s_current1 < 1) {
        s_current1 = 1;
    } else if (s_current1 > 255) {
        s_current1 = 255;
    }

    s_current2 = values[3];
    if (s_current2 < 1) {
        s_current2 = 1;
    } else if (s_current2 > 255) {
        s_current2 = 255;
    }

    s_power1 = values[4];
    if (s_power1 < 1) {
        s_power1 = 1;
    } else if (s_power1 > 255) {
        s_power1 = 255;
    }

    s_power2 = values[5];
    if (s_power2 < 1) {
        s_power2 = 1;
    } else if (s_power2 > 255) {
        s_power2 = 255;
    }

    s_energy1 = values[6];
    if (s_energy1 < 1) {
        s_energy1 = 1;
    } else if (s_energy1 > 255) {
        s_energy1 = 255;
    }

    s_energy2 = values[7];
    if (s_energy2 < 1) {
        s_energy2 = 1;
    } else if (s_energy2 > 255) {
        s_energy2 = 255;
    }

    s_temperature1 = values[8];
    if (s_temperature1 < 1) {
        s_temperature1 = 1;
    } else if (s_temperature1 > 255) {
        s_temperature1 = 255;
    }

    s_temperature2 = values[9];
    if (s_temperature2 < 1) {
        s_temperature2 = 1;
    } else if (s_temperature2 > 255) {
        s_temperature2 = 255;
    }

    Serial.print("Voltage1: "); Serial.println(s_voltage1);
    Serial.print("Voltage2: "); Serial.println(s_voltage2);
    Serial.print("Current1: "); Serial.println(s_current1);
    Serial.print("Current2: "); Serial.println(s_current2);
    Serial.print("Power1: "); Serial.println(s_power1);
    Serial.print("Power2: "); Serial.println(s_power2);
    Serial.print("Energy1: "); Serial.println(s_energy1);
    Serial.print("Energy2: "); Serial.println(s_energy2);
    Serial.print("Temperature1: "); Serial.println(s_temperature1);
    Serial.print("Temperature2: "); Serial.println(s_temperature2);
  }
}

void setup() {
  Serial.begin(4800);
  delay(10);
  SerialPort.begin(4800, SERIAL_8N1, RXPin, TXPin);
  delay(10);

  Serial.println();
  Serial.println("Configuring access point...");

  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  dnsServer.start(53, "*", myIP);

  server.on("/", handleRoot);
  server.on("/settings", handlePage2);
  server.on("/generate_204", handleCaptivePortal); //for newer android versions.
  server.on("/fwlink", handleCaptivePortal); //for windows
  server.on("/ncsi.txt", handleCaptivePortal); //for windows
  server.on("/connecttest.txt", handleCaptivePortal); //for windows
  server.on("/hotspot-detect.html", handleCaptivePortal); //for android
  server.on("/connectivitycheck.gstatic.com", handleCaptivePortal); //for android
  server.on("/data", HTTP_GET, handleData); 
  server.on("/settings1", handleSocket1Settings);
  server.on("/settings2", handleSocket2Settings);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();

  while (SerialPort.available()) {
    char c = SerialPort.read();
    if (c == '$') {
      str = "";
    } else if (c == '#') {
      parseData();
      str = "";
    } else {
      str += String(c);
    }
  }
}