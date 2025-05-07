#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>

#include "KukaVar.h"

const char* name = "KUKA-Var";
IPAddress robot_ip(192,168,0,101);

WiFiManager wifi;
WiFiClient client;
KukaVar kuka(robot_ip, 7000, client);
// KUKA kuka("192.168.0.101", 7000, client);

void setup() 
{
  Serial.begin(115200);
  wifi.autoConnect(name);

  Serial.print("[KUKA]: Connecting...");
  if(kuka.connect())
    Serial.println("OK");
  else
    Serial.println("FAIL");

  ArduinoOTA.setHostname(name);
  ArduinoOTA.begin();
}

void loop() 
{
  ArduinoOTA.handle();
  
  static uint32_t last = millis();
  if(millis() - last < 1000)
    return;
  else
    last = millis();

  if(kuka.connected())
  {
    static float f = 1.1;
    uint32_t then = millis();

    if(kuka.write("VAR_NAME", String(f)))
        Serial.println("[KUKA]: Write Success!");
    else
        Serial.println("[KUKA]: Write Fail!");
    then = millis();
    Serial.print("\n[KUKA]: Write Took --> ");
    Serial.print(millis() - then);
    Serial.println("ms\n");

    String value = kuka.read("VAR_NAME");
    if(!value.startsWith("ERROR"))
        Serial.println("[KUKA]: Read Success!");   
    else
        Serial.println("[KUKA]: Read Fail!");
    then = millis();
    Serial.print("\n[KUKA]: Read Took --> ");
    Serial.print(millis() - then);
    Serial.println("ms\n");

    Serial.printf("VAR_NAME = %f", f);
    Serial.println();

    f = f + 0.1;
  }
  else
  {
    kuka.connect();
  }
}