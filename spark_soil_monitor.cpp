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

pinMode(soilPower, OUTPUT); //Set D7 as an OUTPUT
digitalWrite(soilPower, LOW); //Set to LOW so no power is flowing through the sensor

// Set up the MAX17043 LiPo fuel gauge:
lipo.begin(); // Initialize the MAX17043 LiPo fuel gauge
lipo.quickStart(); // Quick start restarts the MAX17043 in hopes of getting a more accurate guess for the SOC.

}

void loop() {
//get soil moisture value from the function below and print it
Particle.publish("soilLambda", "{\"soil\":" + String(readSoil()) + ", \"battery\":"+ String(getVoltagePercentage()) + "}", 60, PRIVATE);

delay(3600000);//take a reading every hour
//This time is used so you can test the sensor and see it change in real-time.
//For in-plant applications, you will want to take readings much less frequently.

    //If your soil is too dry, turn on Red LED to notify you
    //This value will vary depending on your soil and plant
    if(readSoil() < 200)
    {
      // take control of the RGB LED
      RGB.control(true);
      RGB.color(255, 0, 0);//set RGB LED to Red
    }
    else
    {
      // resume normal operation
      RGB.control(false);
    }
}
//This is a function used to get the soil moisture content
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
