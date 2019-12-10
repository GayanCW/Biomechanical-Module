/*left to right 1,2,3,4

  1= cancel - 1023 byte
  2= up     - 1004 byte
  3= down   - 995 byte
  4= enter  - 985 byte 

  A0 = button input
  A1 = external sensor
  A2 = LDR 
  A3 = voltage
   
  V 86
  P 80
  I 73
  E 69
  T 84
  F 70 */
/////////////////////////////////////////////////////////////////////////
byte up_0[8]={
  B00000,
  B00000,
  B00000,
  B00001,
  B00011,
  B00111,
  B01111,
  B11111
};
byte up_1[8]={
  B00000,
  B00000,
  B00000,
  B10000,
  B11000,
  B11100,
  B11110,
  B11111
};
byte down_0[8]={
  B00000,
  B00000,
  B00000,
  B11111,
  B01111,
  B00111,
  B00011,
  B00001
};
byte down_1[8]={
  B00000,
  B00000,
  B00000,
  B11111,
  B11110,
  B11100,
  B11000,
  B10000
};
/////////////////////////////////////////////////////////////////////////
#include <LiquidCrystal.h>
#include "HX711.h"
#include <EEPROM.h>

HX711 scale(10,11); // DT,SCK
LiquidCrystal lcd(8, 7, 5, 4, 3, 2);

#define voPin 9
#define piezo 13
#define sound_pin 12
#define backLight_pin 6

/////////////////////////////////////////////////////////////////////
float calibration_factor = EEPROM.read(1)*100.00;   // this calibration factor is adjusted according to my load cell
float minimum_voltage = EEPROM.read(2)/10.00;       // minimum_voltage in EEPROM index 2(8.5)
byte V_read=EEPROM.read(3); 
byte P_read=EEPROM.read(4);
float minimum_weight_range=EEPROM.read(5)/10.0;
float maximum_weight_range=EEPROM.read(6)/10.0;
byte sound_sensor=EEPROM.read(7);

float units=0;
float voltage=12;

uint8_t sound_signal=false;
uint8_t pressed_state=false;
uint8_t pressed_signal=false;

uint32_t pressStart_time;
uint32_t pressed_time;
uint32_t path=0;

unsigned int path_array[8];
unsigned int button_value;
unsigned int presentage;
unsigned int raw=0, stay_page=0, stay_path=0;
unsigned int maximum_pages=8, maximum_raw=8;
unsigned int cal_1=0,cal_2=0;
unsigned int method_NO=0;
unsigned int time_delay=100;    // button hold delay
unsigned int backLight_time=30; // seconds
unsigned int backLight_startTime=millis();
unsigned int beep_delay=millis();

byte contrast=0;
/////////////////////////////////////////////////////////////////////

void setup() {
  //Serial.begin(115200);
////////////////////////////////////////////  
  pinMode(piezo,OUTPUT);
  pinMode(sound_pin,INPUT);
  pinMode(backLight_pin,OUTPUT);  
  pinMode(voPin,OUTPUT);  
////////////////////////////////////////////  
  lcd.begin(20, 4);
  analogWrite(voPin,150);
////////////////////////////////////////////
  lcd.createChar(1, up_0);
  lcd.createChar(2, up_1);
  lcd.createChar(3,down_0);
  lcd.createChar(4,down_1);
////////////////////////////////////////////
  scale.set_scale(calibration_factor);
  scale.tare();  //Reset the scale to 0
////////////////////////////////////////////
  
  analogWrite(backLight_pin,60); 
////////////basic////////////////
  
  welcome_Note();
}


