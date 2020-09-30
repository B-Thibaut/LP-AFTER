#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//IMPORTANT 

#ifndef STASSID
#define STASSID "Votre ssid"
#define STAPSK  "Votre mot de passe"
#endif

//IMPORTANT

const char* ssid = STASSID;
const char* password = STAPSK;
String valS;
int val;
String message;
ESP8266WebServer server(80);

const int led = 12;

void handleRoot() {
  message = "<b1>Luminositex</b><br><br><form action='/' method='get'><input type='range' name='led' min='0' max='255'><br><input type='submit' value='envoi'><br><br>";
  for (uint8_t i = 0; i < server.args(); i++) {  
    if( server.argName(i) == "led"){
        valS = server.arg(i);
        val = valS.toInt();
        analogWrite(led, val);
        message += server.arg(i);
        Serial.println("Mon est entré dans le if");
    }
    else{
      analogWrite(led, 0);
      message += "Premier lancement !";
      Serial.println("on est entré dans le else");
    }
  }
  server.send(200, "text/html", message);
}

void handleNotFound() {
  //digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    if( server.argName(i) == "led" && server.arg(i) == "1"){
        digitalWrite(led, 1);
        message += "<br>on est dans le if";
    }
    else{
      digitalWrite(led, 0);
      message += "<br>on est dans le else";
    }
  }
  message += "<br><a href='/test?led=0'>on allume</a>";
  message += "<br><a href='/test?led=1'>on eteint</a>";
  server.send(404, "text/html", message);
  //digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/gif", []() {
    static const uint8_t gif[] PROGMEM = {
      0x47, 0x49, 0x46, 0x38, 0x37, 0x61, 0x10, 0x00, 0x10, 0x00, 0x80, 0x01,
      0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x10, 0x00, 0x00, 0x02, 0x19, 0x8c, 0x8f, 0xa9, 0xcb, 0x9d,
      0x00, 0x5f, 0x74, 0xb4, 0x56, 0xb0, 0xb0, 0xd2, 0xf2, 0x35, 0x1e, 0x4c,
      0x0c, 0x24, 0x5a, 0xe6, 0x89, 0xa6, 0x4d, 0x01, 0x00, 0x3b
    };
    char gif_colored[sizeof(gif)];
    memcpy_P(gif_colored, gif, sizeof(gif));
    // Set the background to a random set of colors
    gif_colored[16] = millis() % 256;
    gif_colored[17] = millis() % 256;
    gif_colored[18] = millis() % 256;
    server.send(200, "image/gif", gif_colored, sizeof(gif_colored));
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
