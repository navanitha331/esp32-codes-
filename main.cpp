#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// Can be client or even host
#ifndef STASSID
#define STASSID "nani"  // Replace with your network credentials
#define STAPSK  "nani12345"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const int segmentA = 12;
const int segmentB = 13;
const int segmentC = 14;
const int segmentD = 15;
const int segmentE = 16;
const int segmentF = 17;
const int segmentG = 18;
const int segmentDP = 23;

void OTAsetup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.begin();
}

void OTAloop() {
  ArduinoOTA.handle();
}

// Function to display a number on the 7-segment display using Boolean logic
void displayNumber(int number) {
  bool a, b, c, d, e, f, g;
  
  // Calculate each segment's state using Boolean expressions
  a = !((number != 1) && (number != 4));
  b = !((number != 5) && (number != 6));
  c = !(number != 2);
  d = !((number != 1) && (number != 4) && (number != 7));
  e = !((number == 0) || (number == 2) || (number == 6) || (number == 8));
  f = !((number != 1) && (number != 2) && (number != 3) && (number != 7));
  g = !((number != 0) && (number != 1) && (number != 7));
  
  digitalWrite(segmentA, a);
  digitalWrite(segmentB, b);
  digitalWrite(segmentC, c);
  digitalWrite(segmentD, d);
  digitalWrite(segmentE, e);
  digitalWrite(segmentF, f);
  digitalWrite(segmentG, g);
}

void setup() {
  OTAsetup();
  
  // Initialize the segment pins as outputs
  pinMode(segmentA, OUTPUT);
  pinMode(segmentB, OUTPUT);
  pinMode(segmentC, OUTPUT);
  pinMode(segmentD, OUTPUT);
  pinMode(segmentE, OUTPUT);
  pinMode(segmentF, OUTPUT);
  pinMode(segmentG, OUTPUT);
  pinMode(segmentDP, OUTPUT);

  // Display the specific number (e.g., number 5)
  displayNumber(5);
}

void loop() {
  OTAloop();
  delay(10);  // If no custom loop code ensure to have a delay in loop

  // Display the specific number (e.g., number 5)
  displayNumber(5);
}

