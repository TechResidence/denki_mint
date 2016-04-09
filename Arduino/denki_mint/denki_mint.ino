/*
  Deknki Mint Water Pourer
*/

#include <ESP8266WiFi.h>
#include "Constant.h"
Constant constant = Constant();

// Tweet Library for Arduino
const char* host = "arduino-tweet.appspot.com";
String url = "/update";

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

String payload(const char* host, String url, String token, String comment) {
    String content_length = String(token.length() + comment.length() + 14);
    Serial.println(comment);

    return String("POST ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" + 
      "Content-Length: " + content_length + "\r\n" + 
      "\r\n" + 
      "token=" + token +
      "&status=" + comment + "\r\n" + 
      "Connection: close\r\n\r\n";
}

void tweet(const char* host, String url, String token, String comment) {

    delay(5000);  
    Serial.print("connecting to ");
    Serial.println(host);
    
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }
    
    // We now create a URI for the request  
    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    String content = payload(host, url, token, comment);
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
    tweet(host, url, constant.token, comment);
  } else {
    // tweet status
    String comment = "I'm fine :D 土の状態: " + String(sensorValue);
    tweet(host, url, constant.token, comment);
  }

  //DEEP SLEEPモード突入命令
  Serial.println("DEEP SLEEP START!!");

  //1:μ秒での復帰までのタイマー時間設定  2:復帰するきっかけの設定（モード設定）
  ESP.deepSleep(3600 * 1000 * 1000 , WAKE_RF_DEFAULT);

  //deepsleepモード移行までのダミー命令
  delay(1000);

  //実際にはこの行は実行されない
  Serial.println("DEEP SLEEPing....");

  // delay(1); // delay in between reads for stability
  // delay(10000); // Wait few seconds for mint drinking water
}

// the loop routine runs over and over again forever:
void loop() {
}

