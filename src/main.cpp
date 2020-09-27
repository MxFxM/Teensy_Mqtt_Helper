#include <Arduino.h>

#include <hidden.h>

#include <mqtt.h>

Mqtt_Helper mqtt = Mqtt_Helper("ABCDEF");

void setup() {
  Serial.begin(9600);

  Serial.println("Waiting");
  delay(5000);
  Serial.println("Here we go...");

  Serial.println();
  Serial.println("Connect packet:");

  char data[255];
  int len = mqtt.ConnectAuthenticate(&data[0], user, password);
  
  if (len == -1) {
    Serial.println("Connect failed");
  } else {
    for (int i = 0; i < len; i++) {
      Serial.write(data[i]);
    }
    Serial.println();
  }

  Serial.println();
  Serial.println("Publish packet:");

  len = mqtt.Publish(&data[0], "testtopic", "hello, this is a message {json:;01}");
  
  if (len == -1) {
    Serial.println("Publish failed");
  } else {
    for (int i = 0; i < len; i++) {
      Serial.write(data[i]);
    }
    Serial.println();
  }
}

void loop() {
}