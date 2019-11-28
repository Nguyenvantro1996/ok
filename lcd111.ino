//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <IRremote.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <virtuabotixRTC.h> 
#include "tongHop.h"
#include <DHT.h>
//#include "buzzer.h"
// set up nhiet do
const int DHTPIN = 2;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

byte degree[8] = {
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};

//
int RECV_PIN = 10;
int thoiGianHenGio = 5*1000; // 5 giay;
int soLanHenGio = 10; // keu 10 lan;
int nhietDoCao = 5;
IRrecv irrecv(RECV_PIN);
decode_results results;
//bool batMay ;
int _henGio = 0;
int start = 1;
int gio = 1;
int phut = 0;
//Wiring SCLK -> 6, I/O -> 7, CE -> 8
//Or CLK -> 6 , DAT -> 7, Reset -> 8

virtuabotixRTC myRTC(6, 7, 8); //If you change the wiring change the pins here also

LiquidCrystal_I2C lcd(0x27,4,5);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup()
{
  // nhiet do
  dht.begin();  
  // bat may
  //batMay = true;
  // set up remote
   pinMode(RECV_PIN, INPUT);  
   pinMode(9, OUTPUT);  
   
   irrecv.enableIRIn(); // Start the receiver
// set up lcd
 // myRTC.setDS1302Time(0, 12, 3, 7, 26, 11, 2019);
 
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("Tro Nguyen");
  Serial.begin(9600);
}


void loop()
{
  //Serial.println("chay chuong trinh");
  // nhiet do
    float h = dht.readHumidity();
    float t = dht.readTemperature();
  //
  if( t >= 34)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ND qua cao: ");
    lcd.print(round(t));
    lcd.print("^C");
    lcd.setCursor(0, 1);
    lcd.print("Phai tat May nhe");
    for(int i = 0; i <nhietDoCao; i ++)
    {
      digitalWrite(9, HIGH);
      delay(200);
      digitalWrite(9, LOW);
      delay(200);
    }
    digitalWrite(9, LOW);
    nhietDoCao  = 0;
    start = 0; // tat may;
    delay(5000);
  }
  else
  {
    nhietDoCao = 5;
    start = 1;
  }
  if(irrecv.decode(&results))
  {
      batMay();
      tatMay();
      henGio();
      caiDatGio();
      irrecv.resume();
      
  }
  
  // bat may de chay;
//  digitalWrite(9, LOW); // tat delay(500);
  delay(500); // phai de ham delay() o day;
 if(start == 1)
 {
        myRTC.updateTime();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(myRTC.hours);
    lcd.print(":");
    lcd.print(myRTC.minutes);
    lcd.print(":");
    lcd.print(myRTC.seconds);
    
    if (isnan(t) || isnan(h)) { // Kiểm tra xem thử việc đọc giá trị có bị thất bại hay không. Hàm isnan bạn xem tại đây http://arduino.vn/reference/isnan
    } 
    else
    {
      lcd.print("  ");
      lcd.print(round(t));
     // lcd.write(1);
      lcd.print("^");
      lcd.print("C");
    }
    lcd.setCursor(14, 0);
    if(_henGio == 1) lcd.print("^^");
   // set time;
    lcd.setCursor(0,1);
    lcd.print("Set time: ");
    lcd.print(gio);
    lcd.print(":");
    lcd.print(phut);
    //
    if((gio == myRTC.hours) && (phut == myRTC.minutes) && _henGio == 1)
    {
      //buzzer_play();
      //digitalWrite(9, HIGH);
     for(int i = 0; i < soLanHenGio; i ++)
     {
        digitalWrite(9, LOW);
        delay(100);
        digitalWrite(9, HIGH);
        delay(100);
     }
     digitalWrite(9, LOW);
     soLanHenGio = 0;
    }
    else
      {
        digitalWrite(9, LOW);
        soLanHenGio = 10;
      }
      
    
    digitalWrite(9, LOW); // tat delay(500);
 }
 else
 {
  //lcd.clear();
  
 }
 
// the end program;
}

