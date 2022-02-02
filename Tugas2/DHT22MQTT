#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 18
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "ASUS_X008";
const char* password = "12345678";
const char* mqtt_server = "192.168.43.188";

// set pin numbers
const int buttonPin = 4;  
const int ledPin =  5;    

// variable for storing the pushbutton status 
int ledState = LOW;     
int lastButtonState;    
int currentButtonState;
long now = millis();
long lastMeasure = 0; 

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  
  if(topic=="led"){
      Serial.print("Changing LED to ");
      if(messageTemp == "on"){
        digitalWrite(ledPin, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(ledPin, LOW);
        Serial.print("Off");
      }
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);  
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  dht.begin();

  currentButtonState = digitalRead(buttonPin);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
} 

void loop() {
  lastButtonState    = currentButtonState;   
  currentButtonState = digitalRead(buttonPin);

  if(lastButtonState == HIGH && currentButtonState == LOW) {
    Serial.println("The button is pressed");
    
    ledState = !ledState;
    digitalWrite(ledPin, ledState); 
  }
  
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP32Client");

  now = millis();
  if (now - lastMeasure > 2000) {
    lastMeasure = now;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float hic = dht.computeHeatIndex(t, h, false);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    
    static char temperatureTemp[7];
    dtostrf(hic, 6, 2, temperatureTemp);
    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);
    
    client.publish("temperature", temperatureTemp);
    client.publish("humidity", humidityTemp);
    
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.println(" *C ");
  }
}
