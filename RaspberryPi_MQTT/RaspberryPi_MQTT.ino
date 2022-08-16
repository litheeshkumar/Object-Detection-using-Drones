#include <ESP8266WiFi.h>          
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <PubSubClient.h>

#define DHTPIN D0        // Digital pin connected to the DHT sensor
#define LED D2
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

//declaration of variables
float tempC = 0;
const int soil_moisture_pin = A0;
float soil_moisture_value = 0;

unsigned long previousMillis = 0;
unsigned long interval = 10000;

const char* mqttServer = "192.168.43.2";
const int mqttPort = 1883;

char ssid[] = "******";
char pass[] = "******";

WiFiClient client;

int status = WL_IDLE_STATUS;

void connectWifi()
{
    Serial.print(F("Connecting to "));
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(F("."));
    }
    Serial.println();
    Serial.print(F("WiFi connected"));
    Serial.println(F("IP address: "));
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    Serial.println();
  
    String message = "";
    for (int i = 0; i < length; i++) 
    {
        Serial.print((char)payload[i]);
        message+=(char)payload[i];
    }
    Serial.println("-----------------------");
    if(String(topic)=="LED")
    {
        if(message=="LED ON")
        {
            digitalWrite(LED,HIGH);
            Serial.println("LED IS ON");
        }
        else
        {
            digitalWrite(LED,LOW);
        }
    }
}

WiFiClient espClient;
PubSubClient client(mqttServer, mqttPort, callback, espClient);

void setup() 
{
    Serial.begin(115200);
    
    connectWifi();   // Connect to Wi-Fi network.
    Serial.println("connected...");
    pinMode(LED,OUTPUT);
    digitalWrite(LED,LOW);
    dht.begin();
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    
    while (!client.connected()) 
    {
        Serial.println("Connecting to MQTT...");
        if (client.connect("ESP8266Client" )) 
        {
            client.subscribe("LED");     
            Serial.println("connected");
        } 
        else 
        {
            Serial.print("failed with state ");
            //Serial.print(client.state());
            delay(2000);
        }
    }
}

void loop() 
{
    int count = 0;
    count = count + 1;
    int chk = DHT.read11(DHT11_PIN);
    
    client.loop();
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) 
    {
        // save the last time you updated the DHT values and Soil Moisture values
        previousMillis = currentMillis;
        tempC = DHT.temperature;
        soil_moisture_value = analogRead(soil_moisture_pin);
        if(!isnan(soil_moisture_value)&&!isnan(tempC))
        {
            Serial.println("Temperature : " +String(tempC));
            Serial.println("soil moisture values : " +String(soil_moisture_value));
            String toSend = String(tempC) + "," +String(soil_moisture_value);
            client.publish("data",toSend.c_str());
        }
    }

    // Reconnect to WiFi if it gets disconnected.
    if (WiFi.status() != WL_CONNECTED) 
    {
        connectWifi();
    }
}
