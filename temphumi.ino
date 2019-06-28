#include <DHT.h> //  DHT.h library
#include <ESP8266WiFi.h> // ESP8266WiFi.h library

#define DHTPIN 0        //Not D0 Pin it is D3 Pin, Here 0 means GPIO 0.
#define DHTTYPE  DHT11

const char* ssid     = "";//  WiFi SSID(Name)
const char* password = "";// Wifi Password 
const char* host = "api.thingspeak.com";
const char* writeAPIKey = "65X66705R585BPN2"; //copy ThingSpeak channel API Key.

DHT dht(DHTPIN, DHTTYPE, 15);

void setup() {
  // Initialize sensor
 Serial.begin(115200);
 dht.begin();
  delay(1000);
 Serial.println("Connecting to ");
       Serial.println(ssid);
//  Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
   Serial.println("");
   Serial.println("WiFi connected");
}

void loop() {
 float humidity = dht.readHumidity();
 float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    return;
  }

// make TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    return;
  }

  String url = "/update?key=";
  url+=writeAPIKey;
  url+="&field1=";
  url+=String(temperature);
  url+="&field2=";
  url+=String(humidity);
  url+="\r\n";
  
  // Request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  Serial.print("Temperature:");
  Serial.print(temperature);
  Serial.print("\n");
  Serial.print("Humidity:");
  Serial.println(humidity);
  Serial.println("Send to ThingSpeak.\n");
client.stop();
  Serial.println("Wait for 15 sec to update next datapack in thingSpeak");
    delay(1000);
 
}
