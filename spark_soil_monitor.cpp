// This #include statement was automatically added by the Particle IDE.
#include "lib1.h"

#include "SparkFunMAX17043/SparkFunMAX17043.h"

/*  
    This application monitors the moisture level of your houseplant 
    and exposes that data to be monitored via the Internet. 
    Development environment specifics:
    Particle Build environment (https://www.particle.io/build)
    Particle Photon RedBoard
    Released under the MIT License(http://opensource.org/licenses/MIT)
*/

int val = 0; //variable to store soil value
int soil = A2; //Declare a variable for the soil moisture sensor 
int soilPower = D6; //Variable for Soil moisture Power

void setup() {

Serial.begin(9600); 

pinMode(soilPower, OUTPUT); //Set D7 as an OUTPUT
digitalWrite(soilPower, LOW); //Set to LOW so no power is flowing through the sensor

// Set up the MAX17043 LiPo fuel gauge:
lipo.begin(); // Initialize the MAX17043 LiPo fuel gauge
lipo.quickStart(); // Quick start restarts the MAX17043 in hopes of getting a more accurate guess for the SOC.

}

void loop() {
    
    Spark.disconnect();
    WiFi.disconnect();
    WiFi.off(); 
    doConnect();
    if (Spark.connected()) {
        Particle.publish("soilLambda", "{\"soil\":" + String(readSoil()) + ", \"battery\":"+ String(getVoltagePercentage()) + "}", 60, PRIVATE);
        RGB.control(true);
        RGB.color(255, 0, 0);
        delay(5000);
    }
    RGB.control(true);
    RGB.color(0,255,0);
    delay(1000);
    System.sleep(SLEEP_MODE_DEEP, 300);
}

void doConnect() {
    WiFi.on();
    if (!WiFi.ready()) {
        WiFi.connect();
        while (WiFi.connecting()) {
            Spark.process(); //SPARK_WLAN_Loop();
            delay(1000);
        }
    }

    if (!Spark.connected()) {
        Spark.connect();
        while (!Spark.connected()) {
            Spark.process(); //SPARK_WLAN_Loop();
            delay(1000);
        }
    }     
}

int readSoil() {
    digitalWrite(soilPower, HIGH);//turn D6 "On"
    delay(10);//wait 10 milliseconds 
    val = analogRead(soil);
    digitalWrite(soilPower, LOW);//turn D6 "Off"
    return val;
}

float getVoltagePercentage() {
    return lipo.getSOC();
}