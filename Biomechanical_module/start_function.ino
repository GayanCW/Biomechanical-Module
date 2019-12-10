uint8_t set_state;
uint8_t sound_get_state;
uint8_t start_state;
uint8_t false_start_state;
uint8_t reaction_state;
uint8_t set_signal;
uint8_t last_time;

uint32_t time1=micros();
uint32_t time2=micros();

uint32_t start_time=millis();
int32_t unit_get_time=millis();
uint32_t last_weight_get_time=millis();

float set_arr[10];
float unit_arr[20];
uint32_t time_arr[20];

float hold_weight;
float last_weight;

byte count_0=0;
byte count_1=0;
byte count_2=0;

uint32_t x,x1,x2;
float y,y1,y2;

void start_FUNCTION(){

    if(micros()-time1>=100000 && reaction_state==false){
      time1=micros();
        weight_Scale(0,0);
          if(count_0<1 && set_signal==true){
              set_arr[count_0]=units; 
              count_0++;           
          }
          if(count_0==1 && set_signal==true){
              hold_weight=units;
              Serial.print("\t\t");
              Serial.println(hold_weight);
              set_state=true;
              set_signal=false;
              count_0=0;  
          }
          if(start_state==true && set_state==true && count_0<20){
              unit_arr[count_0]=units;
                unit_get_time=(millis()-start_time)-100;
                if(unit_get_time<100)  unit_get_time=0;
              time_arr[count_0]=unit_get_time;
                count_0++;
          }
          if(start_state==true && set_state==true && count_0==20){
              reaction_state=true;
              count_0=0;
              method_NO=10;          
          }
        
    }
    if(micros()-time2>=10 && false_start_state==false && start_state==false){
      time2=micros();
        if(set_state==false && set_signal==false){
          char pressed_button=buttonInput();
            if(pressed_button=='E'){
                lcd.clear();
                lcd.setCursor(4,1);
                lcd.print("Set State OK");
                lcd.setCursor(1,2);
                lcd.print("Waiting For Start");
                    set_signal=true;
                    pressed_signal=false;  
            }
            if(pressed_button=='B'){
                set_signal=false;
                pressed_signal=false; 
                method_NO=0;
                back_(); 
            }          
        }  
        if(set_state==true && sound_get_state==false){
          sound_Sensor(0);
            if(sound_signal==HIGH){
                sound_get_state=true;
                last_weight=units;
                last_time=0;
                last_weight_get_time=millis();
                
            }
            if(set_state==true && sound_get_state==true){
                if(hold_weight+maximum_weight_range>units && hold_weight-minimum_weight_range<units){ //in to the range
                    //lcd.clear();
                    //lcd.setCursor(4,1);
                    //lcd.print("Set State OK");
                        start_state=true;
                        start_time=millis();
                        false_start_state=false;
                        sound_get_state=false;  
                }            
            }
            if(set_state==true && sound_get_state==false){
                if(hold_weight+maximum_weight_range<units || hold_weight-minimum_weight_range>units){ //out to the range
                    lcd.clear();
                    lcd.setCursor(1,1);
                    lcd.print("False Start....!!!");
                    lcd.setCursor(1,2);
                    lcd.print("Reacted Befor Start");
                    lcd.setCursor(7,3);
                    lcd.print("Signal");
                        start_state=false;
                        false_start_state=true; 
                        sound_get_state=false;
                        method_NO=11; 
                }            
            }
        }
    }  
} 

