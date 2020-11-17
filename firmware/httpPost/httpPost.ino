#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <FreeRTOS.h>


WebServer server(80);

// JSON data buffer
StaticJsonDocument<250> jsonDocument;
char buffer[250];

const char* ssid = "Watu Gong 36";
const char* password = "watugong36";

// defines pins numbers
const int trigPin = 2;
const int echoPin = 5;

const int stbyPin = 13;
const int Ain1Pin = 12; // GROUND
const int Ain2Pin = 14; // RED

// defines variables
long duration;
int distance, waterLevel = 0;
int batasAtas = 0, batasBawah = 0;
bool pumpOn = false;

//Your Domain name with URL path or IP address with path
const char* serverName = "http://192.168.0.11:4000/api/post-sensor-data";

void setup() {
  // Motor setup
  pinMode(stbyPin,OUTPUT);
  pinMode(Ain1Pin,OUTPUT);
  pinMode(Ain2Pin,OUTPUT);

  digitalWrite(stbyPin, LOW);
  digitalWrite(Ain1Pin, LOW);
  digitalWrite(Ain2Pin, HIGH);
  
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

  setupTask();
  setupRouting();
}

void setupRouting() {
  server.on("/update_batas", HTTP_POST, handlePost);

  // start server
  server.begin();
}

void handlePost() {
  if (server.hasArg("plain") == false) {
    //handle error here
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  
  // Get RGB components
  batasAtas = atoi(jsonDocument["value1"]);
  batasBawah = atoi(jsonDocument["value2"]);
  Serial.print("batas atas: ");
  Serial.println(batasAtas);
  Serial.print("batas bawah: ");
  Serial.println(batasBawah);

  Serial.println("Pesan telah sampai");
  
  // Respond to the client
  server.send(200, "application/json", "{}");
}

void postMeasurement(void *pvParameters) {
  String sensorid = "5ef9cdadaa019958751a8306";
  int tinggiBak = 41;
  for(;;) {
    int value1 = tinggiBak - getDistance();
    waterLevel = value1;
    if (value1 < 0) {
      value1 = 0;
    }
    Serial.print("Tinggi air: ");
    Serial.println(value1);
    
    int postTime = millis();
    httpPost(sensorid, value1);
    int doneTime = millis();
    postTime = doneTime - postTime;
    Serial.print("Post time: ");
    Serial.println(postTime);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
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

void refillTank(void *pvParameters) {
  for(;;) {
    if (waterLevel < batasBawah) {
      pumpOn = true;
    } else if (waterLevel >= batasAtas) {
      pumpOn = false;
    }
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void setupTask() {
  xTaskCreate(     
  postMeasurement,      
  "Post measurement data",      
  8192,      
  NULL,      
  2,     
  NULL     
  );
  xTaskCreate(     
  refillTank,      
  "Operate Water Pump",      
  4096,      
  NULL,      
  1,     
  NULL     
  ); 
}

void loop() {
  server.handleClient();
  if (pumpOn) {
    digitalWrite(trigPin,HIGH);
    digitalWrite(stbyPin,HIGH);
  } else {
    digitalWrite(trigPin,LOW);
    digitalWrite(stbyPin,LOW);
  }
}
