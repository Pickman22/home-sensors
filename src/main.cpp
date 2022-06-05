#include <Arduino.h>
#include <TaskScheduler.h>
#include <stdint.h>
#include "ESP8266WiFi.h"
#include "secrets.h"
#include "dbg.h"

WiFiClient sock;
Scheduler sch;

#define CONN_MAN_TS (200u)
#define CONN_MAN_ERR_RETRY (10u)
void tsk_conn_man_fcn(void);
Task tsk_conn_man(CONN_MAN_TS, TASK_FOREVER, &tsk_conn_man_fcn);

void setup() {
  DBG_INIT();
  pinMode(LED_BUILTIN, OUTPUT);
  sch.addTask(tsk_conn_man);
  tsk_conn_man.enable();
}

void loop() {
  sch.execute();
}

void tsk_conn_man_fcn(void) {

  static enum ConnState {
    NotConnected = 0,
    Connecting,
    Connected,
    Error,
    Die
  } _conn_state = NotConnected;

  switch(_conn_state) {
    static uint8_t err_retry_count = 0u;

    case NotConnected:
      WiFi.mode(WIFI_STA);
      WiFi.disconnect();
      WiFi.begin(secrets.ssid, secrets.pass);
      digitalWrite(LED_BUILTIN, false);
      _conn_state = Connecting;
      DBG_INFO("Connecting to:  %s", secrets.ssid.c_str());
      break;

    case Connecting:
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

      switch(WiFi.status()) {
        case WL_CONNECT_FAILED:
          DBG_ERR("Connection failed. Retrying...");
          _conn_state = NotConnected;
          break;

        case WL_CONNECTION_LOST:
          DBG_WARN("Connection lost. Reconnecting...");
          _conn_state = NotConnected;
          break;

        case WL_CONNECTED:
          DBG_INFO("Connected. IP: %s, RSSI: %ddBm.",
              WiFi.localIP().toString().c_str(), WiFi.RSSI());
          WiFi.setAutoReconnect(true);
          _conn_state = Connected;
          break;

        case WL_NO_SSID_AVAIL:
          DBG_INFO("SSID %s not available.", secrets.ssid.c_str());
          _conn_state = Connecting;
          break;

        case WL_WRONG_PASSWORD:
          DBG_ERR("Wrong password.");
          _conn_state = Error;
          break;

        case WL_DISCONNECTED:
          DBG_DEBUG("Connecting...");
          _conn_state = Connecting;
          break;

        default:
          DBG_WARN("Unhandled state.");
          _conn_state = Error;
          break;
      }
      break;

    case Connected:
      digitalWrite(LED_BUILTIN, true);
      if(WiFi.status() != WL_CONNECTED) {
        _conn_state = Connecting;
      } else {
        _conn_state = Connected;
      }
      break;

    case Error:
      if(++err_retry_count > CONN_MAN_ERR_RETRY) {
        DBG_ERR("Connection manager error. Will not recover!");
        _conn_state = Die;
        tsk_conn_man.setInterval(1000u);
        tsk_conn_man.enable();
      } else {
        DBG_ERR("Connection manager error. Retrying: %d!", err_retry_count);
        _conn_state = Connecting;
      }
      break;

    case Die:
      DBG_ERR("RIP...");
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      DBG_DEBUG("WiFi status: %d.", WiFi.status());
      break;
  }
}
