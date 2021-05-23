/* This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details. */
#include <Adafruit_NeoPixel.h>
#include <Encoder.h>
#include <ESP8266WiFi.h>
#include "Tello.h"

/*
 * make sure the Tello is on latest firmeware, use the Tello App on your smartphone to do so!

  //Tello Lib     https://github.com/akshayvernekar/telloArduino
  //Tello SDK     https://www.ryzerobotics.com/de/tello/downloads
  //Tello Python  https://github.com/dji-sdk/Tello-Python
 *
*/

/*  Sample Tello Libs Commands:

    tello.takeoff();
    delay(5000);
    tello.up(30);
    delay(2000);
    tello.down(30);
    delay(2000);
    tello.right(30);
    delay(2000);
    tello.left(30);
    delay(2000);
    tello.land();

  */

// WiFi network name and password:
const char * networkName = "TELLO-AB5077";//replace with your Tello SSID, label under battery
const char * networkPswd = NULL; // "";//pleas set password using the Tello App on your smartphone!

//Are we currently connected?
boolean connected = false;

Tello tello;

Adafruit_NeoPixel WSpixels = Adafruit_NeoPixel((2<24)?2:24,0,NEO_GRB + NEO_KHZ800);
int rotary = 0 ;
// Encoder-Library http://www.pjrc.com/teensy/td_libs_Encoder.html
Encoder button_encoder(14,12); // Objekt Dreh-Encoder

void setup(){ // Einmalige Initialisierung

  pinMode( 2 , INPUT);
  pinMode( 13 , INPUT);
  WSpixels.begin();//-------------- Initialisierung Neopixel
  WSpixels.show();  

  Serial.begin(115200);
  Serial.println();
  WSpixels.setPixelColor(0,(0<32)?0:32,(0<32)?0:32,(0<48)?0:48);
  WSpixels.show();
  WSpixels.setPixelColor(1,(0<32)?0:32,(0<32)?0:32,(0<48)?0:48);
  WSpixels.show();

  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
  delay(500);

    if(connected)
    {
      Serial.println("ATTENTION - PREP FOR TAKEOFF - KEEP SPACE CLEAR");
      
      Serial.println("5sec ...");
      delay( 1000 );
      Serial.println("4sec ...");
      delay( 1000 );
      Serial.println("3sec ...");
      delay( 1000 );
      Serial.println("2sec ...");
      delay( 1000 );
      Serial.println("1sec ...");
      delay( 1000 );
      
      tello.takeoff();
      Serial.println("--TAKEOFF--");
    }
    
  delay( 2000 ); //wait till stabilized
  Serial.print("Altitude: ");
  Serial.println(tello.getAltitude());
  Serial.print("Battery: ");
  Serial.println(tello.getBattery());

}

void loop() { // Kontinuierliche Wiederholung 
  rotary = button_encoder.read() ;
  //Serial.print(String(rotary));
  //Serial.println();
  if (!( digitalRead(2) ))
  {
    WSpixels.setPixelColor(0,(32<32)?32:32,(0<32)?0:32,(0<48)?0:48);
    WSpixels.show();

    if(connected )
    {
      tello.left(20); // 20cm to the left
      Serial.println("<<left");
      //Serial.println(tello.getAltitude());
    }
    delay( 1000 ); //wait till stabilized
  }
  else
  {
    WSpixels.setPixelColor(0,(0<32)?0:32,(0<32)?0:32,(0<48)?0:48);
    WSpixels.show();
  }
  
  if (!( digitalRead(13) ))
  {
    WSpixels.setPixelColor(1,(0<32)?0:32,(32<32)?32:32,(0<48)?0:48);
    WSpixels.show();
    if(connected )
    {
      tello.right(20); // 20cm to the right
      Serial.println("right>>");
      //Serial.println(tello.getAltitude());
    }  
    delay( 1000 ); //wait till stabilized
  }
  else
  {
    WSpixels.setPixelColor(1,(0<32)?0:32,(0<32)?0:32,(0<48)?0:48);
    WSpixels.show();
  }

  if (!( digitalRead(13) ) && !( digitalRead(2) )){
    tello.land();
    Serial.println("--LAND--");
    Serial.println("");
    Serial.println("... press RESET to takeoff again ...");
    delay (5000);
   }

}


void connectToWiFi(const char * ssid, const char * pwd) 
{
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  delay(100);
  //Initiate connection
  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED) { // Warte bis Verbindung steht 
    delay(500); 
    Serial.print(".");
  };

  if (WiFi.status() == WL_CONNECTED){

     //When connected set
      Serial.print("WiFi connected! IP address: ");
      Serial.println(WiFi.localIP());
      //initialise Tello after we are connected
      tello.init();
      connected = true;
      
  } else {

      Serial.println("WiFi lost connection");
      connected = false;
    
  }
}
