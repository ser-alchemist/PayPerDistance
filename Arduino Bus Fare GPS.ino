#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TinyGPS++.h> 
#include <TimerOne.h>
#include<SPI.h>
#include<MFRC522.h>

#define SS_PIN 53
#define RST_PIN 49
#define SIZE 3
#define GENERAL 0
#define STUDENT 1
#define POLICE 2
#define FARE_PER_KM 2.75
#define MIN_FARE 10.0

const int IRSensorPin = 2;
const int swtchPIN = 3;

int inputState;
int lastInputState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 5;
long time;

double rad = 10.0;
const int  en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3;
const int i2c_addr = 0x3F;
LiquidCrystal_I2C lcd(i2c_addr, en, rw, rs, d4, d5, d6, d7, bl, POSITIVE);

TinyGPSPlus gps;
MFRC522 mfrc522(SS_PIN, RST_PIN);
float radius_of_earth = 63781000;
float lat1, lat2, long1, long2;
double distance = 0.0;
double gpsOffset = 15.0;
bool flag = false;

struct passanger
{
    String uid;
    double balance;
    int pass;
    bool flag;
    double startD;
};
struct passanger passangers[SIZE];

void calc(String id)
{
  for(int i = 0; i < SIZE; i++ )
  {
      if(passangers[i].uid.equalsIgnoreCase(id))
      {
         if((passangers[i].flag) == false)
         {            
            if((passangers[i].balance)>=MIN_FARE)
            {
                Serial.println("Entry gate open");
                lcd.clear();
                lcd.setCursor(2,0);
                lcd.print("W E L C O M E !!");
                lcd.setCursor(5,1);
                lcd.print(passangers[i].uid);
                lcd.setCursor(0,3);
                lcd.print("Balance: ");
                lcd.print(passangers[i].balance);
                lcd.setCursor(18,3);
                lcd.print("TK");
                passangers[i].startD = distance;
                passangers[i].flag = true;
                return;
            }
            else
            {
                 lcd.clear();
                 lcd.setCursor(2,1);
                 lcd.print("Insufficient");
                 lcd.setCursor(5,2);
                 lcd.print("Balance");
                 Serial.println("Insufficient Balance!");
                return;
            }
         }
         else{
             Serial.println("Exit gate open");
             if((passangers[i].pass) == GENERAL)
             {
                 double travelledD = distance - passangers[i].startD;
                 double fare = travelledD * FARE_PER_KM;
                 if(fare < MIN_FARE)
                 {
                 fare = MIN_FARE;
                 }
                 passangers[i].balance -= fare;

                 lcd.clear();
                 lcd.setCursor(1,0);
                 lcd.print("You have travelled");
                 lcd.setCursor(4,1);
                 lcd.print(travelledD);
                 lcd.setCursor(12,1);
                 lcd.print("Meter");
                 lcd.setCursor(0,2);
                 lcd.print("Fare is: ");
                 lcd.setCursor(12,2);
                 lcd.print(fare);
                 lcd.setCursor(16,2);
                 lcd.print("Taka");
                  lcd.setCursor(0,3);
                 lcd.print("New Balance:");
                  lcd.setCursor(14,3);
                 lcd.print(passangers[i].balance);
                  lcd.setCursor(18,3);
                 lcd.print("Tk");


                 
                 Serial.print("You have travelled ");
                 Serial.print(travelledD);
                 Serial.print("m. Fare is taka: ");
                 Serial.println(fare);
                 Serial.print("New Balance: ");
                 Serial.print(passangers[i].balance);
                 Serial.println("");
                 passangers[i].flag = false;
                 return;
             }
             else if((passangers[i].pass) == STUDENT)
             {

                 double travelledD = distance - passangers[i].startD;
                 double fare = travelledD * FARE_PER_KM * 0.5;
                 if(fare < MIN_FARE)
                 {
                 fare = MIN_FARE * 0.5;
                 }
                 passangers[i].balance -= fare;


                 lcd.clear();
                 lcd.setCursor(1,0);
                 lcd.print("You have travelled");
                 lcd.setCursor(4,1);
                 lcd.print(travelledD);
                 lcd.setCursor(12,1);
                 lcd.print("Meter");
                 lcd.setCursor(0,2);
                 lcd.print("Fare is: ");
                 lcd.setCursor(12,2);
                 lcd.print(fare);
                 lcd.setCursor(16,2);
                 lcd.print("Taka");
                  lcd.setCursor(0,3);
                 lcd.print("New Balance:");
                  lcd.setCursor(14,3);
                 lcd.print(passangers[i].balance);
                  lcd.setCursor(18,3);
                 lcd.print("Tk");

                 
                 Serial.print("You have travelled ");
                 Serial.print(travelledD);
                 Serial.print("m. Fare is taka: ");
                 Serial.println(fare);
                 Serial.print("New Balance: ");
                 Serial.print(passangers[i].balance);
                 Serial.println("");
                 passangers[i].flag = false;
                 return;
             }
             else if((passangers[i].pass) == POLICE)
             {
                 double travelledD = distance - passangers[i].startD;
                 double fare = 0.0;                 
                 passangers[i].balance -= fare;

                 lcd.clear();
                 lcd.setCursor(1,0);
                 lcd.print("You have travelled");
                 lcd.setCursor(4,1);
                 lcd.print(travelledD);
                 lcd.setCursor(12,1);
                 lcd.print("Meter");
                 lcd.setCursor(0,2);
                 lcd.print("Fare is: ");
                 lcd.setCursor(12,2);
                 lcd.print(fare);
                 lcd.setCursor(16,2);
                 lcd.print("Taka");
                  lcd.setCursor(0,3);
                 lcd.print("New Balance:");
                  lcd.setCursor(14,3);
                 lcd.print(passangers[i].balance);
                  lcd.setCursor(18,3);
                 lcd.print("Tk");

                 
                 Serial.print("You have travelled ");
                 Serial.print(travelledD);
                 Serial.print("m. Fare is taka: ");
                 Serial.println(fare);
                 Serial.print("New Balance: ");
                 Serial.print(passangers[i].balance);
                 Serial.println("");
                 passangers[i].flag = false;
                 return;
             }
         }      
       }  
    }
    Serial.println("Card not registered");
    lcd.clear();
     lcd.setCursor(0,1);
     lcd.print("Card not registered");  
       
}


