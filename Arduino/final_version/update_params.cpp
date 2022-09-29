#include <ArduinoJson.h>

// Load Wi-Fi library
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "DHT.h"
#include <HTTPClient.h>
#include "time.h"
#include "fonctions.h"



void updateParameters(){
  
  String serverName = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?read=1";
  HTTPClient http;
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  http.begin(serverName);
  int httpCode = http.GET();
  Serial.print("HTTP Status Code: ");
  Serial.println(httpCode);
  //---------------------------------------------------------------------
  //getting response from google sheet
  char payload[800];// = {};
  if (httpCode > 0) {
    http.getString().toCharArray(payload, 800);
    Serial.println("Payload: ");
    Serial.println(payload);    
  }
  //---------------------------------------------------------------------
  http.end();
  if(httpCode < 0)ESP.restart();

  StaticJsonDocument<800> doc;
  //char json[] = payload;
  
  DeserializationError error = deserializeJson(doc, payload);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  DUREE_DEEPSLEEP = doc["DUREE_DEEPSLEEP"];
  LED = doc["LED"];
  MAX_HUMIDITY = doc["MAX_HUMIDITY"];
  MIN_HUMIDITY = doc["MIN_HUMIDITY"];
  MAX_TEMP = doc["MAX_TEMP"];
  MIN_TEMP = doc["MIN_TEMP"];
  FORCE_POMPE = doc["FORCE_POMPE"];
  ARROSAGE_AUTOMATIQUE = doc["ARROSAGE_AUTOMATIQUE"];
  DUREE_AROSAGE = doc["DUREE_AROSAGE"];
  HEURE_AROSAGE = doc["HEURE_AROSAGE"];
  TIMELAPSE = doc["TIMELAPSE"];
  FREQUENCE_TIMELAPSE = doc["FREQUENCE_TIMELAPSE"];
  FORCE_PHOTO = doc["FORCE_PHOTO"];

  Serial.print("Variables recieved are :");
  Serial.print(DUREE_DEEPSLEEP);
  Serial.print("; ");
  Serial.print(LED);
  Serial.print("; ");
  Serial.print(MAX_HUMIDITY);
  Serial.print("; ");
  Serial.print(MIN_HUMIDITY);
  Serial.print("; ");
  Serial.print(MAX_TEMP);
  Serial.print("; ");
  Serial.print(MIN_TEMP);
  Serial.print("; ");
  Serial.print(FORCE_POMPE);
  Serial.print("; ");
  Serial.print(ARROSAGE_AUTOMATIQUE);
  Serial.print("; ");
  Serial.print(DUREE_AROSAGE);
  Serial.print("; ");
  Serial.print(HEURE_AROSAGE);
  Serial.print("; ");
  Serial.print(TIMELAPSE);
  Serial.print("; ");
  Serial.print(FREQUENCE_TIMELAPSE);
  Serial.print("; ");
  Serial.println(FORCE_PHOTO);

}

String printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "None";
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  char timeWeekDay[200];
  strftime(timeWeekDay,200, "%B_%d_%Y_%H_%M_%S", &timeinfo);
  return timeWeekDay;
}

void sendVariables(float temp, float humidity, float light){
  
  String timestamp = printLocalTime();
  
  Serial.print("Sending Data to sheet : ");
  Serial.print(timestamp);
  Serial.print(" , ");
  Serial.print(temp);
  Serial.print(" , ");
  Serial.print(humidity);
  Serial.print(" , ");
  Serial.println(light);
  String serverName = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec";
  
  HTTPClient http;
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

  http.begin(serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String dataPost = "timestamp="+timestamp+"&temperature=" + String(temp) + "&humidity=" + String(humidity)+ "&light=" + String(light);
  int httpCode = http.POST(dataPost); 
  Serial.print("HTTP Status Code: ");
  Serial.println(httpCode);
  //---------------------------------------------------------------------
  //getting response from google sheet
  String payload;
  if (httpCode > 0) {
    payload = http.getString();
    Serial.println("Payload: "+payload);    
  }
  //---------------------------------------------------------------------
  http.end();
  if(httpCode < 0)ESP.restart();
}


int getHour()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return 1;
  }
  int currentHour = timeinfo.tm_hour;
  Serial.println("Current hour :");
  Serial.println(currentHour);
  return currentHour;
}
