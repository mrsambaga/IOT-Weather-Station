#include "DHT.h"
#define DHTPIN 18
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// set pin numbers
const int buttonPin = 4;  // the number of the pushbutton pin
const int ledPin =  5;    // the number of the LED pin
char in;
int ledState = LOW;     // the current state of LED
int lastButtonState;    // the previous state of button
int currentButtonState; // the current state of button

long now;
long lastMeasure = 0;
int interval = 2000;

void setup() {
  Serial.begin(115200);  
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  
  currentButtonState = digitalRead(buttonPin);
  
  dht.begin();
}

void loop() {
  lastButtonState    = currentButtonState;   
  currentButtonState = digitalRead(buttonPin);

  if(lastButtonState == HIGH && currentButtonState == LOW) {
    Serial.println("The button is pressed");
    
    ledState = !ledState;
    digitalWrite(ledPin, ledState); 
  }

  if(Serial.available())  {
    in = Serial.read();
    if(in == 48) {
      digitalWrite(ledPin, LOW);
    }
    else if(in == 49) {
      digitalWrite(ledPin, HIGH);
    }
  }

  now = millis();
  if ((now - lastMeasure) > interval) {

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  lastMeasure = now;

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(" *C ");
  }
}