double distance_in_meter(float start_lat, float start_long, float end_lat, float end_long) {

      start_lat/= 180 / PI; 
      start_long/= 180 / PI;
      end_lat/= 180 / PI;
      end_long/= 180 / PI;
      
      // haversine formula 
      float a = pow(sin((end_lat-start_lat)/2), 2) + cos(start_lat) * cos(end_lat) * pow(sin((end_long-start_long)/2), 2);
      float answer = radius_of_earth * 2 * atan2(sqrt(a), sqrt(1-a));
      return (answer/gpsOffset);
}

void displayInfo()
{
  //Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    if(!flag)
    {
      lat1 = gps.location.lat();
      long1 = gps.location.lng();
      flag = true;
    }
    else{
      lat2 = gps.location.lat();
      long2 = gps.location.lng();
      distance += distance_in_meter(lat1, long1, lat2, long2);
      lat1 = lat2;
      long1 = long2;
    }
  }
  
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  lcd.begin(20,4);
  SPI.begin();
  mfrc522.PCD_Init();

  lcd.clear();
  lcd.setCursor(1,1);
  lcd.print("Place your card");
  lcd.setCursor(1,2);
  lcd.print("on the RFID reader..");
  Timer1.initialize(1000000);  // Set the timer to 60 rpm, 1,000,000 microseconds (1 second)
  //Timer1.attachInterrupt(timerIsr);  // Attach the service routine here
  
  Serial.println("RFID reading UID...");
  passangers[0] = {"45 76 7B 89", 2.20, GENERAL, false, 0.0};
  passangers[1] = {"FA 8A 8B BE", 2399.8, STUDENT, false, 0.0};
  passangers[2] = {"AC A0 61 83", 25.89, POLICE, false, 0.0};
}


void loop() {
  // put your main code here, to run repeatedly:

  if(digitalRead(swtchPIN) == HIGH)
  {
    //9Serial.println("GPS distance ");
    //Serial.print(distance);
    if (Serial1.available() > 0){
      if (gps.encode(Serial1.read()))
      {
        displayInfo();
        delay(500);
      }
    }
  }

  else{
    time = millis();
    int currentSwitchState = digitalRead(IRSensorPin);
  
    if (currentSwitchState != lastInputState) {
      lastDebounceTime = millis();
    }
  
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (currentSwitchState != inputState) {
        inputState = currentSwitchState;
        if (inputState == LOW) {
        //  digitalWrite(ledPin, LOW);
          distance += 10;
          Serial.print("d: ");
          Serial.println(distance); // Real RPM from sensor
        }
      }
    }
    lastInputState = currentSwitchState;
  }

  
  if(mfrc522.PICC_IsNewCardPresent())
  {
    if(mfrc522.PICC_ReadCardSerial())
    {
      tone(4,500);
      delay(100);
      noTone(4);
      String readUid= "";
      
      Serial.print("UID : ");
      for(byte i = 0; i <mfrc522.uid.size; i++)
      { 
        //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
        //Serial.print(mfrc522.uid.uidByte[i], HEX);
        readUid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        readUid.concat(String(mfrc522.uid.uidByte[i], HEX));

      }
      readUid.toUpperCase();
      Serial.print(readUid);
      Serial.println();
      calc(readUid.substring(1));
      mfrc522.PICC_HaltA();
      delay(1000);
    }
  }
}
