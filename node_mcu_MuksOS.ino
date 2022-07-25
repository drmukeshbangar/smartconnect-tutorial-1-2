/*
 * AUTHOR: DR. MUKESH BANGAR
 *
 * Created on: 12.07.2022
 *
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <SocketIOclient.h>
#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;

const char* router_ssid = "enter here router ssid";
const char* router_password = "enter here router password";
const char* socket_ip = "enter here socket server ip";
int port = 5000;

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);     
   
    if(WiFi.getMode() & WIFI_AP) {
        WiFi.softAPdisconnect(true);
    }

    WiFiMulti.addAP(router_ssid, router_password);
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    String ip = WiFi.localIP().toString();
    Serial.println("Connected to :" + ip);

    // server address, port and URL
    socketIO.begin(socket_ip, port, "/socket.io/?EIO=4");

    // event handler
    socketIO.onEvent(socketIOEvent);
    digitalWrite(LED_BUILTIN, HIGH);
}

void sendMsg(String msg){
  // creat JSON message for Socket.IO (event)
        DynamicJsonDocument doc(1024);
        JsonArray array = doc.to<JsonArray>();
        array.add("from_esp");
        array.add(msg);
        String output;
        serializeJson(doc, output);

        // Send event
        socketIO.sendEVENT(output);

        // Print JSON for debugging
        Serial.println("sent msg: " + output);
  };
  
 void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case sIOtype_DISCONNECT:
            Serial.printf("[IOc] Disconnected!\n");
            break;
        case sIOtype_CONNECT:
            Serial.printf("[IOc] Connected to url: %s\n", payload);
            break;
        case sIOtype_EVENT:
            Serial.printf("[IOc] get event: %s\n", payload);
            String str = (char*)payload;
            str.trim();
            String output = str.substring(9, 11);       
           
            if(output == "01" ){
               digitalWrite(LED_BUILTIN, LOW); 
                 Serial.println("light on");
                 sendMsg("speak sir light is turning on");
             };
              
            if(output == "00"){   
               digitalWrite(LED_BUILTIN, HIGH);
               Serial.println("light off"); 
               sendMsg("speak sir light is turning off");
              };
              
            Serial.println(output);
            break;
    }
}

void loop() {
    socketIO.loop();
}
