#define PRINT(var) Serial.print(#var ":\t"); Serial.print(var); Serial.print('\t')
#define PRINTLN(var) Serial.print(#var ":\t"); Serial.println(var)

const int BUTTON = 12;
const int LED = 14;
int BUTTONstate = 0;
int pinTemp = 13;

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);
}

void loop()
{
  BUTTONstate = digitalRead(BUTTON);
  if (BUTTONstate == HIGH)
  {
    digitalWrite(LED, HIGH);
  } 
  else{
    digitalWrite(LED, LOW);
  }

  int temp = analogRead(pinTemp);  
  temp = temp * 0.48828125;   
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println("C");  
  delay(1000); 
}
