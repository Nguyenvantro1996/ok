
// Serial.println("chay chuong trinh");
        //playRemote();
       // irrecv.resume();
      
        if(results.value == 0xFFA25D)// ch-
        {
          
          batMay = false;
          Serial.println("tat may");
          
        }
        if(results.value == 0xFFE21D)// ch+
        {
          batMay = true;
           Serial.println("bat may");
             //myRTC.setDS1302Time(00, 15, 15, 7, 24, 11, 2019);
           //irrecv.resume();
        }
        if(results.value == 0xFF22DD) // prev: -- phut
        {
          if(phut == 0) phut = 60;
          phut --;
          Serial.println(phut); 
        }
        if(results.value == 0xFF02FD) // next ++ phut
        {
          if(phut == 60) phut = 0;
          phut ++;
          Serial.println(phut); 
        }
        if(results.value == 0xFFC23D) // play ++ gio;
        {
          if(gio == 24) gio = -1;
          gio ++;
          Serial.println(gio);
        }
//
if(batMay == true)
  {
    myRTC.updateTime();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(myRTC.hours);
    lcd.print(":");
    lcd.print(myRTC.minutes);
    lcd.print(":");
    lcd.print(myRTC.seconds);
   // set time;
    lcd.setCursor(0,1);
    lcd.print("Set time: ");
    lcd.print(gio);
    lcd.print(":");
    lcd.print(phut);
    //
    if(gio == phut)
    {
      //buzzer_play();
      digitalWrite(9, HIGH);
    }
    else
      digitalWrite(9, LOW);
    delay(500);
    
  }
  else 
  {
    
      // tat may;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" may da tat");
      delay(1000);
    
  }


  
