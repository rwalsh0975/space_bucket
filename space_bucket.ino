  //Libraries 
  #include <Adafruit_Sensor.h>
  #include <DHT.h>
  
  //Constants
  //DHT 22
  #define DHTPIN 2     // Digital Pin for DHT22
  #define DHTTYPE DHT22   // DHT 22  (AM2302)
  DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

  // Photocell
  const float VCC = 4.98; // Measured voltage of Ardunio 5V line
  const float R_DIV = 4610.0; // Measured resistance of 4.7k resistor
  
  // Analog INPUT pins
  const int SOIL_SIG = A0; // Pin connected to SIG of Soil Moisture Sensor
  const int LIGHT_SIG = A1; // Pin connected to voltage divider output
  
  // Digital INPUT pins
  // Digital OUTPUT pins
  const int SOIL_POWER = 3;

// JSON Buffers
DynamicJsonBuffer temperatureBuffer(temperatureBufferSize);
DynamicJsonBuffer humidityBuffer(humidityBufferSize);
DynamicJsonBuffer soilBuffer(soilBufferSize);
DynamicJsonBuffer lightBuffer(lightBufferSize);

// JSON Objects
// Humidity
JsonObject& humidityRoot = humidityBuffer.createObject();
// Soil
JsonObject& soilRoot = soilBuffer.createObject();
// Light
JsonObject& lightRoot = lightBuffer.createObject();
JsonObject& lightValue = lightRoot.createNestedObject("value");
// Temperature
JsonObject& temperatureRoot = temperatureBuffer.createObject();
JsonObject& temperatureValue = temperatureRoot.createNestedObject("value");

// Photocell
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 4610.0; // Measured resistance of 4.7k resistor

// Analog INPUT pins
const int SOIL_SIG = A0; // Pin connected to SIG of Soil Moisture Sensor
const int LIGHT_SIG = A1; // Pin connected to voltage divider output

// Digital INPUT pins

// Digital OUTPUT pins
const int SOIL_POWER = 3;

// Structs
struct TemperatureHumidityReadings {
  int tempCelcius;
  int tempFahrenheit;
  int humidity;
};
struct PhotocellReadings {
  int lightADC;
  float voltage;
  float resistance;
};
struct SensorData {
  TemperatureHumidityReadings tempHumid;
  PhotocellReadings light;
  int soilMoisture;
};

// Other Globals
SensorData sensorData;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();

  // Input Signals
  pinMode(LIGHT_SIG, INPUT);
  pinMode(SOIL_SIG, INPUT);

  // Output Signals
  pinMode(SOIL_POWER, OUTPUT);

  // Initialized to OFF
  digitalWrite(SOIL_POWER, LOW);

  humidityRoot["sensor"] = "humidity";
  soilRoot["sensor"] = "soil-moisture";
  lightRoot["sensor"] = "light";
  temperatureRoot["sensor"] = "temperature";
}

void loop() {
   if(!Serial) {  //check if Serial is available... if not,
    Serial.end();      // close serial port
    delay(100);        //wait 100 millis
    Serial.begin(9600); // reenable serial again
  }

  sensorData.tempHumid = readTempAndHumidity();
  // Humidity
  humidityRoot["time"] = 0;
  humidityRoot["value"] = sensorData.tempHumid.humidity;
  humidityRoot.printTo(Serial);
  Serial.println();

  //Temperature
  temperatureRoot["time"] = 0;
  temperatureValue["fahrenheit"] = sensorData.tempHumid.tempFahrenheit;
  temperatureValue["celcius"] = sensorData.tempHumid.tempCelcius;
  temperatureRoot.printTo(Serial);
  Serial.println();

  // Soil Sensor
  sensorData.soilMoisture = readSoilSensor();
  soilRoot["time"] = 0;
  soilRoot["value"] =  sensorData.soilMoisture;
  soilRoot.printTo(Serial);
  Serial.println();

  // Photocell
  sensorData.light = readPhotocell();
  lightRoot["time"] = 0;
  lightValue["light"] = sensorData.light.lightADC;
  lightValue["voltage"] = sensorData.light.voltage;
  lightValue["resistance"] = sensorData.light.resistance;
  lightRoot.printTo(Serial);
  Serial.println();
  
  delay(10000); // Delay 10 seconds
}

TemperatureHumidityReadings readTempAndHumidity() {
  TemperatureHumidityReadings val;
  val.humidity = dht.readHumidity();
  val.tempCelcius = dht.readTemperature();
  val.tempFahrenheit = val.tempCelcius * 1.8 + 32;
  return val;
}

PhotocellReadings readPhotocell() {
  PhotocellReadings val;
  // Read the ADC, and calculate voltage and resistance from it
  val.lightADC = analogRead(LIGHT_SIG);
  if (val.lightADC > 0)
  {
    // Use the ADC reading to calculate voltage and resistance
    val.voltage = val.lightADC * VCC / 1023.0;
    val.resistance = R_DIV * (VCC / val.voltage - 1.0);
  }
  return val;
}

int readSoilSensor() {
  digitalWrite(SOIL_POWER, HIGH);  // Turn sensor ON
  delay (10);                      // wait for sensor
  int moisture = analogRead(SOIL_SIG); // Read moisture value
  digitalWrite(SOIL_POWER, LOW);   // Turn sensor OFF
  return moisture;
}


// Libraries
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ArduinoJson.h>

// Constants
// DHT22
#define DHTPIN 2     // Digital Pin for DHT22
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
const size_t humidityBufferSize = JSON_OBJECT_SIZE(3);
const size_t temperatureBufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3);
const size_t soilBufferSize = JSON_OBJECT_SIZE(3);
const size_t lightBufferSize = 2*JSON_OBJECT_SIZE(3);