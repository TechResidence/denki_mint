/*
  Deknki Mint Water Pourer
*/

#include <ESP8266WiFi.h>

const char* ssid     = "xxxxxxxx";
const char* password = "xxxxxxxx";

const char* host = "arduino-tweet.appspot.com";
String url = "/update";
char msg[] = "I need water!";
char token[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
String contentlen = String(strlen(msg) + strlen(token) + 14);

void setup() {

  // initialize serial communication at 9600 bits per second:
  //Serial.begin(9600);
  Serial.begin(115200);
  pinMode(13, OUTPUT);

  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  
  // print out the value you read:
  Serial.println(sensorValue);

  // sensorValue actual resutls:
  //   - Super dry mud: 350
  //   - Super wet mud: 670
  if (sensorValue > 600) {
    digitalWrite(13, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(10000); // Wait few seconds
    
  } else {
    digitalWrite(13, LOW); // turn the LED off by making the voltage LOW
    delay(2000); // Give water few seconds
    digitalWrite(13, HIGH); // turn the LED on (HIGH is the voltage level)
    
    // =============================================
    // Tweeeeeeeeeeeeeeeeeeeeeeeeeeeeeet
    // 
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
    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Content-Length: " + contentlen + "\r\n" + 
                 "\r\n" + 
                 "token=" + token +
                 "&status=" + msg + "\r\n" + 
                 "Connection: close\r\n\r\n");
  
    delay(10);
    
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    
    Serial.println();
    Serial.println("closing connection");
  
    // Tweet Finish!
    // =============================================  
  }
  
  delay(1); // delay in between reads for stability
  delay(10000); // Wait few seconds
}

