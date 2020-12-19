#Scope
FlightContol is a hobby project to implement an arduino based software for controlling a quadcopter.
The basic idea is a simple software which stabilizes the copter and receives control command via WIFI. 
The remote control shall be done with an Android App which is located in a separate repository.  

#Hardware  
- Arduino Mega 256  
- ESP 8266  
- MPU 6050  

#Wiring
![Alt text](Arduino_mega_flightControl.png?raw=true "wiring")

#Remote control
The ESP8266 is acting as an SoftAP which provides a WIFI.
Basic idea is to connect to this SoftAP and send the control commands. 
The Arduino will read this commands via serial interface and deserialize it.

##How to setup the ESP8266 as SoftAP?
 tbd

##protocol of control data
|Byte|Name|Description|
|---|---|---|
|[0-3]|Start Sequence|`F0 F0 F0` start sequence, used for better deserialization|
|[4-6]|throttle|3 bytes throttle [1000-2000])|
|[7-9]|roll|3 bytes with roll angle [1000-2000]|
|[10-12]|pitch|3 bytes with pitch angle [1000-2000]|
|[13-15]|yaw|3 bytes with yaw angle [1000-2000]|

##How do I get from 3 bytes to an angle?
- sending an angle of 1500 means 0°
- sending 2000 means 90°
- sending 1000 means -90°
```c
float pitch = (pitchAsBytes - 1500) / 500.0 * 90.0; 
```