// Load Wi-Fi library
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "DHT.h"
#include <HTTPClient.h>
#include "time.h"
#include <Arduino_JSON.h>

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 12     // Digital pin connected to the DHT sensor

DHT dht(DHTPIN, DHTTYPE); // setup the sensor

// Replace with your network credentials
const char* ssid = "NETGEAR95";
const char* password = "aquaticpiano858";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

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

String GOOGLE_SCRIPT_ID = "AKfycbwz7PGT_d0WqjZNljvVgTL-JOlp5CIQEWr-2hi93QzqW-zDg0fm_YS07wq9Yspp5LDX";    // change Gscript ID

String printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "None";
  }
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  char timeWeekDay[200];
  strftime(timeWeekDay,200, "%B %d %Y %H:%M:%S", &timeinfo);
  return timeWeekDay;
}


void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);
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

  dht.begin();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}

void loop(){
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  float light = analogRead(13);

  
  String timestamp = printLocalTime();
  Serial.println(timestamp);
  delay(1000);
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            } else if (header.indexOf("GET /read") >= 0) {
              String serverName = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?read=1";
              
              HTTPClient http;
              http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
              http.begin(serverName);
              int httpCode = http.GET();
              Serial.print("HTTP Status Code: ");
              Serial.println(httpCode);
              //---------------------------------------------------------------------
              //getting response from google sheet
              String payload = {};
              if (httpCode > 0) {
                  payload = http.getString();
                  Serial.println("Payload: "+payload);    
              }
              //---------------------------------------------------------------------
              http.end();

              //TREAT THE PAYLOAD

              JSONVar myObject = JSON.parse(payload);
              // JSON.typeof(jsonVar) can be used to get the type of the var
              if (JSON.typeof(myObject) == "undefined") {
                Serial.println("Parsing input failed!");
                return;
              }
              
              Serial.print("JSON object = ");
              Serial.println(myObject);
              
              // myObject.keys() can be used to get an array of all the keys in the object
              JSONVar keys = myObject.keys();
              double sensorReadingsArr = 0;
              for (int i = 0; i < keys.length(); i++) {
                JSONVar value = myObject[keys[i]];
                Serial.print(keys[i]);
                Serial.print(" = ");
                Serial.println(value);
                sensorReadingsArr = double(value);
              }
              digitalWrite(output27, bool(sensorReadingsArr));


            } else if (header.indexOf("GET /SendData") >= 0) {
              Serial.println("Sending Data to sheet : ");
              Serial.print(temp);
              Serial.print(" , ");
              Serial.print(humidity);
              Serial.print(" , ");
              Serial.print(light);
              String serverName = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec";//+"/exec?";
              
              HTTPClient http;
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
              
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button       
            
            client.println("<p><a href=\"/read\"><button class=\"button button2\">READ LED</button></a></p>");
            
            client.println("<p><a href=\"/SendData\"><button class=\"button button2\">Write Data </button></a></p>");

            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
