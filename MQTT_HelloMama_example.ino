/*
 MQTT publish/subscriber example https://www.cloudmqtt.com/

  - connects to an MQTT server
  - publishes to the topic "outTopic"
  - subscribes to the topic "outTopic"

  Modified by SpidyHero 
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 0;     // NodeMCU pushbutton pin
const int ledPin =  16;      // LED pin on NodeMCU

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

//-------------------------------//

const char *ssid =  "xxxxx";   // your WiFi network name
const char *pass =  "xxxxx";   //your WiFi network password

// Update these with values suitable for your network.
IPAddress server(xx,xx,xxx,xxx); // CloudMQTT Server IP, ping m11.cloudmqtt.com to get IP

WiFiClient wclient;
PubSubClient client(wclient,"m11.cloudmqtt.com", 18940); //your MQTT Port number

// use this function to receive message from CloundMQTT subscribes topic
void callback(const MQTT::Publish& pub) { 
  Serial.print(pub.topic());
  Serial.print(" => ");
  Serial.println(pub.payload_string());
}


// setup
void setup()
{
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();

  
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);


  
  client.set_callback(callback);

  WiFi.begin(ssid, pass);

  int retries = 0;
  while ((WiFi.status() != WL_CONNECTED) && (retries < 10)) {
    retries++;
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
  }

//----MQTT validate connection ----//

  if (client.connect(MQTT::Connect("NodeMCU")
                    .set_auth("yourCloudMQTTuser", "yourCloudMQTTpass"))) {
    Serial.println();
    Serial.println("Publish success.");
    client.publish("outTopic","NodeMCUconnected"); //Publish/send message to clound on outTopic
    
    Serial.println("Subscribe success.");
    client.subscribe("outTopic"); //Subscribes topic to receive message from cloud and print on monitor
    
               }
}

void loop()
{
  client.loop();

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  }
  else {
    // turn LED off:
    // send message to your topic when release button
    digitalWrite(ledPin, LOW);
    client.publish("outTopic","button is pushed"); // send message to "outTopic" when release button
    Serial.print("button is pushed");
    Serial.println();
    delay(300);
  }
  
}
