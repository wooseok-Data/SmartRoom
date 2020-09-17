#include <CapacitiveSensor.h>
#include <Servo.h>

Servo myServo;
CapacitiveSensor capSensor = CapacitiveSensor(4,2);

// Bedroom Sensor : When user laid on Bed
//Touch Sensor
int threshold = 1000 ;
// Light Sensor
int LightValue;
// Status LED
const int LightCheck = A1;
const int LED_Room = 11;
const int BoardLED = 13;
//Temperature Check
const int Temperature = A0;
// Air Conditioning, Notice
const int COOL = 9;
const int WARM = 8;
//Air Condition Setting
const int AirConditionControl = A2;
int setting; //0 : AUTO ,1 : Warm ,2 : Cool
int potVal;
int angle;

void setup() {
  Serial.begin(9600); 
  
  //If user already fell aleep, but when the lights on
  pinMode(LED_Room , OUTPUT); 
  pinMode(BoardLED, OUTPUT);
  
  //Findout Room's Light is on or not
  digitalWrite(BoardLED, HIGH);
  
  //Alarm Button
  while(millis() < 3000)
  {
    LightValue = analogRead(A1);
  }
  digitalWrite(BoardLED, LOW);
  
  //AirCondition, cool, warm, auto
  myServo.attach(7);
  pinMode(COOL, OUTPUT);
  pinMode(WARM, OUTPUT);
}

void loop() {
  //Check AirConditioner Setting
  potVal = analogRead(AirConditionControl);
  angle = map(potVal, 0,1023,0,179);
  myServo.write(angle);
  
  Serial.print(">>");
  
  if(angle <60){
    setting = 0; //AUTO
  }else if(angle <90){
    setting = 1; //Warmer
  }else{
    setting = 2; // Cooler
  }  
  //1023, 376, 0
  
  // put your main code here, to run repeatedly:
  long Touch_sensorValue = capSensor.capacitiveSensor(300);
  
  if(Touch_sensorValue > threshold)
  {   
    Serial.println("START SLEEPING");
    Serial.println("=======================");
    //if user laid on Bed, Lights on
    digitalWrite(LED_Room, HIGH);
    //Chech whether light is on or not
    Serial.println("Recognizing Whether User is Sleeping");
    for(int waiting = 0; waiting <3; waiting++)
    {
      Serial.print("...");
      delay("500");
    }
    LightValue = analogRead(LightCheck);
    //If Light is on, Turn on the alram if user wanted
    //And turn off
//    Serial.println(LightValue);
    if (LightValue > 600){
        Serial.println("Turning off the Light");
        tone(12,80,440);
        delay(1000);
        digitalWrite(LED_Room, LOW);
        delay(300);
        digitalWrite(LED_Room, HIGH);
        delay(300);
        digitalWrite(LED_Room, LOW);
        delay(300);
        digitalWrite(LED_Room, HIGH);
        delay(300);
        digitalWrite(LED_Room, LOW);
    }    
    else{
      delay(2000);
      digitalWrite(LED_Room, LOW);         
    }
    delay(500 );
    Serial.println("Good NIGHT!");
    delay(500);
    Serial.println("Air Conditioning Starting");
    delay(500);
    int temp_sensor = analogRead(Temperature);
    float voltage = (temp_sensor/1024.0) * 5.0;
    float temperature = (voltage - 0.5) * 100;
    Serial.print("Temperature : ");
    Serial.println(temperature);
    
    if(setting == 0){
      Serial.println("***AUTO***");
      if(temperature < 22)
      {
        Serial.println(" [WARMING]  ");
        digitalWrite(WARM, HIGH);
      }
      else if (temperature > 25)
      {
        Serial.println(" [COOLING]  ");
        digitalWrite(COOL, HIGH);
      }
      else{
        digitalWrite(WARM, LOW);
        digitalWrite(COOL, LOW);
      }
    }else if(setting == 1){
      Serial.println(" [WARMING]  ");
      digitalWrite(WARM, HIGH);
    }else{
      Serial.println(" [COOLING]  ");
      digitalWrite(COOL, HIGH);
    }
  Serial.println("=======================");
  }else{
    digitalWrite(WARM, LOW);
    digitalWrite(COOL, LOW);
  }
  
  delay(3000);
  digitalWrite(WARM, LOW);
  digitalWrite(COOL, LOW);
}
