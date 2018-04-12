import serial
import json
from ubidots import ApiClient

ser = serial.Serial('/dev/ttyACM0',115200)
api = ApiClient(token='A1E-rGXA3bemNJ97L1Pq7EACElgtQVGXhf')
soilMoisture = api.get_variable('5ac2c0fac03f976ab842e0f0')
light = api.get_variable('5ac2bffac03f976a183cc31d')
temperature = api.get_variable('5ac2bf1bc03f9768bd07bf53')
humidity = api.get_variable('5ac2bf55c03f9768bf828234')
logFilename = 'sensors.log';
logFile = open(logFilename, 'a+');

def reportSoilMoisture(data):
    print('Soil Moisture: %s' % data["value"])
    logFile.write('Soil Moisture: %s' % data["value"] + '\n')
    soilMoisture.save_value({'value': data["value"]})

def reportTemperature(data):
    print('Temperature: %s' % data["value"]["fahrenheit"])
    logFile.write('Temperature: %s' % data["value"]["fahrenheit"] + '\n')
    temperature.save_value({'value': data["value"]["fahrenheit"]})

def reportHumidity(data):
    print('Humidity: %s' % data["value"])
    logFile.write('Humidity: %s' % data["value"] + '\n')
    humidity.save_value({'value': data["value"]})

def reportLight(data):
    print('Light: %s' % data["value"]["light"])
    logFile.write('Light: %s' % data["value"]["light"] + '\n')
    light.save_value({'value': data["value"]["light"]})

def sensor_chooser(data):
    switcher = {
            "soil-moisture": reportSoilMoisture,
            "temperature": reportTemperature,
            "humidity": reportHumidity,
            "light": reportLight
    }
    funcToRun = switcher.get(data["sensor"], lambda x: "Invalid Sensor")
    funcToRun(data)
            
while True:
    try:
        sensorData = ser.readline()
        jsonData = json.loads(sensorData)
        sensor_chooser(jsonData)
    except:
        logFile.write('Invalid JSON: %s' % sensorData)

