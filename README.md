Send Voice Message using Particle and GSM Breakout Board
--------------------------------------------------------

I bought a GSM Breakout Board and was resting on my table for a long. Last week I decided to work on a project using GSM and Particle Core. Actually I decided to work on 2 project, one is to deliver a voice message to a specified number, another one is one IVR system. Here is the first project I worked, send a voice message to a specified telephone number. 

This project basically call a number, when the call is picked up on the other end, deliver a voice message and then hangup. For this one I am using Particle Core, SIM900 based GSM Breakout Board and [Emic2 Text to Speech Module](https://www.sparkfun.com/products/11711). Both GSM Breakout and Emic2 are Serial based. So you need two Serial to communicate to these two, first I tried Serial1 and Serial2. But only one was working either Serial1 or Serial2. I was not able to get both working at the same time. So I used [SPI to Serial Breakout  Board](https://www.sparkfun.com/products/9981). This breakout board convert SPI or I2C to Serial. Here I am using SPI to communication. The Emic2 is connected through Serial2 and GSM Breakout Board is connected through the SPI to Serial board.

For this project I am using Adafruit FONA library to communicate to the GSM Board. Even though this is written for Adafruit FONA, it will work with other SIM800/SIM900 boards. Adafruit FONA is based on SIM800L module. I have slightly modified the Adafruit FONA Library to include the functions to read current call status and set the auto answer.

For voice output I am using Emic2 Text to Speech module. The audio output of this module is connected to the Mic Input of the GSM Breakout Board using 3.5mm Audio connector.

The Particle Core application exposes three function to set the number to which voice message has to be delivered, set the message and to set the number of rings before automatically answer the call. When the send message function is invoked, the application calls the number and waits for the other end to pick up the phone. Once the phone us picked up, then the voice message is delivered and the application hang up the line. The status of the current call is obtained using the GSM900 AT Command **`AT+CLCC`**. This command returns the status of the current calls. This command is explained in [this document](http://www.simcom.us/act_admin/supportfile/SIM900_ATC_V1.00.pdf). The response string contains the status of the current call if any, otherwise the command returns just the string "OK". The *status* field of the response string contains the status of the call. Following are the possible values.

1. 0 Active
2. 1 Held
3. 2 Dialing
4. 3 Alerting
5. 4 Incoming
6. 5 Waiting

The application waits for the active call status and once it is active, the message is delivered using the Emic2 TTS Module and hangs up the call once the TTS is complete. For incoming call if the auto answer is set, the SIM module automatically answer the call after the specified number of rings. In this case also when the call is active, the application delivers the message and hangs up.

**Wiring**

*Particle Core and I2C/SPI-to-UART*

 - Core MOSI to I2C/SPI-to-UART MOSI 
 - Core MISO to I2C/SPI-to-UART MISO
 - Core SCK to I2C/SPI-to-UART SCK
 - Core SS to I2C/SPI-to-UART SS

*I2C/SPI-to-UART and GSM Shield*

 - I2C/SPI-to-UART Vcc to 3.3v   
 - I2C/SPI-to-UART GND to GND
 - I2C/SPI-to-UART Tx to GSM Board Rx   
 - I2C/SPI-to-UART Rx to GSM Board Tx 
 - GSM Board 5v to 5v 
 - GSM Board GND to GND

*Particle Core and Emic2 TTS Module*

 - Core Tx to Emic2 Rx 
 - Core Rx to Emic2 Tx 
 - Emic2 5v to 5v 
 - Emic2 GND to GND

