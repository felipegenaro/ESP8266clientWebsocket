#include <Arduino.h>
#include <Thread.h>
#include <StaticThreadController.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <Hash.h>
// #include <WiFiManager.h>

// WiFiManager wifiManager;

//#define MAX_SRV_CLIENTS 1
//
//WiFiServer server(23);
//WiFiClient serverClients[MAX_SRV_CLIENTS];

WebSocketsClient webSocket;
#define PIN_BUTTON D2
String owner="Teste2";
int buttonState=0;

//configuracao de rede
const char* ssid     = "WIFI_EDUC_126";
const char* password = "educ126634#";

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
    webSocket.sendTXT("Owner= "+ owner + ", MAC= " + WiFi.macAddress());

    if(payload[0]=='s'){
        digitalWrite(PIN_BUTTON, !digitalRead(PIN_BUTTON));
//digitalWrite(LED_BUILTIN, LOW);
//delay(500);
//digitalWrite(LED_BUILTIN, HIGH);
          
//        ESP.reset();
    }
}

void setup() {
    USE_SERIAL.begin(115200);
    USE_SERIAL.println();
 
    WiFi.begin(ssid, password);
 
    while(WiFi.status() != WL_CONNECTED) {
        delay(100);
    }
    
    pinMode(PIN_BUTTON, OUTPUT);
    
    //thread de conexao
    threadwebSocket.onRun(webSocketRun);
    threadwebSocket.setInterval(1);

    //ip e porta do servidor
    webSocket.begin("10.84.44.13", 81);
    webSocket.onEvent(webSocketEvent);

    WiFi.setSleepMode(WIFI_NONE_SLEEP);

//    //start UART and the server
//  Serial.begin(115200);
//  server.begin();
//  server.setNoDelay(true);
//
//  Serial1.print("Ready! Use 'telnet ");
//  Serial1.print(WiFi.localIP());
//  Serial1.println(" 23' to connect");

//    delay(1000);
}
 
void loop() {
  controll.run();

  //envio de informacoes
  webSocket.sendTXT("Owner= "+ owner + ", someData= aloaloalo");
  delay(1000);
  webSocket.sendTXT("Owner= "+ owner + ", someData= aloaloalo");
  delay(1000);
  webSocket.sendTXT("overLoad");
  delay(1000);


//  uint8_t i;
//  //check if there are any new clients
//  if (server.hasClient()) {
//    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
//      //find free/disconnected spot
//      if (!serverClients[i] || !serverClients[i].connected()) {
//        if (serverClients[i]) {
//          serverClients[i].stop();
//        }
//        serverClients[i] = server.available();
//        Serial1.print("New client: "); Serial1.print(i);
//        break;
//      }
//    }
//    //no free/disconnected spot so reject
//    if (i == MAX_SRV_CLIENTS) {
//      WiFiClient serverClient = server.available();
//      serverClient.stop();
//      Serial1.println("Connection rejected ");
//    }
//  }
//  //check clients for data
//  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
//    if (serverClients[i] && serverClients[i].connected()) {
//      if (serverClients[i].available()) {
//        //get data from the telnet client and push it to the UART
//        while (serverClients[i].available()) {
//          Serial.write(serverClients[i].read());
//        }
//      }
//    }
//  }
//  //check UART for data
//  if (Serial.available()) {
//    size_t len = Serial.available();
//    uint8_t sbuf[len];
//    Serial.readBytes(sbuf, len);
//    //push UART data to all connected telnet clients
//    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
//      if (serverClients[i] && serverClients[i].connected()) {
//        serverClients[i].write(sbuf, len);
//        delay(1);
//      }
//    }
//  }
  
}

