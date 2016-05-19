#include "lib1.h"
#include "SparkFunMAX17043/SparkFunMAX17043.h"

int val = 0; 
int soil = A2;  
int soilPower = D6;

void setup() {

Serial.begin(9600); 

pinMode(soilPower, OUTPUT);
digitalWrite(soilPower, LOW);

// Set up the MAX17043 LiPo fuel gauge
lipo.begin();
lipo.quickStart();

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
    digitalWrite(soilPower, HIGH);
    delay(10);
    val = analogRead(soil);
    digitalWrite(soilPower, LOW);
    return val;
}

float getVoltagePercentage() {
    return lipo.getSOC();
}