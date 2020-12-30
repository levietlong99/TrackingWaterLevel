# TrackingWaterLevel  
Using Arduino (ESP8266) for tracking water level in proof. Updating to firebase for app in real time. 

<img src="https://arduino.esp8266.vn/_images/Iot-wifi-uno-hw-pinout1.png" alt="Arduino Uno Wifi" width="250"/>

# About my project  

* We tracking water level in proof with 6 below warning level:  
  * HIGH_LEVEL1 (highest level in proof ❌): Stop pump through RELAY.  
  * HIGH_LEVEL2 : Send warning sms to user📲.  
  * HIGH_LEVEL3: turn on alarm and sirens 🔊 (as long as when pump is still running).  
  
  * LOW_LEVEL3: turn on alarm and sirens (as long as when faucet is still running).  
  * LOW_LEVEL2: Send warning sms to user📲.  
  * LOW_LEVEL1 (lowest level in proof ❌): Stop faucet through RELAY.  

* Using Arduino ESP8266 to connect to Firebase 🔊 (Google service) for some purposes below:  
  
  * Get status of pump and faucet (So Software enggineers can control our system).  
  * Update water level and traking information in real time (So they can tracking water level and draw graph on app mobile / PC).  
  * Update water state if embedded device has same thing change.  

# Things need to be prepared for this project  

* Arduino ESP8266 combined with UNO version (You still can use esp8266).  
* Module Sim 900A.  
* Relay 2 module with Opto (removal noise of electrical contacting).  
* HCSR04 sensor for traking water level, you can use some waterproof version.  
* Real time RTC tiny module.  
* One  DC supply 12V with some lm2596 lower volatge in compatible with this project.  
* 2 water-bottle (bottle type 18L is my choice).  
* Pump and electric controlling faucet 12V-DC  

### Notice: because i'm lack of software knowledge, so that you can see only embedded code C base on Arduino in this project.  

📌For more information, please contact to my email: vietlongle2000@gmail.com  
📌Be healthy and hard work, my friends.  
