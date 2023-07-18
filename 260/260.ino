#include <Adafruit_INA260.h>
#include <wire.h>
#include <LiquidCrystal.h>
#include <WiFi.h>
#include "ThingSpeak.h"


const char* ssid    = "****"; %ingresa nombre red wifi
const char* password = "****";% contraseña

unsigned long channelID = ; %nombre del canal de thingspeak
const char* WriteAPIKey =""; %llave de la api
WiFiClient cliente;

Adafruit_INA260 ina260 = Adafruit_INA260();

int sda_pin = 8; // GPIO16 as I2C SDA
int scl_pin =9 ; // GPIO17 as I2C SCL

//TwoWire I2CBME = TwoWire(0);

void setup() {
  
  USBSerial.begin(115200);
  while (!USBSerial) { delay(10); }

  Wire.begin(sda_pin, scl_pin);
 // Wire.setPins(sda_pin, scl_pin); // Set the I2C pins before begin
  //Wire.begin(); // join i2c bus (address optional for master)
 
  Serial.begin(115200);
  // Wait until serial port is opened
  while (!Serial) { delay(10); }


  if (! ina260.begin()) {
    Serial.println("Failed to find INA260 chip");
    while (1) { delay(10); }
  }

  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("Conectado al WiFi");
  ThingSpeak.begin(cliente);
  delay(5000);
  
 
}

void loop() {

    float shuntvoltage = 0;
    float busvoltage = 0;
    float current_A = 0;
    float loadvoltage = 0;
    float power_W = 0;
    
    //shuntvoltage =ina260.readShuntVoltaje_mV();
    busvoltage = ina260.readBusVoltage();
    current_A = ina260.readCurrent();
    power_W = ina260.readPower();
    loadvoltage = (busvoltage + (shuntvoltage/1000));
    delay(1000);

  Serial.print(busvoltage); Serial.print("\n"); 
  Serial.print(shuntvoltage); Serial.print("\n");
  Serial.print(loadvoltage); Serial.print("\n");
  Serial.print(current_A); Serial.print("\n");
  Serial.println(power_W);
  delay(1000);

  ThingSpeak.setField(1,loadvoltage);
  ThingSpeak.setField(2,current_A);
  ThingSpeak.setField(3,power_W);

  String volta = String(loadvoltage);

 
  
  ThingSpeak.writeFields(channelID,WriteAPIKey);
  /*Imprimimos una frase indicando el envío, y agregamos un retardo de 10 segundos*/
  Serial.println("Datos enviados a ThingSpeak!");Serial.print("\n"); 
  //delay(10000);

  
}
