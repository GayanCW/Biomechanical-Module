
void set_METHOD_Path(boolean method_state){
  
    if(method_state==true){
      char pressed_button=buttonInput();
        if(pressed_button=='M' && pressed_signal==true){
            menu__();
            pressed_signal=false;     
        }
        if(pressed_button=='E' && pressed_signal==true){
            enter_();
            pressed_signal=false;
        }
        if(pressed_button=='D' && pressed_signal==true){
            down_();       
            pressed_signal=false;
        }
        if(pressed_button=='U' && pressed_signal==true){
            up_();
            pressed_signal=false;
        }
        if(pressed_button=='B' && pressed_signal==true){
            back_();
            pressed_signal=false;
        }
        if(pressed_button=='C' && pressed_signal==true){
            cancle_();
            pressed_signal=false;
        }
    }
}

void set_LCD_Path(){
  if(path<=1 && stay_page==0){
    keys(1);
      lcd.setCursor(0,1);
      lcd.print("                    "); 
      lcd.setCursor(0,2);
      lcd.print("                    ");  
  }
        if(path<=1 && stay_page==1){
          keys(2);
          maximum_raw=2; 
          select_Cursor(1);
              if((raw-1)/2==0){   
                  lcd.setCursor(1,1);
                  lcd.print("Start              ");
                  lcd.setCursor(1,2);
                  lcd.print("Settings           ");
              }
        }
              if(path==1000000 && stay_page==2){
                maximum_raw=1;
                method_NO=1;
                    lcd.setCursor(0,0);
                    lcd.print("                    ");
                    lcd.setCursor(0,1);
                    lcd.print(" Please Press 'OK'  ");
                    lcd.setCursor(0,2);
                    lcd.print("    at Set State    ");
              }
              if(path==2000000 && stay_page==2){
                keys(2);
                maximum_raw=4; 
                select_Cursor(1);
                    if((raw-1)/2==0){
                        lcd.setCursor(1,1);
                        lcd.print("weight scale       ");
                        lcd.setCursor(1,2);
                        lcd.print("sound sensor       ");
                    }
                    if((raw-1)/2==1){
                        lcd.setCursor(1,1);
                        lcd.print("Battery            ");
                        lcd.setCursor(1,2);
                        lcd.print("reset              ");
                    }
              }
                    if(path==2100000 && stay_page==3){
                      keys(2);
                      maximum_raw=3; 
                      select_Cursor(1);
                          if((raw-1)/2==0){
                              lcd.setCursor(1,1);
                              lcd.print("Check Weight       ");
                              lcd.setCursor(1,2);
                              lcd.print("Calibrate Scale    ");
                          }
                          if((raw-1)/2==1){
                              lcd.setCursor(1,1);
                              lcd.print("Set Weight Range   ");
                              lcd.setCursor(1,2);
                              lcd.print("                   ");
                          }
                    }
                          if(path==2120000 && stay_page==4){ //calibrate weight 
                              maximum_raw=1;
                              method_NO=2;
                          }
                          if(path==2130000 && stay_page==4){ //set weight range
                            keys(2);
                            maximum_raw=2; 
                            select_Cursor(1);
                                lcd.setCursor(1,1);
                                lcd.print("Minimum Range:-");
                                lcd.print(minimum_weight_range);
                                lcd.setCursor(1,2);
                                lcd.print("Maximum Range:+");
                                lcd.print(maximum_weight_range);
                          }
                                if(path==2131000 && stay_page==5){
                                    maximum_raw=1;
                                    method_NO=2; 
                                }
                                if(path==2132000 && stay_page==5){
                                    maximum_raw=1;
                                    method_NO=2; 
                                }
                    if(path==2200000 && stay_page==3){
                      keys(2);
                      maximum_raw=2; 
                      select_Cursor(1);
                          lcd.setCursor(1,1);
                          lcd.print("Select Sensor      ");
                          lcd.setCursor(1,2);
                          lcd.print("Check Sound Range  ");
                    }
                          if(path==2210000 && stay_page==4){
                              keys(2);
                              maximum_raw=2; 
                              select_Cursor(1);
                                  lcd.setCursor(1,1);
                                  lcd.print("Internal Sensor    ");
                                  lcd.setCursor(1,2);
                                  lcd.print("External Sensor    ");  
                          }
                                if((path==2211000 || path==2212000) && stay_page==5){ //voltage/presentage(on/off)
                                    keys(3);
                                    maximum_raw=2;
                                    select_Cursor(2);
                                        lcd.setCursor(5,2);
                                        lcd.print("ON");
                                        lcd.setCursor(12,2);
                                        lcd.print("OFF");
                                }
                                                        
                    if(path==2300000 && stay_page==3){
                      keys(2);
                      maximum_raw=2; 
                      select_Cursor(1);
                          lcd.setCursor(1,1);
                          lcd.print("Voltage/Presentage");
                          lcd.setCursor(1,2);
                          lcd.print("Low Battery Voltage");
                    }
                          if(path==2310000 && stay_page==4){ //voltage/presentage(on/off)
                            keys(2);
                            maximum_raw=2;
                            select_Cursor(1);
                                lcd.setCursor(1,1);
                                lcd.print("Voltage On/Off     ");
                                lcd.setCursor(1,2);
                                lcd.print("Presentage On/Off  ");
                          }
                                if((path==2311000 || path==2312000) && stay_page==5){ //voltage/presentage(on/off)
                                    keys(3);
                                    maximum_raw=2;
                                    select_Cursor(2);
                                        lcd.setCursor(5,2);
                                        lcd.print("ON");
                                        lcd.setCursor(12,2);
                                        lcd.print("OFF");
                                }
                                      
                          if(path==2320000 && stay_page==4){ //low battery voltage
                              maximum_raw=1;
                              method_NO=2;
                          }
                    if(path==2400000 && stay_page==3){
                      maximum_raw=2; 
                      method_NO=2;
                    } 
}

