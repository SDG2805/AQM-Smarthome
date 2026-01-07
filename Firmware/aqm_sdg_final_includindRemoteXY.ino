// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP32CORE_BLE

#include <BLEDevice.h>

// RemoteXY connection settings 
#define REMOTEXY_BLUETOOTH_NAME "RemoteXY"
#define REMOTEXY_ACCESS_PASSWORD "12345"

#include <RemoteXY.h>
#include <Arduino.h>
#include <SensirionI2cScd4x.h>
#include <Wire.h>

// macro definitions
// make sure that we use the proper definition of NO_ERROR
#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

SensirionI2cScd4x sensor;

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t const PROGMEM RemoteXY_CONF_PROGMEM[] =   // 181 bytes V21 
  { 254,12,0,12,0,1,0,32,0,0,0,168,0,21,0,0,0,65,81,77,
  0,24,1,200,100,1,1,9,0,68,0,2,200,53,49,24,36,67,79,194,
  178,32,32,105,110,32,112,112,109,0,7,1,65,40,10,110,64,2,26,2,
  1,7,1,87,40,10,110,64,2,26,2,1,194,53,16,20,20,1,68,75,
  55,125,41,50,24,94,247,84,101,109,112,101,114,97,116,117,114,32,105,110,
  32,67,194,176,0,70,101,117,99,104,116,105,103,107,101,105,116,32,105,110,
  32,37,0,7,43,65,32,11,110,64,2,26,2,1,129,3,55,33,11,64,
  17,112,101,111,112,108,101,0,129,41,56,39,7,64,17,65,117,195,159,101,
  110,116,101,109,112,46,0,129,3,77,33,10,64,17,118,111,108,117,109,101,
  0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  float people;
  float volume;
  float ausentemp;

    // output variables
  float co2c;
  float tem;
  float hum;

    // complex variables
  RemoteXYType_Notification notification_01; // call .print() or .println(), then .send()

} RemoteXY;   
#pragma pack(pop)

float prev_people;
float prev_volume;
float prev_ausentemp;

int buzzer = 17;
int ledr = 4;
int ledb = 15;
int ledg = 23;
String AQ = "good";

static char errorMessage[64];
static int16_t error;

void PrintUint64(uint64_t& value) {
  Serial.print("0x");
  Serial.print((uint32_t)(value >> 32), HEX);
  Serial.print((uint32_t)(value & 0xFFFFFFFF), HEX);
}

void setup() {
  RemoteXY_Init ();  // initialization by macros

  pinMode(ledr, OUTPUT);
  pinMode(ledg, OUTPUT);
  pinMode(ledb, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
    RemoteXYEngine.delay(100);
  }
  Wire.begin();
  sensor.begin(Wire, SCD41_I2C_ADDR_62);

  uint64_t serialNumber = 0;
  RemoteXYEngine.delay(30);
  // Ensure sensor is in clean state
  error = sensor.wakeUp();
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute wakeUp(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
  }
  error = sensor.stopPeriodicMeasurement();
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
  }
  error = sensor.reinit();
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute reinit(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
  }
  // Read out information about the sensor
  error = sensor.getSerialNumber(serialNumber);
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute getSerialNumber(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }
  Serial.print("serial number: ");
  PrintUint64(serialNumber);
  Serial.println();
  //
  // If temperature offset and/or sensor altitude compensation
  // is required, you should call the respective functions here.
  // Check out the header file for the function definitions.
  // Start periodic measurements (5sec interval)
  error = sensor.startPeriodicMeasurement();
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute startPeriodicMeasurement(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }
  //
  // If low-power mode is required, switch to the low power
  // measurement function instead of the standard measurement
  // function above. Check out the header file for the definition.
  // For SCD41, you can also check out the single shot measurement example.
  //
}

void loop() {
  RemoteXYEngine.handler (); 

  bool dataReady = false;
  uint16_t co2Concentration = 0;
  float temperature = 0.0;
  float relativeHumidity = 0.0;
  //
  // Slow down the sampling to 0.2Hz.
  //
  RemoteXYEngine.delay(5000);
  error = sensor.getDataReadyStatus(dataReady);
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute getDataReadyStatus(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }
  while (!dataReady) {
    RemoteXYEngine.delay(100);
    error = sensor.getDataReadyStatus(dataReady);
    if (error != NO_ERROR) {
      Serial.print("Error trying to execute getDataReadyStatus(): ");
      errorToString(error, errorMessage, sizeof errorMessage);
      Serial.println(errorMessage);
      return;
    }
  }
  //
  // If ambient pressure compenstation during measurement
  // is required, you should call the respective functions here.
  // Check out the header file for the function definition.
  error =
    sensor.readMeasurement(co2Concentration, temperature, relativeHumidity);
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute readMeasurement(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }
  int co2conc = (int) co2Concentration;
  //
  // Print results in physical units.
  Serial.print("CO2 concentration [ppm]: ");
  Serial.print(co2Concentration);
  Serial.println();
  Serial.print("Temperature [°C]: ");
  Serial.print(temperature);
  Serial.println("");
  Serial.print("Relative Humidity [RH in %]: ");
  Serial.print(relativeHumidity);
  Serial.println();
  
  if (co2Concentration <= 1000) {
    setColor(0, 255, 0);
    digitalWrite(buzzer, LOW);
  }
  if (co2Concentration > 1000 && co2Concentration <= 1380) { 
    setColor(255, 100, 0);
    digitalWrite(buzzer, LOW);
  }
  if (co2Concentration > 1380){
   setColor(255, 0, 0);
   RemoteXY.notification_01.print("Ventilate!/Lüften!");
   RemoteXY.notification_01.println();
   RemoteXY.notification_01.send();
   digitalWrite(buzzer, HIGH);
   RemoteXYEngine.delay(200);
   digitalWrite(buzzer, LOW);
   RemoteXYEngine.delay(100);
  }
  float ventilate = 0;
  if (RemoteXY.people != 0 && RemoteXY.volume != 0){
    ventilate = ((0.00001*800)/(RemoteXY.people*0.1/RemoteXY.volume)); // formula which tells how often it should be ventilated per h
  }
  float ventilation_duration;
  if (prev_volume != RemoteXY.volume || prev_people != RemoteXY.people || prev_ausentemp != RemoteXY.ausentemp){
    if (RemoteXY.ausentemp < 10){
      ventilation_duration = 3.5; //2-5 is the rec
    } else if (RemoteXY.ausentemp >= 10 && RemoteXY.ausentemp < 20){
      ventilation_duration = 7.5; //5-10 is the rec
    } else if (RemoteXY.ausentemp >= 20){
      ventilation_duration = 13; //10-20 is the rec
    }
    RemoteXY.notification_01.print("You should ventilate every ");
    RemoteXY.notification_01.print(ventilate);
    RemoteXY.notification_01.print(" hour, for a duration of ");
    RemoteXY.notification_01.print(ventilation_duration);
    RemoteXY.notification_01.print(" min.");
    RemoteXY.notification_01.println();
    RemoteXY.notification_01.send();
    prev_volume = RemoteXY.volume;
    prev_people = RemoteXY.people;
    prev_ausentemp = RemoteXY.ausentemp;
  }
  RemoteXY.co2c = co2conc;
  RemoteXY.tem = temperature;
  RemoteXY.hum = relativeHumidity;
}

void setColor(int redValue, int greenValue, int blueValue){
  analogWrite(ledr, redValue);
  analogWrite(ledg, greenValue);
  analogWrite(ledb, blueValue);
}