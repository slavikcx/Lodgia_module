//define Libraries


#include <ESP8266WiFi.h>
//-#include <aREST.h>
//-#include <aREST_UI.h>
#include <ESP8266WebServer.h> //HTTP WebServer
#include "PietteTech_DHT.h" //DHT sensor lib

//-------------------------------
// defines for DHT lib
#define DHTTYPE  DHT11           // Sensor type DHT11/21/22/AM2301/AM2302
#define DHTPIN   4              // Digital pin for communications
#define REPORT_INTERVAL 10000    // in msec must > 2000
// The port to listen for incoming TCP connections
#define LISTEN_PORT           8080



//-------------------------------
//define constants
// WiFi parameters
const char* ssid = "Delphi_Guests";
const char* password = "tofave12";

//const char* ssid = "SlavaWIFI";
//const char* password = "ZXCde3_vgy765_1234567890_vvv";

//Time server 
const char* timeServerHost = "ntp.time.in.ua";
//const char* timeServerHost = "utcnist.colorado.edu";
//const char* timeServerHost = "utcnist2.colorado.edu";
const int timeServerHttpPort = 13;

//const int relayPin = 13;


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

//String saT = ""; 
//String saH = "";
String dateTime = "";
String webPage = "";

//------------------------------
//Functions declaration

//Libs functions
void dht_wrapper(); // must be declared before the lib initialization

//User functions
void getTemperature();
String getTime();
//String toString (float data);
String buildWebPage();





//------------------------------
// Libs instantiate
PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);
// Create an instance of the server
WiFiServer server(LISTEN_PORT);

ESP8266WebServer webServer(80);



// -Create aREST instance
//-aREST_UI rest_ui = aREST_UI();


// This wrapper is in charge of calling must be defined like this for the lib work
void dht_wrapper() {
  DHT.isrCallback();
}


//------------------------------
// Initialize the system
void setup()
{
  Serial.begin(115200); 
  
  Serial.println("Starting....");
 

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
  
  //- Give name and ID to device
  //-rest_ui.set_id("1");
  //-rest_ui.set_name("esp8266");

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

  //- Set the title
  //-rest_ui.title("Test Page");
  //- Init variables and expose them torest_uiAPI
 
  //-rest_ui.variable("Date_and_Time", &dateTime);
  //-rest_ui.variable("Temperature", &aT);
  //-rest_ui.variable("Humidity", &aH);

   //- Labels & buttons
  //-rest_ui.label("Date_and_Time");
  //-rest_ui.label("Temperature");
  //-rest_ui.label("Humidity");


  //-rest_ui.button.name("Relay Control");
  //-rest_ui.button(13);

 //webPage += "<h1>ESP8266 Web Server</h1><p>Socket #1 <a href=\"socket1On\"><button>ON</button></a>&nbsp;<a href=\"socket1Off\"><button>OFF</button></a></p>";

  //webServer.on("/", [](){ webServer.send(200, "text/html", webPage);});

  webServerInit();
  //webServer.begin();
  //Serial.println("HTTP server started");
}

void loop()
{
    webServer.handleClient();

	delay(1);

	getTemperature();



	//Serial.println("before client");
	WiFiClient client = server.available();
  if (!client) {
    return;
  }

  //Serial.println("before client available");
  while (!client.available()) {
    delay(1);
  }

  //-rest_ui.handle(client);
 
}


void webServerInit(void)
{
	Serial.println("in webSereverInit");
	webServer.on("/", handle_root); // handling main webpage

	webServer.begin();
	Serial.println("HTTP server started");
}

void handle_root()
{
	Serial.println("in Handleroot");
	
	webPage = buildWebPage();

	webServer.send(200, "text/html", webPage);
	
}

//function for creating up to date web page
String buildWebPage()
{
	String page = "";
	page += "<h1>Test page</h1><p>Temperature: ";
	page += aT;
	page += "</p> <p>Humidity: ";
	page += aH;
	page += "</p> <p>Relay control</p>";
	page += "<p> <a href=\"relayOn\"> <button>On</button> </a> &nbsp;";
	page += "<a href=\"relayOff\"> <button>Off</button> </a> </p>";

	return page;
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

	  //getting time
	  getTime();
	 
	  //getting avarage T and H
	  aT = tempT/thAverageVar;
	  aH = tempH/thAverageVar;
	  
	  //converting float to string for Win explorer refresh
	 /* saT = toString(aT);
	  Serial.print("after tostring conversion - ");
	  Serial.println(saT);
	  saH = toString(aH);*/

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
// 
String getTime(){
	
	
	WiFiClient client;
	//String datetime = " ";
	Serial.print("connecting to ");
	Serial.println(timeServerHost);
	if (!client.connect(timeServerHost, timeServerHttpPort)) {
		Serial.println("Time server connection failed");
	}

	//Serial.println("SEND REQUEST");
	client.print("HEAD / HTTP/1.1\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; ESP8266 NodeMcu Lua;)\r\n\r\n");

	delay(100);

	while(client.available())
  {
    String line = client.readStringUntil('\r');

      //Parsing date time from server responce
      dateTime = line.substring(0, 11); // getting date
	  dateTime += line.substring(20, 25); //getting year
	  dateTime += line.substring(10, 20); // getting time
           
      Serial.println(dateTime);
  	
	  return dateTime;
	
	}
}








//toString function
/*
String toString (float data)
{
	char charVal[10];               //temporarily holds data from vals 
	String stringVal = "";     //data on buff is copied to this string
  
	
  dtostrf(data, 4, 2, charVal);  //4 is mininum width, 4 is precision; float value is copied onto buff
  
  //display character array
  Serial.print("charVal: ");
  for(int i=0;i<sizeof(charVal);i++)
  {
    Serial.print(charVal[i]);
  }
  Serial.println();
  //convert chararray to string
  for(int i=0;i<sizeof(charVal);i++)
  {
    stringVal+=charVal[i];
  }
  Serial.print("stringVal: ");Serial.println(stringVal); //display string

  return stringVal;
}
*/