void reset_Path_Memory(){
  for(int index=0; index<=10; index++){
      path_array[index]=0;
  }
}

long return_Path(){
  long get_path=0;
  
  for(int index=1; index<=7; index++){
     get_path+=(path_array[index]*pow(10,7-index));     
  }
  get_path++;
  return get_path;
}

void menu__(){
  if(stay_page<=maximum_pages && raw<=maximum_raw){
      reset_Path_Memory();
      stay_page=1;
      raw=1;
      lcd.clear();
  }  
}
void enter_(){
  if(stay_page>0 && raw>0 && stay_page<maximum_pages && raw<=maximum_raw){
      stay_page++;
          path_array[stay_page-1]=raw;
      raw=1;
          path=return_Path();
          lcd.clear();
  }  
}
void down_(){
  if(stay_page>0 && raw>0 && stay_page<=maximum_pages && raw<maximum_raw){
      stay_page=stay_page;
      raw++;                
  }  
}
void up_(){
  if(stay_page>0 && raw>1 && stay_page<=maximum_pages && raw<maximum_raw+1){
      stay_page=stay_page;
      raw--;                 
  }
}
void back_(){
  if(stay_page>1 && raw>0 && stay_page<=maximum_pages && raw<=maximum_raw+1){
      stay_page--;
          path_array[stay_page]=0;
      raw=1;
          path=return_Path();
          lcd.clear();            
  }
}
void cancle_(){
  if(stay_page>0 && raw>0 && stay_page<=maximum_pages && raw<=maximum_raw+1){
      stay_page=0;
      raw=0;
          reset_Path_Memory(); 
      path=0;
      lcd.clear();                 
  }  
}

void select_Cursor(int path){
////////////////////////////////////////////////
  if(path==1 && raw<maximum_raw+1){  
      if(raw%2==1){
          lcd.setCursor(0,2);
          lcd.print(" "); 
          lcd.setCursor(0,1);
          lcd.print(">"); 
      }
      if(raw%2==0){
          lcd.setCursor(0,1);
          lcd.print(" ");
          lcd.setCursor(0,2);
          lcd.print(">");  
      }          
  }
  if(path==2 && raw<maximum_raw+1){  
      if(raw%2==1){
          lcd.setCursor(11,2);
          lcd.print(" OFF "); 
          lcd.setCursor(4,2);
          lcd.print("[ON]"); 
      }
      if(raw%2==0){
          lcd.setCursor(4,2);
          lcd.print(" ON ");
          lcd.setCursor(11,2);
          lcd.print("[OFF]");  
      }          
  }
////////////////////////////////////////////////
}

/*void reset_DATA(){
  for(int index=0; index<10; index++){
    EEPROM.write(index,0);
  }
}*/


void low_Battery_Notification(){
  analogWrite(backLight_pin,10);
  
  lcd.setCursor(0,0);
  lcd.print("                    ");
  lcd.setCursor(0,1); 
  lcd.print("Low Battery...!     ");
  lcd.setCursor(0,2); 
  lcd.print("Plug in Your Charger");
  lcd.setCursor(0,3);
  lcd.print("                    ");    
}

void ok(){
  lcd.setCursor(16,3); 
  lcd.print("[Ok]");
  delay(200);
  lcd.clear();
}

void reset_all_Data(){
  reset_start_DATA();
  
  for(int i=0; i<10; i++){
    EEPROM.write(i,0);
    if(i<8)path_array[i]=0;
  }

    EEPROM.write(1,169);   // this calibration factor is adjusted according to my load cell
    EEPROM.write(2,85);       // minimum_voltage in EEPROM index 2(8.5)
    EEPROM.write(5,25);
    EEPROM.write(6,25);
    calibration_factor = EEPROM.read(1)*100.00;   // this calibration factor is adjusted according to my load cell
    minimum_voltage = EEPROM.read(2)/10.00;       // minimum_voltage in EEPROM index 2(8.5)
    minimum_weight_range=EEPROM.read(5)/10.0;
    maximum_weight_range=EEPROM.read(6)/10.0;

    units=0;
    path=0;
    voltage=12;
    contrast=10;
    
    sound_signal=false;
    pressed_state=false;
    pressed_signal=false;
    
    button_value=0;
    presentage=0;
    raw=0, stay_page=0, stay_path=0;
    maximum_pages=8, maximum_raw=8;
    cal_1=0,cal_2=0;
    method_NO=0;
    time_delay=100;    // button hold delay
    backLight_time=30; // seconds
    backLight_startTime=millis();
    beep_delay=millis();
    pressStart_time=millis();
    pressed_time=millis();

    analogWrite(backLight_pin,20);

    //lcd.setCursor(0,1);
    //lcd.print("Successfull");
}
