#include <Arduino.h>
#include<ESP8266WiFi.h>
#include<ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include<ArduinoJson.h>
#include<Arduino_JSON.h>

const char* ssid = "Redmi";
const char* password = "hoangyenhi";

// Create async server port 80 ;

AsyncWebServer server(80);
// create websocket
AsyncWebSocket ws("/ws");
// set led GPIO
const int ledPin1 = 14;
const int ledPin2 = 12;
const int ledPin3 = 13;

String message = "";
String slidervalue1 = "0";
String slidervalue2 = "0";
String slidervalue3 = "0";

int dutyCyle1 ;
int dutyCyle2 ;
int dutyCyle3 ;
JSONVar sliderValues;
// JSON valiable to hold slider value

//get slider values
String getSliderValues()
{
  sliderValues["slidervalue1"] = String(slidervalue1);
  sliderValues["slidervalue2"] = String(slidervalue2);
  sliderValues["slidervalue3"] = String(slidervalue3);

  String jsonString = JSON.stringify(sliderValues);
  return jsonString;
}

void initFS()
{
  if(!LittleFS.begin())
  {
    Serial.println("Loi khi cai dat Little FS");
  }
  else
  {
    Serial.println("LittleFS thanh cong");
  }
}
//Khoi tao wifi
void initWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.print("Connecting wifi ...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP() );
}

void notifyClients(String sliderValues)
{
  ws.textAll(sliderValues);
}