void true_START(){
    
  Serial.begin(115200);
  delay(1000);
  Serial.print("===================================================\n");
  Serial.print("--------------2D Bio-mechanical Model--------------\n");
  Serial.print("Hold Weight= ");
  Serial.print(hold_weight);
  Serial.print("\n");
  Serial.print("Last Weight= ");
  Serial.print(last_weight);
  Serial.print("\n\n");
    //last_weight=hold_weight; // new calculation
      Serial.print(last_time);
      Serial.print("\t");
      Serial.println(last_weight);
    while(count_2<20){
      Serial.print(time_arr[count_2]); 
      Serial.print("\t");
      Serial.println(unit_arr[count_2]);
        count_2++;  
    }
    
    if(count_2==20){
        calculation();
        Serial.print("\n\n---------------Analyse Reaction Time---------------\n\n");
        
        Serial.print("x1= ");
        Serial.print(x1);
        Serial.print("\t\ty1= ");
        Serial.print(y1);
        
        Serial.print("\nx2= ");
        Serial.print(x2);
        Serial.print("\t\ty2= ");
        Serial.print(y2);
         
        Serial.print("\n\nReacted Weight= ");
        Serial.print(y);
        Serial.print(" Kg");
        Serial.print("\nReaction Time = ");
        Serial.print(x);
        Serial.print(" milliseconds");
        count_2=21;
    }
    if(count_2==21 && x>=100){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Good Start...!!!");
        lcd.setCursor(0,2);
        lcd.print("Reaction Time:");
        lcd.print(x);
        lcd.print("ms");
        lcd.setCursor(5,3);
        lcd.print("----------");
            Serial.print("\n\n---------------------------------------------------\n");
            Serial.print("Good Start...!!!");
            Serial.print("\n---------------------------------------------------\n\n");
            Serial.print("Your Data Analysed Successfully.......!!!\n");
            Serial.print("===================================================\n");  
            Serial.end();
        delay(15000);
        method_NO=0;
        reset_start_DATA();
        back_();      
    }
    if(count_2==21 && x<100){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("False Start...!!!");
        lcd.setCursor(0,2);
        lcd.print("Reaction Time:");
        lcd.print(x);
        lcd.print("ms");
        lcd.setCursor(5,3);
        lcd.print("----------");
            Serial.print("\n\n---------------------------------------------------\n");
            Serial.print("False Start...!!!");
            Serial.print("\n---------------------------------------------------\n\n");
            Serial.print("Your Data Analysed Successfully.......!!!\n");
            Serial.print("===================================================\n");  
            Serial.end();
        //delay(10000);
        method_NO=11;
        reset_start_DATA();     
    }
   
}

void false_START(){
    if(micros()-time1>150000){
      time1=micros(); 
        beep_Tone(true,100); 
        beep_Tone(false,50); 
    }
    if(micros()-time2>16){
      time2=micros();
        char pressed_button=buttonInput();  

        if(pressed_button=='B'){
            beep_Tone(false,50);
            method_NO=0;
            reset_start_DATA();
            back_();         
        }
    }
}

void calculation(){
    while(count_1<20){
        if(last_weight+maximum_weight_range<unit_arr[count_1]){
            if(count_1==0 && count_1<20){
                y1=last_weight;
                x1=0;
                x2=time_arr[count_1];
                y2=unit_arr[count_1];
                y=last_weight+maximum_weight_range; 
                    count_1=20; 
            }
            if(count_1>0 && count_1<20){
                x1=time_arr[count_1-1];
                y1=unit_arr[count_1-1];
                x2=time_arr[count_1];
                y2=unit_arr[count_1];
                y=last_weight+maximum_weight_range; 
                    count_1=20; 
            }  
        }
        
        if(last_weight-minimum_weight_range>unit_arr[count_1]){
            if(count_1==0 && count_1<20){
                y1=last_weight;
                x1=0;
                x2=time_arr[count_1];
                y2=unit_arr[count_1];
                y=last_weight-minimum_weight_range;
                    count_1=20;  
            }
            if(count_1>0 && count_1<20){
                x1=time_arr[count_1-1];
                y1=unit_arr[count_1-1];
                x2=time_arr[count_1];
                y2=unit_arr[count_1];
                y=last_weight-minimum_weight_range;
                    count_1=20;  
            }
        
        }
        if((last_weight+maximum_weight_range>=unit_arr[count_1] || last_weight-minimum_weight_range<=unit_arr[count_1]) && count_1<20){
            count_1++;
        }
    }

    if(count_1==20){
      x = x1 + ((y-y1)*(x2-x1)/(y2-y1));
      count_1=21;
    }
  
}

void reset_start_DATA(){

    set_state=false;
    sound_get_state=false;
    start_state=false;
    false_start_state=false;
    reaction_state=false;
    set_signal=false;
    
    time1=micros();
    time2=micros();
    
    start_time=millis();
    unit_get_time=millis();
    last_weight_get_time=millis();
    last_time=0;
    
    hold_weight=0.00;
    last_weight=0.00;
    
    count_0=0;
    count_1=0;
    count_2=0;

}
