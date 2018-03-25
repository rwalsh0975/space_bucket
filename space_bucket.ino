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
}

void loop() {
  // Temperature & Humidity
  TemperatureHumidityReadings tempHumid = readTempAndHumidity();
  printTemperatureHumidity(tempHumid);

  // Photocell
  PhotocellReadings light = readPhotocell();
  printPhotocell(light);

  // Soil Sensor
  int soilMoisture = readSoilSensor();
  Serial.println("Soil Moisture: " + String(soilMoisture));
  delay(10000); // Delay 10 seconds
}

TemperatureHumidityReadings readTempAndHumidity() {
    TemperatureHumidityReadings val;
    val.humidity = dht.readHumidity();
    val.tempCelcius = dht.readTemperature();
    val.tempFahrenheit = val.tempCelcius * 1.8 + 32;
    return val;
}

void printTemperatureHumidity(TemperatureHumidityReadings tempHumid) {
  Serial.println("Humidity: " + String(tempHumid.humidity) + "%; Temperature (°C): " + String(tempHumid.tempCelcius) + "°C; Temperature (°F): " + String(tempHumid.tempFahrenheit) + "°F");
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

void printPhotocell(PhotocellReadings light) {
  Serial.println("Light Value: " + String(light.lightADC) + " ; Voltage: " + String(light.voltage) + " Volts; Resistance: " + String(light.resistance) + "Ωs");
}

int readSoilSensor() {
  digitalWrite(SOIL_POWER, HIGH);  // Turn sensor ON
  delay (10);                      // wait for sensor
  int moisture = analogRead(SOIL_SIG); // Read moisture value
  digitalWrite(SOIL_POWER, LOW);   // Turn sensor OFF 
  return moisture; 
}

