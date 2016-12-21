//define Libraries
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WebConfig.h> 

#include "PietteTech_DHT.h" //DHT sensor lib

//-------------------------------
// defines for DHT lib
#define DHTTYPE  DHT11           // Sensor type DHT11/21/22/AM2301/AM2302
#define DHTPIN   4              // Digital pin for communications
#define REPORT_INTERVAL 5000    // in msec must > 2000

//-------------------------------
//define constants
const int eepromTempAddress = 0;
const int eepromHumidityAddress = 1;


//-------------------------------
//define globals

// for DHT
unsigned long startMills;
float t, h, d;
int acquireresult;
int acquirestatus;
bool bDHTstarted;       // flag to indicate we started acquisition

//------------------------------
//Functions declaration

//Libs functions
void dht_wrapper(); // must be declared before the lib initialization

//User functions
void getTemperature();





// Lib instantiate
PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);

WebConfig* pWebConfig; 


// This wrapper is in charge of calling must be defined like this for the lib work
void dht_wrapper() {
  DHT.isrCallback();
}



// Initialize the system
void setup()
{
  Serial.begin(115200); 
    
  pWebConfig = new WebConfig("BASIC WEBCONFIG v1.0", "ESP8266", "8266", false);
  Serial.println("");
  Serial.print ("IP address: ");
  Serial.println(WiFi.localIP());


  startMills = millis();
  
  // delay 2 sec before start acquire DHT
  while ( (millis() - startMills) < 2000 ) {
    yield();
  }

// blocking method for DHT
  acquirestatus = 0;
  acquireresult = DHT.acquireAndWait(1000);
  if ( acquireresult == 0 ) {
    t = DHT.getCelsius();
    h = DHT.getHumidity();
    d = DHT.getDewPoint();
  } else {
    t = h = d = 0;
  }
  

  
}

void loop()
{
  pWebConfig->ProcessHTTP();
  yield();
  
  getTemperature();

  //sendRequest((int)t);
  
  delay(5000);
 
}

//send request function
//
//void sendRequest(int data){
//
//  client.connect(host, httpPort);
//  
//  request = "GET " + url + data + isWriteToDB + "HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
//  Serial.println ("Sending request");
//  Serial.println (request);
//  
//  client.print (request);
//}

// Get temperature function
void getTemperature(){
  
  
   if (bDHTstarted) {
    acquirestatus = DHT.acquiring();
    if (!acquirestatus) {
      acquireresult = DHT.getStatus();
      if ( acquireresult == 0 ) {
		  t = DHT.getCelsius();
          h = DHT.getHumidity();
        //d = DHT.getDewPoint();
      }
      bDHTstarted = false;
    }
  }

  if ((millis() - startMills) > REPORT_INTERVAL) {

      Serial.print("Humidity (%): ");
      Serial.println(h);


      Serial.print("Temperature (oC): ");
      Serial.println(t);
	

      startMills = millis();

    // to remove lock
    if (acquirestatus == 1) {
      DHT.reset();
    }

    if (!bDHTstarted) {
      // non blocking method
      DHT.acquire();
      bDHTstarted = true;
    }
  }

}


