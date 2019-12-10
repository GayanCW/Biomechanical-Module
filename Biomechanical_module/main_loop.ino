void loop(){
    
  if(method_NO==0){
      set_METHOD_Path(true);
      set_LCD_Path();
      backLight_Auto_MODE();
          if(path>=0 && path!=1000000){
            home_();                
          }
          if(path==2110000){
              keys(4);
              maximum_raw=0;
              weight_Scale(1,0);                  
          }
          if(path==2220000){
              keys(4);
              maximum_raw=0;
              sound_Sensor(1);                  
          }
/////////////////////////////////////////////////////////////////////////
          if(path==2211100 && stay_page==6){
              ok();
              EEPROM.write(7,157); //internal true
              back_();                           
          }
          if(path==2211200 && stay_page==6){
              ok();
              EEPROM.write(7,143); //internal false
              back_();
          }
          if(path==2212100 && stay_page==6){
              ok();
              EEPROM.write(7,153); //external ok
              back_();  
          }
          if(path==2212200 && stay_page==6){
              ok();
              EEPROM.write(7,139); //external false 
              back_(); 
          }
/////////////////////////////////////////////////////////////////////////          
          if(path==2311100 && stay_page==6){
              ok();
              battery('V',true);
              V_read=EEPROM.read(3);
              back_();                           
          }
          if(path==2311200 && stay_page==6){
              ok();
              battery('V',false); 
              V_read=EEPROM.read(3);
              back_();
          }
          if(path==2312100 && stay_page==6){
              ok();
              battery('P',true);
              P_read=EEPROM.read(4);
              back_();  
          }
          if(path==2312200 && stay_page==6){
              ok();
              battery('P',false);
              P_read=EEPROM.read(4); 
              back_(); 
          }          
  }           
  
  if(method_NO==1){
    maximum_raw=1;
    start_FUNCTION();
  }
  if(method_NO==10){
    true_START();
  }
  if(method_NO==11){
    false_START();
  }
  
  if(method_NO==2){
      backLight_Auto_MODE();
      set_METHOD_Path(false);
      set_LCD_Path();
          if(path>=0){
            home_();    
          }
          if(path==2120000){
              keys(2);
              weight_Scale(1,0);
              weight_Scale(2,0);
          }
          if(path==2320000){
              keys(2);
              battery('1',0);
          }
          if(path==2131000){
              keys(3);
              weight_Scale(3,1);
          }
          if(path==2132000){
              keys(3);
              weight_Scale(3,2);  
          }
          if(path==2400000){
            reset_all_Data();           
          }
  }

  if(method_NO==3){
      low_Battery_Notification();   
  }

}


