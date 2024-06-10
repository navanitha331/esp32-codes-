#include <ArduinoOTA.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

// ESP32 pins for seven-segment display
const int segPins[] = {12, 13, 14, 15, 16, 17, 18};

AsyncWebServer server(80);

void OTAsetup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin("nani", "nani12345");
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

  // Assign segment states to the respective pins
  digitalWrite(segPins[0], a);
  digitalWrite(segPins[1], b);
  digitalWrite(segPins[2], c);
  digitalWrite(segPins[3], d);
  digitalWrite(segPins[4], e);
  digitalWrite(segPins[5], f);
  digitalWrite(segPins[6], g);
}

void setup() {
  // Set segment pins as output
  for (int a = 0; a < 7; a++) {
    pinMode(segPins[a], OUTPUT);
  }

  // Initialize OTA
  OTAsetup();

  // Route to handle incoming requests
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", "<form method='get' action='/display'><input type='number' name='digit'><input type='submit' value='Submit'></form>");
  });

  // Route to handle display requests
  server.on("/display", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasArg("digit")) {
      int digit = request->arg("digit").toInt();
      if (digit >= 0 && digit <= 9) {
        displayNumber(digit);
        request->send(200, "text/plain", "Digit set successfully");
      } else {
        request->send(400, "text/plain", "Invalid digit");
      }
    } else {
      request->send(400, "text/plain", "No digit provided");
    }
  });

  // Start server
  server.begin();
}

void loop() {
  // Handle OTA
  OTAloop();
  delay(10);  // If no custom loop code, ensure to have a delay in the loop
}

