#include "ESP8266_FIRE_BASE.h"
#include "HCSR04.h"
#include "RTClib.h"

char print_str[20];

RTC_DS1307 rtc; // Khai báo biến cho rtc
uint16_t timer_buff[6];
void timer_count();

void setup(){
  
    pinMode(PUMP_BUTTON, INPUT_PULLUP);
    pinMode(FAUCET_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PUMP_BUTTON), pump_button, CHANGE);
    attachInterrupt(digitalPinToInterrupt(FAUCET_BUTTON), faucet_button, CHANGE);  
    lcd_init();
    read_EEPROM();                                         
    Serial.begin(9600);
    Wire.begin();
    HCSR04_init();
    setup_wifi_and_firebase();
    lcd.clear();
}

void loop(){

    get_data_from_firebase();
    water_level = check_distance();
    handle_warning();
    lcd_display();
    yield();
    timer_count();
    write_EEPROM();
    push_data_to_firebase(timer_buff[0], timer_buff[1],\
           timer_buff[2], timer_buff[3], timer_buff[4], timer_buff[5]);
    ESP.wdtFeed();
    delay(3000);
}

void timer_count() {
 
    DateTime now = rtc.now();
    timer_buff[0] = now.hour();
    timer_buff[1] = now.minute();
    timer_buff[2] = now.second();
    timer_buff[3] = now.day();
    timer_buff[4] = now.month();
    timer_buff[5] = now.year();
    lcd.setCursor(0, 3);
    if(timer_buff[0] <= 12){
        sprintf(print_str, "%02d:%02d:AM  %02d/%02d/%04d", timer_buff[0], timer_buff[1],\
                            timer_buff[3], timer_buff[4], timer_buff[5]);
    }
    else{
        sprintf(print_str, "%02d:%02d:PM  %02d/%02d/%04d", timer_buff[0] - 12, timer_buff[1],\
                            timer_buff[3], timer_buff[4], timer_buff[5]);
    }
    lcd.print(print_str);
}

ICACHE_RAM_ATTR void pump_button(){

    if(new_button != true){
      
        if(pump == 1) pump = 0;
        else pump = 1;
        new_button = true;
    }
}

ICACHE_RAM_ATTR void faucet_button(){

    if(new_button != true){
        
        if(faucet == 1) faucet = 0;
        else faucet = 1;
        new_button = true;
    }
}
