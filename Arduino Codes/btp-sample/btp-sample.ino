#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h>
#include <dht.h>
dht DHT;
#define DHT11_PIN D0
float hum=0;

//declaration of variables
float tempC = 0;

const int soil_moisture_pin = A0;
float soil_moisture_value = 0;

char ssid[] = "HA-7";
char pass[] = "68426842";

#define channelID 1702216
const char mqttUserName[] = "EgwEIgYVCCIMISgCMAYSICk";
const char clientID[] = "EgwEIgYVCCIMISgCMAYSICk";
const char mqttPass[] = "+fy/5Vw9VNuREA/Bswn5itud";

#define mqttPort 1883
WiFiClient client;

const char* server = "mqtt3.thingspeak.com";
int status = WL_IDLE_STATUS;
long lastPublishMillis = 0;
int connectionDelay = 1;
int updateInterval = 15;
PubSubClient mqttClient( client );

void connectWifi()
{
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
}

void mqttConnect() {
  // Loop until connected.
  while ( !mqttClient.connected() )
  {
    // Connect to the MQTT broker.
    if ( mqttClient.connect( clientID, mqttUserName, mqttPass ) ) {
      Serial.print( "MQTT to " );
      Serial.print( server );
      Serial.print (" at port ");
      Serial.print( mqttPort );
      Serial.println( " successful." );
    } else {
      Serial.print( "MQTT connection failed, rc = " );
      Serial.print( mqttClient.state() );
      Serial.println( " Will try again in a few seconds" );
      delay( connectionDelay * 1000 );
    }
  }
}

void setup() {
  Serial.begin( 115200 );
  // Delay to allow serial monitor to come up.
  delay(500);

  // Connect to Wi-Fi network.
  connectWifi();

  // Configure the MQTT client
  mqttClient.setServer( server, mqttPort );

  // Set the MQTT message handler function.
  mqttClient.setCallback( mqttSubscriptionCallback );

  // Set the buffer to handle the returned JSON. NOTE: A buffer overflow of the message buffer will result in your callback not being invoked.
  mqttClient.setBufferSize( 2048 );
}

// Function to handle messages from MQTT subscription.
void mqttSubscriptionCallback( char* topic, byte* payload, unsigned int length ) {
  // Print the details of the message that was received to the serial monitor.
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Subscribe to ThingSpeak channel for updates.
void mqttSubscribe( long subChannelID ) {
  String myTopic = "channels/" + String( subChannelID ) + "/subscribe/fields/field1";
  String myTopic1 = "channels/" + String( subChannelID ) + "/subscribe/fields/field2";
  String myTopic2 = "channels/" + String( subChannelID ) + "/subscribe/fields/field3";
  String myTopic3 = "channels/" + String( subChannelID ) + "/subscribe/fields/field4";
  mqttClient.subscribe(myTopic.c_str());
  mqttClient.subscribe(myTopic1.c_str());
  mqttClient.subscribe(myTopic2.c_str());
  mqttClient.subscribe(myTopic3.c_str());
}

// Publish messages to a ThingSpeak channel.
void mqttPublish(long pubChannelID, String message) {
  String topicString = "channels/" + String( pubChannelID ) + "/publish";
  mqttClient.publish( topicString.c_str(), message.c_str() );
}

void loop() {

  int count = 0;
  count = count + 1;
  int chk = DHT.read11(DHT11_PIN);

  tempC = DHT.temperature;
  delay(100);

// temperature sensor
//  tempC = analogRead(tempPin);
//  tempC = tempC * 300 / 1023;

  
  // Humidity
  hum = DHT.humidity;
  delay(100);

  // pH sensor
  float pH = random(60, 64);
  pH = pH / 10;
  delay(100);
  
  //Soil Moisture
  soil_moisture_value = analogRead(soil_moisture_pin);
 

//  float randomnumber = random(600, 970);
  delay(2000);

  // Reconnect to WiFi if it gets disconnected.
  if (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }

  // Connect if MQTT client is not connected and resubscribe to channel updates.
  if (!mqttClient.connected()) {
    mqttConnect();
    mqttSubscribe( channelID );
  }

  // Call the loop to maintain connection to the server.
  mqttClient.loop();
  delay(1000);
  // Update ThingSpeak channel periodically. The update results in the message to the subscriber.
  if ( (millis() - lastPublishMillis) > updateInterval * 1000) {
    mqttPublish( channelID, (String("field1=") + String(tempC )));
    delay(4000);
    mqttPublish( channelID, (String("field2=") + String(hum) ) );
    delay(4000);
    mqttPublish( channelID, (String("field3=")  + String(pH)) );
    delay(4000);
    mqttPublish( channelID, (String("field4=")  + String(soil_moisture_value)) );
    Serial.println("--------------------------------------------");
    lastPublishMillis = millis();

  }



}