// function playRemote
void caiDatGio()
{
  

    int gio = myRTC.hours;
    int phut = myRTC.minutes;
    int giay = myRTC.seconds;
  if(results.value == 0xFF10EF) // 4
  {
    gio --;
    if(gio == -1) gio = 23;
  }
  if(results.value == 0xFF38C7) // 5
  {
    phut --;
    if(phut == -1) phut = 59;
  }
  if(results.value == 0xFF5AA5) // 6
  {
    giay = 0;
  }
  if(results.value == 0xFF42BD) // 7
  {
    gio ++;
    if(gio == 24) gio = 0;
  }
    if(results.value== 0xFF4AB5) // 8
  {
    phut ++ ;
    if(phut == 60) phut = 0;
  }
  if(results.value == 0xFF52AD) // 9
  {
    giay = 0;
  }
  myRTC.setDS1302Time(giay, phut, gio, 7, 26, 11, 2019);
  
}
void henGio()
{
  if(results.value == 0xFFE01F) // vol-
  {
    _henGio = 0;
  }
  if(results.value == 0xFFA857) // vol+
  {
    _henGio = 1;
  }
  if(results.value == 0xFF22DD) // prev: -- phut
        {
           phut --;
          if(phut == -1) phut = 59;
         
          Serial.println(phut); 
        }
        if(results.value == 0xFF02FD) // next ++ phut
        {
           phut ++;
          if(phut == 60) phut = 0;
         
          Serial.println(phut); 
        }
        if(results.value == 0xFFC23D) // play ++ gio;
        {
          if(gio == 24) gio = -1;
          gio ++;
          Serial.println(gio);
        }
}
void tatMay()
{
  if(results.value == 0xFFA25D) // CH-
  {
     // tat may;
      start = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" may da tat");
      delay(1000);
  }
  //irrecv.resume();
}
void batMay()
{
  if(results.value == 0xFFE21D) // CH+
  {
    start = 1;

    delay(500);
  }
  
}
void playRemote() // takes action based on IR code received describing Car MP3 IR codes 
{

  switch(results.value){
  case 0xFFA25D:  
    Serial.println(" CH-            "); 
   // lcd.print("ch-");
     
    break;
  case 0xFF629D:  
    Serial.println(" CH             "); 
    //lcd.print("ch");
    break;
  case 0xFFE21D:  
    Serial.println(" CH+            "); 
      
    break;
  case 0xFF22DD:  
    Serial.println(" blue LED off          "); 
    digitalWrite(13,LOW);
    break;
  case 0xFF02FD:  
    Serial.println(" blue LED on        "); 
    digitalWrite(13, HIGH);
    break;
  case 0xFFC23D:  
    Serial.println(" PLAY/PAUSE     "); 
    break;
  case 0xFFE01F:  
    Serial.println(" VOL-           "); 
    break;
  case 0xFFA857:  
    Serial.println(" VOL+           "); 
    break;
  case 0xFF906F:  
    Serial.println(" EQ             "); 
    break;
  case 0xFF6897:  
    Serial.println(" 0              "); 
    break;
  case 0xFF9867:  
    Serial.println(" 100+           "); 
    break;
  case 0xFFB04F:  
    Serial.println(" 200+           "); 
    break;
  case 0xFF30CF:  
    Serial.println(" 1              "); 
    break;

  case 0xFF18E7:  
    Serial.println(" 2              "); 
    break;

  case 0xFF7A85:  
    Serial.println(" 3              "); 
    break;

  case 0xFF10EF:  
    Serial.println(" 4              "); 
    break;

  case 0xFF38C7:  
    Serial.println(" 5              "); 
    break;

  case 0xFF5AA5:  
    Serial.println(" 6              "); 
    break;

  case 0xFF42BD:  
    Serial.println(" 7              "); 
    break;

  case 0xFF4AB5:  
    Serial.println(" 8              "); 
    break;

  case 0xFF52AD:  
    Serial.println(" 9              "); 
    break;

  default: 
    Serial.print(" unknown button   ");
    Serial.println(results.value, HEX);

  }
  delay(500);
}
