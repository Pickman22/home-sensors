#ifndef SECRETS_H
#define SECRETS_H

#include "ESP8266WiFi.h"

typedef struct Secrets_tag {
  IPAddress ip_addr;
  uint16_t port;
  String ssid;
  String pass;
} Secrets;

extern Secrets secrets;

#endif // SECRETS_H
