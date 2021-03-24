#include <DHTesp.h>
#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>

const char* ssid     = "";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "";     // The password of the Wi-Fi network

int prevTemp = 0;

#define PATH ""

DHTesp DHT;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("The real time temp and humidity is: ");
  DHT.setup(14, DHTesp::DHT11); // GPIO14 D5
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(" Humidity: ");
  Serial.print(DHT.getHumidity());
  Serial.println('%');
  Serial.print(" Temperature: ");
  int temp = DHT.getTemperature();
  int fahrenheit = (temp * 9/5) + 32;
  Serial.print(fahrenheit);
  Serial.println('F');
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected...");

    if (temp != prevTemp) {
      Serial.println("Temperature changed! POSTing new temp now boss");
      HTTPClient http;
      http.begin(PATH);
      http.addHeader("Content-Type", "text/plain");
      
      http.POST(String(fahrenheit));
      
      delay(2000);
      Serial.println("Done boss!");
    } else {
      Serial.println("Temperature DID NOT change boss");
    }
  }
  prevTemp = temp;
  delay(58000);

}