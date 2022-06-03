#include <Arduino.h>
#include <stdint.h>
#include "ESP8266WiFi.h"
#include "secrets.h"

WiFiClient sock;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(200u);
  WiFi.begin(secrets.ssid, secrets.pass);
}

void loop() {
  static uint8_t pin_state = 0u;
  if(WiFi.status() != WL_CONNECTED) {
    /* Flash LED until connected. */
    pin_state ^= 1u;
    delay(200u);
    digitalWrite(LED_BUILTIN, pin_state);
  } else {
    /* Once we get here means we are connected. */
    /* Leave LED on to signal change of state. */
    digitalWrite(LED_BUILTIN, 1u);

    /* Try to ping server. */
    if(sock.connected()) {
      sock.print("Connected.");
    }
    else {
      sock.connect(secrets.ip_addr, secrets.port);
    }
    delay(500);
  }
}
