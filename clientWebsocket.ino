#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <Hash.h>
#include <Thread.h>
#include <StaticThreadController.h>
// #include <WiFiManager.h>

WebSocketsClient webSocket;
// WiFiManager wifiManager;

String owner = "MQ1";
byte mac[6];

//configuracao de rede
const char* ssid     = "<NETWORK>";
const char* password = "<PASSWORD>";

//set custom ip
// wifiManager.setSTAStaticIPConfig(IPAddress(192,168,0,99), IPAddress(192,168,0,1), IPAddress(255,255,255,0));

//definicao dos pinos
#define PINO_LED D2

#define USE_SERIAL Serial

//THREADS
Thread threadwebSocket = Thread();
Thread threadRead = Thread();
StaticThreadController<2> controll(&threadwebSocket,&threadRead);
 
void webSocketRun() {
    webSocket.loop();
}
void webSocketEvent(WStype_t type, uint8_t * payload, size_t lenght) {
    WiFi.macAddress(mac);
    int cMac = mac[5]+mac[4]+mac[3]+mac[2]+mac[1]+mac[0];
  
    webSocket.sendTXT("Owner= "+ owner + ", MAC= " + mac[5]+":"+mac[4]+":"+mac[3]+":"+mac[2]+":"+mac[1]+":"+mac[0]);

    //target commands
    if (payload[0] == cMac) {
        digitalWrite(PINO_LED, !digitalRead(PINO_LED));
        // ESP.reset();
    }
}

void setup() {
    USE_SERIAL.begin(115200);
    USE_SERIAL.println();
 
    WiFi.begin(ssid, password);
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }
    
    pinMode(PINO_LED, OUTPUT);
    
    //thread de conexao
    threadwebSocket.onRun(webSocketRun);
    threadwebSocket.setInterval(1);

    //ip e porta do servidor
    webSocket.begin("<HOST>", 81);
    webSocket.onEvent(webSocketEvent);

    WiFi.setSleepMode(WIFI_NONE_SLEEP);
}
 
void loop() {
  controll.run();

  //envio de informacoes
  webSocket.sendTXT("Owner= "+ owner + ", Data= tmpOp"+"temp"+"nvOil...");
  delay(1000); 
}
