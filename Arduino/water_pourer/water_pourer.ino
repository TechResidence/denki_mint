/*
  Deknki Mint Water Pourer
*/

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
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
  }
  
  delay(1); // delay in between reads for stability
}