char buttonInput(){
//////////////////////press_Time/////////////////////////////
  if(analogRead(A0)>0 && pressed_state==false){
    pressStart_time=millis();
    button_value=analogRead(A0);
    pressed_state=true;   
  }
  if(analogRead(A0)==0 && pressed_state==true){
    pressed_time=millis()-pressStart_time;
    pressed_state=false;
  }
/////////////////////////////////////////////////////////////
  if(button_value>1018 && button_value<=1023){
      if(pressed_time>=time_delay*5){
          backLight_startTime=millis(); 
          pressed_signal=true;  pressed_time=0;
          return 'C';  // Cancel    
      }
      if(pressed_time>=time_delay && pressed_time<time_delay*5){ 
          backLight_startTime=millis();
          pressed_signal=true;  pressed_time=0;
          return 'B';  // Back    
      }
  }
  if(button_value>1001 && button_value<=1007){
      if(pressed_time>=time_delay && pressed_time<time_delay*5){ 
          backLight_startTime=millis();
          pressed_signal=true;  pressed_time=0;
          return 'U';  // Up   
      }
  }
  if(button_value>990 && button_value<=998){
      if(pressed_time>=time_delay && pressed_time<time_delay*5){
          backLight_startTime=millis();
          pressed_signal=true;  pressed_time=0; 
          return 'D'; // Down  
      }
  }
  if(button_value>972 && button_value<=978){
      if(pressed_time>=time_delay*5){
          backLight_startTime=millis();
          pressed_signal=true;  pressed_time=0;
          return 'M';  // Menu    
      }
      if(pressed_time>=time_delay && pressed_time<time_delay*5){
          backLight_startTime=millis();
          pressed_signal=true;  pressed_time=0;
          return 'E';  // Enter     
      }
  }           
}

void keys(int PATH){
      lcd.setCursor(0,3);
      lcd.print("Back");
      
  if(PATH==1 || PATH==2){
        lcd.setCursor(4,3);
        lcd.write("  ");
      lcd.setCursor(6,3);
      lcd.write(byte(1));
      lcd.setCursor(7,3);
      lcd.write(byte(2));        
        lcd.setCursor(8,3);
        lcd.write("    ");      
      lcd.setCursor(12,3);
      lcd.write(byte(3));
      lcd.setCursor(13,3);
      lcd.write(byte(4));        
        lcd.setCursor(14,3);
        lcd.write("  ");
  }
  if(PATH==1){
      lcd.setCursor(16,3);
      lcd.print("Menu");  
  }
  if(PATH==2 || PATH==3 || PATH==4){
      lcd.setCursor(16,3);
      lcd.print("<Ok>");  
  }
  if(PATH==3){
      lcd.setCursor(4,3);
      lcd.print("  ");
      lcd.print("<==");
      lcd.print("  ");
      lcd.print("==>"); 
      lcd.print("  ");
  }
  if(PATH==4){
      lcd.setCursor(4,3);
      lcd.print("            ");
  }       
}

void home_(){
  V_read=EEPROM.read(3); 
  P_read=EEPROM.read(4);
  
  if(V_read==170){
      battery('V',true);  
  }
  if(V_read==156){
      battery('V',false);
  }
  if(P_read==164){
      battery('P',true);
  }
  if(P_read==150){
      battery('P',false);
  }

  if(voltage<=minimum_voltage)  method_NO=3;
  if(method_NO==3 && voltage>minimum_voltage)  method_NO=0;
  if(voltage>minimum_voltage)  method_NO=method_NO;
  
    lcd.setCursor(0,0);
    //lcd.print(path);
    //lcd.print("/");
    lcd.print(stay_page);    
}


