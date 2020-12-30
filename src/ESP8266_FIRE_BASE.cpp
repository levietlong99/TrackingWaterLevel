#include "ESP8266_FIRE_BASE.h"

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_SIZE / 100, LCD_SIZE % 100);

String path = "/controller/";
uint16_t count = 0;
//volatile uint8_t pump = 0;
//volatile uint8_t faucet = 0;
volatile int pump = 0;
volatile int faucet = 0;
volatile bool new_button = false;
//uint8_t temp1 = 0;
//uint8_t temp2 = 0;
float water_level = 0;
uint8_t is_sent_message = 0;
char temp_buff[50];
char time_buff[50];

void setup_wifi_and_firebase(){

    memset(temp_buff, 0, strlen(temp_buff));
    memset(time_buff, 0, strlen(time_buff));
    WiFi.begin(USER_NAME, PASS_WORD);
    while(WiFi.status() != WL_CONNECTED){

        delay(500);
    }
    Firebase.begin(FIRE_BASE_HOST, FIRE_BASE_AUTH);
}

void read_EEPROM(){

    EEPROM.begin(512);
    delay(10);
    count = (EEPROM.read(M_TRACKING_1) << 8) | EEPROM.read(M_TRACKING_0);
    pump = EEPROM.read(M_PUMP);
    faucet = EEPROM.read(M_FAUCET);
    water_level = EEPROM.read(M_WATER_LEVEL);
    is_sent_message = EEPROM.read(M_MESSAGE_STATUS);
}

void lcd_init(){

    lcd.begin();
    pinMode(SIREN_PIN, OUTPUT);
    pinMode(PUMP_PIN, OUTPUT);
    pinMode(FAUCET_PIN, OUTPUT);
}

void lcd_display(){

    if(!new_button){
      
        lcd.setCursor(0, 0);
        if(faucet == 1){                    // 1 - Bật relay máy bơm, tín hiệu HIGH đi vào RELAY

            digitalWrite(FAUCET_PIN, HIGH);
            lcd.print("FAUCET:  ON ");
        }
        else{                               // 2 - Tắt relay máy bơm, tín hiệu LOW đi vào RELAY

            digitalWrite(FAUCET_PIN, LOW);
            lcd.print("FAUCET:  OFF");
    
        }
        lcd.setCursor(0, 1);
        if(pump == 1){                      // 1 - Bật relay máy bơm, tín hiệu HIGH đi vào RELAY

            digitalWrite(PUMP_PIN, HIGH);
            lcd.print("PUMP:    ON ");
        }
        else{                               // 2 - Tắt relay máy bơm, tín hiệu LOW đi vào RELAY

            digitalWrite(PUMP_PIN, LOW);
            lcd.print("PUMP:   OFF");
        }
    }
    lcd.setCursor(0, 2);
    if(water_level > BELOW_LIMIT_1){

        lcd.print("OUT OF WATER      ");
    }
    else if(water_level < ABOVE_LIMIT_1){

        lcd.print("WATER OVERFLOW     ");
    }
    else{

        lcd.print("WATER LEVEL  ");
        lcd.print((BELOW_LIMIT_1 - water_level) / (BELOW_LIMIT_1 - ABOVE_LIMIT_1) * 100);
        lcd.print("% ");
    }
}

void get_data_from_firebase(){

//  FirebaseObject object = Firebase.get(path);
//  temp1 = object.getInt("faucet");
//  temp2 = object.getInt("pump");
//  if(new_button == true){
//
//    Firebase.setInt("controller/faucet", faucet);
//    Firebase.setInt("controller/pump", pump);
//    new_button = false;
//  }
//  else{
//    pump = temp2;
//    faucet = temp1;
//  }

  FirebaseObject object = Firebase.get(path);
  if(new_button == true){

    if(faucet == 1){

      Firebase.setInt("controller/faucet", 1);
    }
    else if(faucet == 0){
      
      Firebase.setInt("controller/faucet", 0);
    }
    if(pump == 1){
      
      Firebase.setInt("controller/pump", 1);
    }
    else if(pump == 0){
      
      Firebase.setInt("controller/pump", 0);
    }
    new_button = false;
  }
  else{  
  
    pump = object.getInt("pump");
    faucet = object.getInt("faucet");
  }
}

void write_EEPROM(){                      // Ghi dữ liệu vừa nhận vào EEPROM

    EEPROM.write(M_TRACKING_1, (count >> 8) & 0xFF);
    EEPROM.write(M_TRACKING_0, count & 0xFF);
    EEPROM.write(M_PUMP, pump);
    EEPROM.write(M_FAUCET, faucet);
    EEPROM.write(M_MESSAGE_STATUS, is_sent_message);
    EEPROM.write(M_WATER_LEVEL, water_level);
    EEPROM.commit();
}

void push_data_to_firebase(uint8_t Hours, uint8_t Minutes, uint8_t Seconds,\
                         uint8_t Days, uint8_t Months, uint16_t Years){

    count++;
    if(count == 65524){

        count = 0;
    }
    sprintf(time_buff, "%02d/%02d/%4d;%02d:%02d:%02d", Days, Months, Years, Hours, Minutes, Seconds); // Up dữ liệu hiện tại
    Firebase.setInt("current/level", water_level);
    Firebase.setString("current/last_updated", time_buff);
    if(count % 15 == 0){

        sprintf(temp_buff, "tracking/row_%d/level", count / 15);                                        // Up dữ liệu tracking
        Firebase.setFloat(temp_buff, water_level);
        sprintf(temp_buff, "tracking/row_%d/time", count / 15);
        Firebase.setString(temp_buff, time_buff);
    }
}

void send_message(){

    Serial.println("AT+CMGF=1");  // Set GSM in text mode
    delay(1000);                   // One second delay
    sprintf(temp_buff, "AT+CMGS=\"%s\"", PHONE_NUMBER);
    Serial.print(temp_buff);
    Serial.println();
    delay(1000);

    if(water_level <= ABOVE_LIMIT_2){

        Serial.print("Warning, water proof ready to be overflow "); // SMS body - Sms Text
    }
    else if(water_level >= BELOW_LIMIT_2){

        Serial.print("Warning, water proof ready to be out of water ");
    }
    delay(300);
    Serial.println();
    Serial.write(26);                //CTRL+Z Command to send text and end session
}

void handle_warning(){
    

    if((water_level >= BELOW_LIMIT_3 &&  faucet == 1) || (water_level <= ABOVE_LIMIT_3 && pump == 1)){

        digitalWrite(SIREN_PIN, HIGH);
    }
    else{
      
        digitalWrite(SIREN_PIN, LOW);
    }
    
    if(!is_sent_message){

        if((water_level >= BELOW_LIMIT_2 &&  faucet == 1) || \
            (water_level <= ABOVE_LIMIT_2 && pump == 1)){ // Xử lý dữ liệu khi ở mức cảnh báo
          
            send_message();
            is_sent_message = 1;
        }
    }
    else if(water_level < BELOW_LIMIT_2 && water_level > ABOVE_LIMIT_2){

        is_sent_message = 0;
    }
    
    if(water_level >= BELOW_LIMIT_1 && faucet == 1){ 
      
        faucet = 0;
        digitalWrite(FAUCET_PIN, LOW);
        Firebase.setInt("controller/faucet", 0);
    }
    else if(water_level <= ABOVE_LIMIT_1 && pump == 1){

        pump = 0;
        digitalWrite(PUMP_PIN, LOW);
        Firebase.setInt("controller/pump", 0);
    }
}
