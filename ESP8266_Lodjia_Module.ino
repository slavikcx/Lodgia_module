
//define Libraries
#include <ESP8266WiFi.h>
#include <aREST.h>
#include <aREST_UI.h>
#include "PietteTech_DHT.h" //DHT sensor lib


#include <WebConfig.h> 



//-------------------------------
// defines for DHT lib
#define DHTTYPE  DHT11           // Sensor type DHT11/21/22/AM2301/AM2302
#define DHTPIN   9              // Digital pin for communications
#define REPORT_INTERVAL 5000    // in msec must > 2000

//-------------------------------
//define constants


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

//WebConfig* pWebConfig; 

//aREST_UI rest = aREST_UI();


// WiFi parameters
//const char* ssid = "Delphi_Guests";
//const char* password = "tofave12";

// The port to listen for incoming TCP connections
//#define LISTEN_PORT           80

// Create an instance of the server
//WiFiServer server(LISTEN_PORT);


// This wrapper is in charge of calling must be defined like this for the lib work
void dht_wrapper() {
  DHT.isrCallback();
}



// Initialize the system
void setup()
{
  Serial.begin(115200); 
    
  //pWebConfig = new WebConfig("BASIC WEBCONFIG v1.0", "ESP8266", "8266", false);
  /*Serial.println("");
  Serial.print ("IP address: ");
  Serial.println(WiFi.localIP());
*/

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
  

  //rest.title("Test page");

  // // Labels
  //rest.label("temperature");
  //rest.label("humidity");

  ////Give name and ID to device
  //rest.set_id("1");
  //rest.set_name("esp8266");

  //// Connect to WiFi
  //WiFi.begin(ssid, password);
  //while (WiFi.status() != WL_CONNECTED) {
	 // delay(500);
	 // Serial.print(".");
  //}
  //Serial.println("");
  //Serial.println("WiFi connected");

  // // Start the server
  //server.begin();
  //Serial.println("Server started");

  //// Print the IP address
  //Serial.println(WiFi.localIP());

  
}

void loop()
{
  //pWebConfig->ProcessHTTP();
  //yield();
  
  // Handle REST calls
	/*WiFiClient client = server.available();
	if (!client) {
		return;
	}
	while (!client.available()) {
		delay(1);
	}*/

  getTemperature();

  //sendRequest((int)t);
 /* 
  rest.variable("temperature", &t);
  rest.variable("humidity", &h);

  rest.handle(client);
*/
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


