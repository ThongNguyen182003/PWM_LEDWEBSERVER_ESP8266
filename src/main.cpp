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
void handleWebSocketMessage( void * arg, uint8_t * data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo*) arg;
  if(info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0 ;
    message = (char *)data;
    if(message.indexOf("1s") >= 0)
    {
      slidervalue1 = message.substring(2);
      dutyCyle1 = map(slidervalue1.toInt(),0,100,0,1023);
      Serial.println(dutyCyle1);
      Serial.print(getSliderValues());
      notifyClients(getSliderValues());
    }
     if(message.indexOf("2s") >= 0)
    {
      slidervalue2 = message.substring(2);
      dutyCyle2 = map(slidervalue2.toInt(),0,100,0,1023);
      Serial.println(dutyCyle2);
      Serial.print(getSliderValues());
      notifyClients(getSliderValues());
    }
     if(message.indexOf("3s") >= 0)
    {
      slidervalue1 = message.substring(2);
      dutyCyle3 = map(slidervalue3.toInt(),0,100,0,1023);
      Serial.println(dutyCyle3);
      Serial.print(getSliderValues());
      notifyClients(getSliderValues());
    }
    if(strcmp((char*)data,"getValues") == 0)
    {
      notifyClients(getSliderValues());
    }
  }
}

  void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
    }
  }

  void initWebSocket()
  {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
  }
  void setup()
  {
    Serial.begin(115200);
    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    initFS();
    initWifi();
    initWebSocket();
    // Web server root url
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS,"/index.html","text/html");
    });

    server.serveStatic("/", LittleFS, "/");

    server.begin();
  }
  void loop()
  {
    analogWrite(ledPin1, dutyCyle1);
    analogWrite(ledPin2, dutyCyle2);
    analogWrite(ledPin3, dutyCyle3);

    ws.cleanupClients();
  }
