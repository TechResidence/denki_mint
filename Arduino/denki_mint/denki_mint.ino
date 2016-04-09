/*
  Deknki Mint Water Pourer
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "Constant.h"
Constant constant = Constant();

// Tweet Library for Arduino
const char* host = "maker.ifttt.com";
String url = "/trigger/denki_mint/with/key/" + constant.token;

// sensorValue actual resutls:
//   - Super dry mud: 350
//   - Super wet mud: 670
int threshold = 320;

void connect_wifi(char* ssid, char* password) {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

String payload(const char* host, String url, String comment) {
    String content_length = String(comment.length() + 7);
    Serial.println(comment);

    return String("POST ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" + 
      "Content-Length: " + content_length + "\r\n" + 
      "\r\n" +
      "value1=" + comment + "\r\n" + 
      "Connection: close\r\n\r\n";
}

void tweet(const char* host, String url, String comment) {

    delay(5000);  
    Serial.print("connecting to ");
    Serial.println(host);
    
    WiFiClientSecure client;
    const int httpsPort = 443;
    if (!client.connect(host, httpsPort)) {
      Serial.println("connection failed");
      return;
    }

    if (client.verify(constant.fingerprint, host)) {
      Serial.println("certificate matches");
    } else {
      Serial.println("certificate doesn't match");
    }
    
    // We now create a URI for the request  
    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    String content = payload(host, url, comment);
    client.print(content);
    delay(10);

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    Serial.println();
    Serial.println("closing connection");
}

void pour() {
  digitalWrite(13, HIGH); // turn the LED on (HIGH is the voltage level)  
  delay(2000); // Give water few seconds
  digitalWrite(13, LOW); // turn the LED off by making the voltage LOW
}

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW); // turn the LED off by making the voltage LOW
  connect_wifi(constant.ssid, constant.password);

  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);

  if (sensorValue < threshold) {    
    // pour water to mint
    pour();

    // tweet status
    String comment = "I need water! 土の状態: " + String(sensorValue);
    tweet(host, url, comment);
  } else {
    // tweet status
    String comment = "I'm fine :D 土の状態: " + String(sensorValue);
    tweet(host, url, comment);
  }

  Serial.println("DEEP SLEEP START!!");
  ESP.deepSleep(3600 * 1000 * 1000 , WAKE_RF_DEFAULT);
  delay(1000); //deepsleepモード移行
  Serial.println("DEEP sleeping....");

}

// the loop routine runs over and over again forever:
void loop() {
}

