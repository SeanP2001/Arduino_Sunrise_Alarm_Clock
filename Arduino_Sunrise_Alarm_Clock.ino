#include "LiquidCrystal.h"

//LCD SETUP
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
LiquidCrystal lcd(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);

// INITIAL TIME
int h=11;    //Hour
int m=59;    //Mins
int s=40;    //Seconds
int flag=0;  //0=AM 1=PM

// ALARM SETTINGS
int alarm=12; //Alarm Hour
int set=0;   //0=NotSet  1=Set
int type=0;  //0=Time  1=Light

// BACKLIGHT TIMEOUT SETTINGS 
const int Time_light=150;
int bl_TO=Time_light;      // Backlight Time-Out
int bl=10;                 // Backlight pin
const int backlight=120; 

// RTC FOR ACCURACY
static uint32_t last_time, now = 0; // RTC

//----------------------------------------------------------------------------------------------------------------------------------

void setup()
{
  lcd.begin(16,2);       //Start LCD
  now=millis();          //read RTC initial value 
  pinMode(22, OUTPUT);   //LED Pin
}

//----------------------------------------------------------------------------------------------------------------------------------

void loop()
{ 
 int x;              //var x Button Input
 x = analogRead(0);  //Read Buttons
 
 int adj;
 adj = analogRead(10);    //Read Potentiometer
 
 int light;
 light = analogRead(12);   //Read light Level

//----------------------------------------------------------------------------------------------------------------------------------

// PRINT TIME
 lcd.setCursor(2,0);
 if(h<10)lcd.print("0");  // always 2 digits
 lcd.print(h);            //Print Hours
 lcd.print(":");
 if(m<10)lcd.print("0");
 lcd.print(m);            //Print Mins
 lcd.print(":");
 if(s<10)lcd.print("0");
 lcd.print(s);            //Print Secs

 if(flag==0) lcd.print(" AM");
 if(flag==1) lcd.print(" PM");

//----------------------------------------------------------------------------------------------------------------------------------
 

for ( int i=0 ;i<5 ;i++)      //5x Loop
{
  while ((now-last_time)<200) //200ms per Loop
  { 
    now=millis();
  }
 last_time=now; // prepare for next loop 


 //BACKLIGHT TIMEOUT 
 bl_TO--;
 if(bl_TO==0)
 {
  analogWrite(bl,0);// Backlight OFF
  bl_TO++;
 }
 
 // BACKLIGHT ACTIVATION
 if(  ((x < 60)||(x > 400 && x < 600)) & (bl_TO==1)  )
 {
  bl_TO=Time_light;
  analogWrite(bl,backlight);
  // wait until Button released

 }

//--------------------------------------------------------------------------------------------------------------------

//BUTTON RESPONSE

 else
 {
  //IF THE ALARM IS NOT SET
  if (set == 0){            
    
    if (x < 60){                      //RIGHT********************* 

    }
    else if (x < 200){                //UP************************
    
      bl_TO=Time_light;               //Turn ON Backlight
      analogWrite(bl,backlight);

      lcd.setCursor(13, 1);           //Display Output (Alarm ON)
      lcd.print("ON ");
      
      set = 1;                        //Set the Alarm
    }
    else if (x < 400){                //DOWN************************ 
      
      bl_TO=Time_light;               //Turn ON Backlight
      analogWrite(bl,backlight);
      
      lcd.setCursor(13, 1);           //Display Output
      lcd.print("OFF");
      
      set = 0;                        //Turn OFF the Alarm
    }
    else if (x < 600){                //LEFT************************ 

    }
    else if (x < 800){                //SELECT**********************  
      
      bl_TO=Time_light;               //Turn ON Backlight
      analogWrite(bl,backlight);      
     
      lcd.setCursor(0, 1);            //Display Output
      lcd.print("Set Time     ");  
      
      s=0;   
      m=m+1;                          //Increment Mins
    }
  }

//------------------------------------------------------------------------------------------------------------------------------------------

  //IF THE ALARM IS SET
  else if (set == 1){
    if (x < 60){                      //RIGHT********************* 
          
      bl_TO=Time_light;               //Turn ON Backlight
      analogWrite(bl,backlight); 
                           
      lcd.setCursor(0, 1);            //Display Output
      lcd.print("Light Wakeup ");  
      
      type = 1;                       //Sets to Light Mode
    }
    else if (x < 200){                //UP************************
    
      bl_TO=Time_light;               //Turn ON Backlight
      analogWrite(bl,backlight);

      lcd.setCursor(13, 1);           //Display Output (Alarm ON)
      lcd.print("ON ");
      
      set = 1;                        //Set the Alarm
    }
    else if (x < 400){                //DOWN************************ 
      
      bl_TO=Time_light;               //Turn ON Backlight
      analogWrite(bl,backlight);
      
      lcd.setCursor(13, 1);           //Display Output
      lcd.print("OFF");
      
      set = 0;                        //Turn OFF the Alarm
    }
    else if (x < 600){                //LEFT************************ 
      
      bl_TO=Time_light;               //Turn ON Backlight
      analogWrite(bl,backlight);
                 
      lcd.setCursor(0, 1);            //Display Output
      if(alarm<10)lcd.print("0");
      lcd.print(alarm);
      lcd.print(":00"); 
      lcd.print(" Alarm  ");
      
      type = 0;                       //Sets to Time Mode
    }
    else if (x < 800){                //SELECT**********************  
      
      bl_TO=Time_light;               //Turn ON Backlight
      analogWrite(bl,backlight);      
                 
      alarm=alarm+1;                  //Increment Alarm Time
            
      lcd.setCursor(0, 1);            //Display Output (Alarm Time)
      if(alarm<10)lcd.print("0");
      lcd.print(alarm);
      lcd.print(":00"); 
      lcd.print(" Alarm  ");       

      type = 0;                       //Sets to Time Mode
    }
  }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

//TIME OVERFLOW
 if(s==60){
  s=0;
  m=m+1;
 }
 if(m==60)
 {
  m=0;
  h=h+1;
 }
 if(h==13)
 {
  h=1;
 }
 if(alarm==13)
 {
  alarm=1;
 }
 

//---------------------------------------------------------------------------------------------------------------------------------------------------

//UPDATE THE TIME DISPLAYED

 //if((x > 600 && x < 800)&&(set = 0))  // If Select is pressed AND the Alarm is not Set 
 {
  lcd.setCursor(2,0);
  if(h<10)lcd.print("0"); // always 2 digits
  lcd.print(h);           //Print Hours
  lcd.print(":");
  if(m<10)lcd.print("0");
  lcd.print(m);           //Print Mins
  lcd.print(":");
  if(s<10)lcd.print("0");
  lcd.print(s);           //Print Secs

  if(flag==0) lcd.print(" AM");
  if(flag==1) lcd.print(" PM");
 }
 } 

//------------------------------------------------------------------------------------------------------------------------------------------------

//ALARM OUTPUT

//If the alarm is set & (IN TIME MODE) alarm time = the current hour/(IN LIGHT MODE) it is light

 if ((type == 0 && alarm == h && set == 1)||(type == 1 && light < adj && set == 1)){  
  digitalWrite(22, HIGH);     //Turn ON the Light
 }
 else{
  digitalWrite(22, LOW);      //ELSE Keep the light OFF
 }
 
}// End of For Loop

//------------------------------------------------------------------------------------------------------------------------------------------------

// 1000ms LOOP

 s=s+1; //increment seconds
       
//TIME OVERFLOW

 if(s==60)
 {
  s=0;
  m=m+1;
 }
 if(m==60)
 {
  m=0;
  h=h+1;
 }
 if(h==12 && m==0 && s==0)
 {
  flag=flag+1;
  if(flag==2)flag=0;
 }
 if(h==13)
 {
  h=1;
 } 
 if(alarm==13)
 {
  alarm=1;
 }
}