void weight_Scale(int path_0,int path_1){  

      scale.set_scale(calibration_factor);
      units = scale.get_units();
          if (units <= 0) units = 0.0; 

  if(path_0==1 && path_1==0){ // check weight
      lcd.setCursor(0,1);
      lcd.print("Weight  : ");
      lcd.print(units,1);
      lcd.print(" kg   ");
      
      lcd.setCursor(0,2); 
      lcd.print("C factor: ");
      lcd.print(calibration_factor);
  }
  
  if(path_0==2 && path_1==0){ // calibrate weight scale
    char pressed_button=buttonInput();     
      if(pressed_button=='U' && pressed_signal==true){
          if(calibration_factor>=0 && calibration_factor<25500){
              calibration_factor += 100;
              pressed_signal=false; // reset press button
          } 
      }
      if(pressed_button=='D' && pressed_signal==true){
          if(calibration_factor>0 && calibration_factor<=25500){
              calibration_factor -= 100;
              pressed_signal=false; // reset press button
          }
      }
      if(pressed_button=='E'&& pressed_signal==true){
          if(calibration_factor>=0 && calibration_factor<=25500){
              calibration_factor=calibration_factor/100;
              EEPROM.write(1,calibration_factor); // save calibration_factor in EEPROM index 1
              calibration_factor=EEPROM.read(1)*100;  
                  ok();
                  back_();
                  method_NO=0;
                  pressed_signal=false; // reset press button
          }
      }    
  }
  if(path_0==3){ // set weight range
    char pressed_button=buttonInput();
      lcd.setCursor(1,1);
      lcd.print("Minimum Range:-");
      lcd.print(minimum_weight_range);
      lcd.setCursor(1,2);
      lcd.print("Maximum Range:+");
      lcd.print(maximum_weight_range);
      if(pressed_button=='D' && pressed_signal==true){
          if(path_1==1 && minimum_weight_range>0.0){
              minimum_weight_range+=0.5;
              lcd.setCursor(0,1);
              lcd.print(">");
              lcd.setCursor(16,1);
              lcd.print(minimum_weight_range);
              pressed_signal=false;  
          }
          if(path_1==2 && maximum_weight_range>0.0){
              maximum_weight_range+=0.5;
              lcd.setCursor(0,2);
              lcd.print(">");
              lcd.setCursor(16,2);
              lcd.print(maximum_weight_range);
              pressed_signal=false;  
          }            
      }
      
      if(pressed_button=='U' && pressed_signal==true){
          if(path_1==1 && minimum_weight_range>=0.0){
              minimum_weight_range-=0.5;
              lcd.setCursor(0,1);
              lcd.print(">");
              lcd.setCursor(16,1);
              lcd.print(minimum_weight_range);
              pressed_signal=false;  
          }
          if(path_1==2 && maximum_weight_range>=0.0){
              maximum_weight_range-=0.5;
              lcd.setCursor(0,2);
              lcd.print(">");
              lcd.setCursor(16,2);
              lcd.print(maximum_weight_range);
              pressed_signal=false; 
          }                      
      }
      if(pressed_button=='E' && pressed_signal==true){
          minimum_weight_range*=10; 
          maximum_weight_range*=10;
          EEPROM.write(5,minimum_weight_range);
          EEPROM.write(6,maximum_weight_range);
          minimum_weight_range=EEPROM.read(5)/10.0;
          maximum_weight_range=EEPROM.read(6)/10.0;
              ok();
              back_(); 
              method_NO=0;
              pressed_signal=false;
      }          
  }  
}

void sound_Sensor(int path_0){
  sound_sensor=EEPROM.read(7);
  
  if(sound_sensor==157)  sound_signal = digitalRead(sound_pin);
  if(sound_sensor==153)  sound_signal = digitalRead(sound_pin); // (I use external sens using internal sens)
      
  if(path_0==1){
          if(sound_signal==HIGH) sound_signal=true;
              if(sound_signal==true){
                  for(byte i=0; i<4; i++){
                    beep_Tone(true,100);
                    beep_Tone(false,10);
                  }
                  delay(500);
                  sound_signal=false;
              }
          else{
              beep_Tone(false,10);
          }
  }
}

