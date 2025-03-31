#include <Arduino.h>
#include "wifi_connect_campusnet.hpp"

#define led_pin 2
void led_blink2();

void setup() {
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);
  int check = wifi_connect();
  if(check == HTTP_CODE_OK){//连接成功之后LED双闪验证
    led_blink2();
  }
}

void loop() {
  void wifi_check();
  delay(1000);
}

void led_blink2() {
  digitalWrite(led_pin, HIGH);
  delay(500);
  digitalWrite(led_pin, LOW);
  delay(500);
  digitalWrite(led_pin, HIGH);
  delay(500);
  digitalWrite(led_pin, LOW);
  delay(500);
}