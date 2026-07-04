#include <OneWire.h>
#include <DallasTemperature.h>

#define BLYNK_TEMPLATE_ID "TMPL6mcxn_vcg"
#define BLYNK_TEMPLATE_NAME "Khazzani"
#define BLYNK_AUTH_TOKEN "fb1dQaBHAbf0rCU89uxYZk-X01kED67S"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] ="AHUF";
char pass[] ="313315@hUF";



float Celsius = 0;

const int pump = 4;
const int trigPin = 12;
const int echoPin = 14;
const int turbidity = A0;

//define sound velocity in cm/uS
#define ONE_WIRE_BUS 13
#define SOUND_VELOCITY 0.034
long duration;
long distanceCm;
int turbValue;

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);
BlynkTimer timer;
long v0;
double v1;
int v2;
// void sendLevel()
// {
//   Blynk.virtualWrite(v0,distanceCm);
// }
// void sendTurb()
// {
//   Blynk.virtualWrite(v1,turbValue);
// }
void sendTemp()
{
  Blynk.virtualWrite(v2,Celsius);
}

void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to");
  Serial.print(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  sensors.begin();
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(pump, OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // timer.setInterval(1000L,sendLevel);
  // timer.setInterval(1000L,sendTurb);
  timer.setInterval(1000L,sendTemp);
}

void loop() {
  Blynk.run();
  timer.run();
  sensors.requestTemperatures();
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH,30000);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  

  
  turbValue = analogRead(turbidity);
  turbValue = map(turbValue, 0,1024,100,0);
  

  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  distanceCm=map(distanceCm,0,23,100,0);
  // sendLevel();

  Serial.print("Turbidity: ");
  Serial.println(turbValue);
  // sendTurb();
  
  

  Celsius = sensors.getTempCByIndex(0);

  Serial.print(Celsius);
  Serial.println(" C  ");
  sendTemp();


  if(distanceCm>25)
  {
    digitalWrite(pump, LOW); // turn on pump
  }
  if(distanceCm<5){
    digitalWrite(pump,HIGH);
  }
  delay(1000);
}