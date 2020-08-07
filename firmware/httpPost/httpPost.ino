/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Nurul Kartika";
const char* password = "14101971";

// defines pins numbers
const int trigPin = 2;
const int echoPin = 5;

// defines variables
long duration;
int distance;

//Your Domain name with URL path or IP address with path
const char* serverName = "http://192.168.1.9:4000/api/post-sensor-data";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
  String sensorid = "5ef9cdadaa019958751a8306";
  int tinggiBak = 41;
  int value1 = tinggiBak - getDistance();
  httpPost(sensorid, value1);
  delay(1000);
//  float value1 = 52, value2 = 20.5, increment = 1.25;
//  for(int i = 0; i < 15; i++) {
//    value1 = value1 + increment;
//    value2 = value2 + increment;
//    httpPost(sensorid, value1, value2);
//    delay(1000);
//  }
//  for(int i = 15; i >= 0; i--) {
//    value1 = value1 - increment;
//    value2 = value2 - increment;
//    httpPost(sensorid, value1, value2);
//    delay(1000);
//  }
}

void httpPost(String sensorid, float value1) {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
      
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Data to send with HTTP POST
    String value1string = String(value1);
    String httpRequestData = "sensorid=" + sensorid + "&value1=" + value1string;           
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
        
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

int getDistance() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance= duration*0.034/2;

  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  
  return distance;
}
