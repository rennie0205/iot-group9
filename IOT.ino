#define BLYNK_TEMPLATE_ID "TMPL6jfYm1_SR"
#define BLYNK_TEMPLATE_NAME "Smart Indoor Plant Watering System"
#define BLYNK_AUTH_TOKEN "-16Q1mRH5htzBnzM5TiTfdFfh9Ym7w7g"
 
#define BLYNK_PRINT Serial
 
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>
 
// === WiFi and Blynk Configuration ===
char ssid[] = "Steins;Gate";        // Replace with your WiFi SSID
char pass[] = "1.048596";    // Replace with your WiFi password
char auth[] = BLYNK_AUTH_TOKEN;        // Blynk authentication token
 
// === DHT11 Sensor Setup ===
#define DHTPIN 17             // DHT11 data pin connected to GPIO 4
#define DHTTYPE DHT11        // Define sensor type
DHT dht(DHTPIN, DHTTYPE);    // Initialize DHT sensor
  
#define SOIL_MOISTURE_PIN 34
 
BH1750 lightMeter;
BlynkTimer timer;
 
// === Function to Send sensor data to Blynk ===
void sendSensorData() {
   // Temperature
  float temp = dht.readTemperature();  // Read temperature in Celsius
  if (!isnan(temp)) {
    Serial.print("Temp: ");
    Serial.println(temp);
    Blynk.virtualWrite(V0, temp); 
  } else {
    Serial.println("Read Temperature failed");
  }

  // Humidity
  float humidity = dht.readHumidity();  // Read temperature in Celsius
  if (!isnan(humidity)) {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    Blynk.virtualWrite(V1, humidity); 
  } else {
    Serial.println("Read Humidity failed");
  }

   // light intensity
  float lux = lightMeter.readLightLevel();
  if(!isnan(lux)) {
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lux");
    Blynk.virtualWrite(V3, lux);
  } else {
    Serial.println("Read light intensity failed");
  }

   // Soil moisure
  int soilRaw = analogRead(SOIL_MOISTURE_PIN);  
  int soilPercent = map(soilRaw, 3720, 1500, 0, 100);  
  soilPercent = constrain(soilPercent, 0, 100);
  if(!isnan(soilPercent)){
    Serial.print("Soil Moisture: ");
    Serial.print(soilPercent);
    Serial.println(" %");
    Blynk.virtualWrite(V2, soilPercent);
  } else {
    Serial.println("Read soil moisure failed");
  }
}
 
void setup() {
  Serial.begin(115200);  // Start serial monitor
  dht.begin();           // Initialize DHT11 sensor
  Wire.begin(21,22);
  lightMeter.begin();
 
  // Connect to WiFi manually (non-blocking method)
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi connected");
 
  Blynk.config(auth);     // Set up Blynk without blocking WiFi
  Blynk.connect();        // Try to connect to Blynk server
 
  // Set timer to send temperature every 10 seconds
  timer.setInterval(10000L, sendSensorData);
}
 
void loop() {
  Blynk.run();    // Keep Blynk running
  timer.run();    // Handle timed functions
}