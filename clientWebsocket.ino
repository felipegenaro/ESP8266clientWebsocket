#include <Arduino.h>
#include <Thread.h>
#include <StaticThreadController.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <Hash.h>
// #include <WiFiManager.h>

// WiFiManager wifiManager;

WebSocketsClient webSocket;
#define PIN_BUTTON D2
String owner="Teste1";
int buttonState=0;

//configuracao de rede
const char* ssid     = "<NOME DA REDE>";
const char* password = "<SENHA DA REDE>";

//set custom ip
// wifiManager.setSTAStaticIPConfig(IPAddress(192,168,0,99), IPAddress(192,168,0,1), IPAddress(255,255,255,0));

#define USE_SERIAL Serial

//THREADS
Thread threadwebSocket = Thread();
Thread threadRead = Thread();
StaticThreadController<2> controll(&threadwebSocket,&threadRead);
 
boolean power;
 
void webSocketRun(){
    webSocket.loop();
  }
void webSocketEvent(WStype_t type, uint8_t * payload, size_t lenght) {
    webSocket.sendTXT("owner= "+ owner + " MAC: " + WiFi.macAddress());
}

void setup() {
    USE_SERIAL.begin(115200);
    USE_SERIAL.println();
 
    WiFi.begin(ssid, password);
 
    while(WiFi.status() != WL_CONNECTED) {
        delay(100);
    }
    
    pinMode(PIN_BUTTON, INPUT);
    
    //ADDING THREADS
    threadwebSocket.onRun(webSocketRun);
    threadwebSocket.setInterval(1);
     
    webSocket.begin("<IP DO SERVIDOR>", 81);
    webSocket.onEvent(webSocketEvent);
}
 
void loop() {
  controll.run();

  //envio de informacoes
  webSocket.sendTXT("(nao e a primeira msg)owner= "+ owner + " MAC: " + WiFi.macAddress());
  delay(50);
}