void battery(char path_1, int path_2){
 
  //  Diode voltage compensation 100 //
  //  5v equals 1023 byte
  //  Equatorial resistance 2.5
  //  maximum voltage 12.6v
  //  minimum voltage
  
    int sensorValue = analogRead(A3);
  if(cal_1==0 || cal_1==2000){
    voltage = (sensorValue) * (5.00 / 1023.00) * 2.50;                    //convert the sensorValue to a true voltage.  
    presentage = 10+(voltage-minimum_voltage)*90/(12.6-minimum_voltage);  //convert the sensorValue to a true presentage.
    cal_1=0;
  }  
  if(path_1=='V'){ // voltage (on/off)
      if(path_2==true){
          lcd.setCursor(10,0);
          lcd.print(voltage,1); 
          lcd.print(" V ");
              EEPROM.write(3,170); //VT=voltage true
      }
      if(path_2==false){
          lcd.setCursor(10,0);
          lcd.print("      ");
              EEPROM.write(3,156); //VT=voltage false
      }          
  }

  if(path_1=='P'){  // presentage (on/off)
      if(path_2==true){
          if(presentage>=100){
              lcd.setCursor(16,0); 
              lcd.print(presentage);
              lcd.print("%"); 
          }
          else{
              lcd.setCursor(17,0); 
              lcd.print(presentage);
              lcd.print("%");
          } 
              EEPROM.write(4,164); //PT=presentage true             
      }
      if(path_2==false){
          lcd.setCursor(16,0);
          lcd.print("    ");
              EEPROM.write(4,150); //PF=presentage false
      }           
  }

  if(path_1=='1' && path_2==0){ // low battery voltage
    char pressed_button=buttonInput();
      lcd.setCursor(0,1);
      lcd.print("Set Minimum Voltage ");
      lcd.setCursor(0,2); 
      lcd.print("       ");
      lcd.setCursor(7,2);
      lcd.print(minimum_voltage);
      lcd.print(" V      ");
    
      if(pressed_button=='U' && pressed_signal==true){
          if(minimum_voltage>=0 && minimum_voltage<12.5){
              minimum_voltage+=0.5; 
              pressed_signal=false;
          }  
      }
      if(pressed_button=='D' && pressed_signal==true){
          if(minimum_voltage>0 && minimum_voltage<=12.5){
              minimum_voltage-=0.5;
              pressed_signal=false;
          }
      }
      if(pressed_button=='E' && pressed_signal==true){
          if(minimum_voltage>=0 && minimum_voltage<=12.5){
              minimum_voltage=minimum_voltage*10; 
              EEPROM.write(2,minimum_voltage);      // save minimum_voltage in EEPROM index 2
              minimum_voltage=EEPROM.read(2)/10.00; 
                  ok();
                  back_();
                  method_NO=0;
                  pressed_signal=false; // reset press button
          }
      }
  }
    cal_1++;
}

void welcome_Note(){
//////////////////////////////Start//////////////////////////////////////
  lcd.setCursor(2,0);
  lcd.print("2D Biomechanical");
  lcd.setCursor(7,1);
  lcd.print("Model");
  lcd.setCursor(3,2);
  lcd.print("Of Block Start");
  for(byte j=4; j>0; j-=2){
      for(byte i=0; i<j; i++){  
          beep_Tone(true,100);
          beep_Tone(false,10);
      }
      delay(1000);
  }
  for(byte j=3; j>0; j--){
      for(byte i=1; i<17; i++){
          lcd.setCursor(i,3);
          lcd.print("==>");
          delay(70);
          lcd.setCursor(i-1,3);
          lcd.print(" ");
          if(i==16) lcd.print("   ");        
      }
  }
  lcd.clear();
/////////////////////////////////////////////////////////////////////////
}

void beep_Tone(boolean path_0,int dT){
    tone(piezo,(path_0*2000),100);
    delay(dT);    
}

void backLight_Auto_MODE(){ //backLight_signal backLight_time
  //maximum evening val=1013
  //minimum evening val=680

    /*int val=analogRead(A2);
        if(millis()-backLight_startTime<=backLight_time*1000 && contrast>=0){
            if(contrast==0) contrast=10;
            //if(val>=100 && val<=1013 && (cal_2==0 || cal_2==20)) contrast=(val/3.7265)-16.8348;  913 140
            if(val>=100 && val<=1013 && (cal_2==0 || cal_2==20)) contrast=(val/6.5214)-5.3341;
            if(cal_2==20) cal_2=0;
                cal_2++;
        } */           
        if(millis()-backLight_startTime>backLight_time*1000 && contrast>0){      
            if(contrast>0) contrast--;
            if(contrast==0) contrast=0;
            beep_delay=millis();          
        }
        /*if(millis()-backLight_startTime>=backLight_time*3000 && millis()-beep_delay>10000){      
           for(byte i=0; i<2; i++){
             beep_Tone(true,100);
             beep_Tone(false,10);
           }
           beep_delay=millis();           
        }*/        
           // analogWrite(backLight_pin,contrast);
}
