// Load Wi-Fi library
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "DHT.h"
#include <HTTPClient.h>
#include "time.h"
#include "fonctions.h"

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN dhtPin     // Digital pin connected to the DHT sensor
#define uS_TO_S_FACTOR 1000000ULL

DHT dht(DHTPIN, DHTTYPE); // setup the sensor

// Replace with your network credentials
const char* ssid = "NETGEAR95";
const char* password = "aquaticpiano858";

//const char* ssid = "FP3";
//const char* password = "bmx0070B";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Assign output variables to GPIO pins
const int output26 = 33;
const int output27 = 4;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


//Set NTP Request Parameters
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

bool wattered = false;
bool fanState = false;
bool overTemp = false;
bool overHumid = false;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, HIGH);
  digitalWrite(output27, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  //Setup initial states of pump and fan
  pinMode(fanPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(fanPin, LOW);
  digitalWrite(pumpPin, LOW);

  //setup sensor power pin
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, HIGH);
  delay(100);
  dht.begin();



}

void loop(){
  //GET sensors information
  digitalWrite(sensorPower, HIGH);
  delay(1000);
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  int light = analogRead(lightPin);
  Serial.println(light);
  digitalWrite(sensorPower, LOW);

  //Update all the parameters
  updateParameters();

  //log the data
  sendVariables( temp, humidity, float(light));

  //check arrosage automatique
  int currentHour = getHour();
  if(currentHour == HEURE_AROSAGE and !wattered){
      Serial.println("Début de l'arosage pour ce temps :");
      Serial.println(DUREE_AROSAGE);
      
      digitalWrite(pumpPin, HIGH);
      delay(1000 * DUREE_AROSAGE);
      digitalWrite(pumpPin, LOW);
  
      Serial.println("Arosage terminé");
      wattered = true;
   }
   if(currentHour == 1 and wattered)wattered = false;
  
  //update pump status with force_pompe
  digitalWrite(pumpPin, FORCE_POMPE);

  //update LED state
  digitalWrite(whiteLed, LED);

  //update fan state
  switch(fanState){
    case true:
      if(overTemp && temp <= MIN_TEMP)overTemp = false;
      if(overHumid && humidity <= MIN_HUMIDITY)overHumid = false;
      break;
    case false:
      if(temp >= MAX_TEMP)overTemp = true;
      if(humidity >= MAX_HUMIDITY)overHumid = true;
      break;
  }
  fanState = overHumid || overTemp;
  digitalWrite(fanPin, fanState);

  //delay(DUREE_DEEPSLEEP*1000);
  //Setup sleep
  esp_sleep_enable_timer_wakeup(DUREE_DEEPSLEEP * uS_TO_S_FACTOR);
  delay(100);
  esp_light_sleep_start();
  delay(3000);
  
}
