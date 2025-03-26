         /////////////////////////////////////////////  
        //   AI-driven Forest Fire Prevention      //
       //        Robot w/ SMS & EZ-Robot          //
      //             ---------------             //
     //          (Arduino MKR GSM 1400)         //           
    //             by Kutluhan Aktar           // 
   //                                         //
  /////////////////////////////////////////////

//
// Remove negligently discarded cigarette butts, quench burning debris, and get informed of the incident location via MKR GSM 1400 over SMS.
//
// For more information:
// https://www.theamplituhedron.com/projects/AI_driven_Forest_Fire_Prevention_Robot_w_SMS_EZ_Robot
//
//
// Connections
// Arduino MKR GSM 1400 :
//                                EZ-B V4 SMART ROBOT CONTROLLER
// D13 (RX) ---------------------- TX
// D14 (TX) ---------------------- RX
//                                Keyes 10mm RGB LED Module (140C05)
// D2  --------------------------- R
// D3  --------------------------- G
// D4  --------------------------- B


// Include the required libraries.
#include <MKRGSM.h>

// Define the APN (Access Point Name) information:
// https://apn.how/
#define PINNUMBER     ""
#define GPRS_APN      "internet"
#define GPRS_LOGIN    ""
#define GPRS_PASSWORD ""

// Initialize GSM, GPRS, SMS, and location instances:
GSMLocation location;
GPRS gprs;
GSM gsmAccess;
GSM_SMS sms;

// Define the RGB LED pins:
#define redPin   2
#define greenPin 3
#define bluePin  4

// Define the data holders:
String command = "", loc = "";

void setup() {
  // Initiate the hardware (Serial1) serial port to communicate with the EZ-B V4 controller.
  Serial1.begin(115200);
  Serial.begin(115200);

  // RGB:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  adjustColor(0,0,0);

  // Start the modem and attach the Arduino MKR GSM 1400 to the GPRS network with the APN, login, and password variables.
  bool connected = false;
  // Uncomment to debug errors with AT commands.
  //MODEM.debug(); 
  while(!connected){
    if((gsmAccess.begin(PINNUMBER) == GSM_READY) && (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)){
      connected = true;
    }else{
      Serial.println("GSM Modem: Not connected!\n");
      adjustColor(255,0,0);
      delay(1000);
    }
  }
  // After connecting to the GPRS network successfully:
  Serial.println("GSM Modem: Connected successfully to the GPRS network!\n");
  adjustColor(0,0,255);
  delay(5000);
}

void loop(){
  // Set the default color.
  adjustColor(255,0,255);
  
  // Obtain the robot's location via GPRS (General Packet Radio Service).
  getLocation();

  // Get the transferred command from the EZ-B V4 controller via the hardware serial port.
  if(Serial1.available() > 0){
    command = Serial1.readString();
  }
  // Activate the received command.
  if(command != ""){
    Serial.println(command);
    if(command == "location"){ SMS("+<_phone_number_>", loc); }
    else if(command == "off"){ adjustColor(0,0,0); }
    else if(command == "red"){ adjustColor(255,0,0); }
    else if(command == "green"){ adjustColor(0,255,0); }
    else if(command == "blue"){ adjustColor(0,0,255); }
    else if(command == "yellow"){ adjustColor(255,255,0); }
    else if(command == "magenta"){ adjustColor(255,0,255); }
    else if(command == "orange"){ adjustColor(255,165,0); }
    else if(command == "pink"){ adjustColor(255,192,203); }
    else if(command == "white"){ adjustColor(255,255,255); }
    // Clear the command string.
    command = "";
  }
  delay(2000);
}

void getLocation(){
  if(location.available()){   
    loc = "Location: "
          + String(location.latitude(), 7) + ", "
          + String(location.longitude(), 7) + "\n"
          + "Altitude: "
          + String(location.altitude()) + "m\n"
          + "Accuracy: +/- "
          + String(location.accuracy()) + "m";
    Serial.println(loc + "\n\n");
  }
}

void SMS(const char* phone, String msg){
  // Send an SMS to the given mobile phone number over the GSM network.
  sms.beginSMS(phone);
  sms.print(msg);
  sms.endSMS();
  Serial.println("SMS sent successfully!");
  adjustColor(0,255,0);
  delay(3000);
}

void adjustColor(int r, int g, int b){
  analogWrite(redPin, 255-r);
  analogWrite(greenPin, 255-g);
  analogWrite(bluePin, 255-b);
}
