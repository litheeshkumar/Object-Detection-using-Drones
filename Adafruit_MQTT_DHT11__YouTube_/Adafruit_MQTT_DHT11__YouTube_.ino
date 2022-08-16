#include <SimpleDHT.h>      // Data ---> D3
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// WiFi parameters
#define WLAN_SSID       "******"
#define WLAN_PASS       "******"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "******"
#define AIO_KEY         "*******" 
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish Temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");
Adafruit_MQTT_Publish Humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");
Adafruit_MQTT_Publish Soil_Moisture = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Soil_Moisture");

int pinDHT11 = 0;
SimpleDHT11 dht11(pinDHT11);
byte hum = 0;  //Stores humidity value
byte temp = 0; //Stores temperature value
const int soil_moisture_pin = A0;
float soil_moisture_value = 0;  //Stores soil moisture value

void setup() 
{
    Serial.begin(115200);
    Serial.println(F("Adafruit IO Example"));
    // Connect to WiFi access point.
    Serial.println(); Serial.println();
    delay(10);
    Serial.print(F("Connecting to "));
    Serial.println(WLAN_SSID);
    WiFi.begin(WLAN_SSID, WLAN_PASS);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(F("."));
    }
    Serial.println();
    Serial.println(F("WiFi connected"));
    Serial.println(F("IP address: "));
    Serial.println(WiFi.localIP());

    // connect to adafruit io
    connect();
}

// connect to adafruit io via MQTT
void connect() 
{
    Serial.print(F("Connecting to Adafruit IO... "));
    int8_t ret;
    while ((ret = mqtt.connect()) != 0) 
    {
        switch (ret) 
        {
            case 1: Serial.println(F("Wrong protocol")); break;
            case 2: Serial.println(F("ID rejected")); break;
            case 3: Serial.println(F("Server unavail")); break;
            case 4: Serial.println(F("Bad user/pass")); break;
            case 5: Serial.println(F("Not authed")); break;
            case 6: Serial.println(F("Failed to subscribe")); break;
            default: Serial.println(F("Connection failed")); break;
        }
        if(ret >= 0)
            mqtt.disconnect();
        Serial.println(F("Retrying connection..."));
        delay(10000);
    }
    Serial.println(F("Adafruit IO Connected!"));
}

void loop()   // ping adafruit io a few times to make sure we remain connected 
{
    if(! mqtt.ping(3))    // reconnect to adafruit io 
    {
        if(! mqtt.connected())
        connect();
    }
    dht11.read(&temp, &hum, NULL);
    int soil_moisture_value = analogRead(soil_moisture_pin);
    Serial.print("Temperature = ");
    Serial.print((int)temp); Serial.print(" *C, "); 
    Serial.print("Humidity = ");
    Serial.print((int)hum); Serial.println(" H");
    Serial.print("Soil Moisture Values = ");
    Serial.println(soil_moisture_value); 
    delay(5000);
   
    if (! Temperature.publish(temp))   //Publish to Adafruit
    {                     
        Serial.println(F("Failed to publish Temperature"));
    } 
    if (! Humidity.publish(hum))       //Publish to Adafruit 
    {                     
        Serial.println(F("Failed to publish Humidity"));
    }
    if (!Soil_Moisture.publish(soil_moisture_value))  //Publish to Adafruit 
    {                     
        Serial.println(F("Failed to publish Humidity"));
    }
    else 
    {
        Serial.println(F("Sent!"));
    }
}
