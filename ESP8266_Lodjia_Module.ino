//define Libraries

#include <ESP8266WiFi.h>
#include <aREST.h>
#include <aREST_UI.h>
#include "PietteTech_DHT.h" //DHT sensor lib

//-------------------------------
// defines for DHT lib
#define DHTTYPE  DHT11           // Sensor type DHT11/21/22/AM2301/AM2302
#define DHTPIN   4              // Digital pin for communications
#define REPORT_INTERVAL 10000    // in msec must > 2000
// The port to listen for incoming TCP connections
#define LISTEN_PORT           80

//-------------------------------
//define constants
// WiFi parameters
const char* ssid = "Delphi_Guests";
const char* password = "tofave12";


//-------------------------------
//define globals

// for DHT
unsigned long startMills;
float t, h, d;
int acquireresult;
int acquirestatus;
bool bDHTstarted;       // flag to indicate we started acquisition
int thAverageVar = 0; //divider for get average T and H during period of time
float tempT, tempH, aT, aH = 0;


//------------------------------
//Functions declaration

//Libs functions
void dht_wrapper(); // must be declared before the lib initialization

//User functions
void getTemperature();





// Lib instantiate
PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);
// Create an instance of the server
WiFiServer server(LISTEN_PORT);
// Create aREST instance
aREST_UI rest = aREST_UI();

// This wrapper is in charge of calling must be defined like this for the lib work
void dht_wrapper() {
  DHT.isrCallback();
}



// Initialize the system
void setup()
{
  Serial.begin(115200); 
  
  //Serial.setDebugOutput(true); // Uncomment if debug serial messages needed
  


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
  
  // Set the title
  rest.title("Test Page");
  // Init variables and expose them to REST API

  rest.variable("Temperature", &aT);
  rest.variable("Humidity", &aH);

   // Labels
  rest.label("Temperature");
  rest.label("Humidity");

   // Give name and ID to device
  rest.set_id("1");
  rest.set_name("esp8266");


  WiFi.begin(ssid,password);
 // Serial.println (WiFi.localIP()); // not using becouse didn't get connected status while was connected to network
  /*while (WiFi.localIP() == "0.0.0.0"){
	  delay(500);
	  (WiFi.printDiag(Serial));

	  Serial.println(".");
	  Serial.println(WiFi.status());
	  Serial.println(" ");
  }*/
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  
  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop()
{
  
  
  getTemperature();

  
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }


  rest.handle(client);
 
 
}



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
   //collecting data for getting average T and H
   tempT = tempT + t;
   tempH = tempH + h;
   thAverageVar ++;


  if ((millis() - startMills) > REPORT_INTERVAL) {

	  //getting avarage T and H
	  aT = tempT/thAverageVar;
	  aH = tempH/thAverageVar;
	  Serial.print("Amounts of measurement: ");
	  Serial.println(thAverageVar);

      Serial.print("Humidity (%): ");
	  Serial.println(aH);


      Serial.print("Temperature (oC): ");
	  Serial.println(aT);
	
	  //setting all tem vars to 0
      startMills = millis();
	  tempT = 0;
	  tempH = 0;
	  thAverageVar = 0;

    // to remove lock
    if (acquirestatus == 1) {
      DHT.reset();
	  thAverageVar = 0;
    }

    if (!bDHTstarted) {
      // non blocking method
      DHT.acquire();
      bDHTstarted = true;
    }
  }

}


