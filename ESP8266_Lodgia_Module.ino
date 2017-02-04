


//define Libraries
#include <Adafruit_BMP280.h>
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <DHT.h>
#include <NewPing.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>



//-------------------------------
// The port to listen for incoming TCP connections
#define LISTEN_PORT           8080 // WiFI Server listen port
// defines for DHT lib
#define DHTTYPE DHT11
#define DHTPIN  0 //already puuled up with on board resistor
//for newPing lib
#define PING_PIN 13  //Pin on ESP which connected to trigger ? echo on US sensor
#define MAX_DISTANCE 300 //Max distance which measuring US sensor in sm. Max distance ~ 400-500sm.


//Define pins
int lightPin = 2; //light pin 13
int powerPin = 16; //power pin 16 (no need to pull up with 470 and 10k resistors)
int pirPin = 14; //PIR sensor pin


//-------------------------------
//define constants
//WiFi 
IPAddress defaultIp(0, 0, 0, 0);
IPAddress AccessPointIP(192, 168, 4, 1);
const char* workSSID = "Delphi_Guests";
const char* workPassword = "tofave12";

const char* homeSSID = "SlavaWIFI";
const char* homePassword = "ZXCde3_vgy765_1234567890_vvv";

//Time server 
const char* timeServerHost = "ntp.time.in.ua";
const int timeServerHttpPort = 13;


//-------------------------------
//define globals

// for DHT and getTemperature
//DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266
DHT_Unified dht(DHTPIN, DHTTYPE);
float insideHumidity, insideTemperature;  // Values read from sensor
unsigned long previousMillis = 0;        // last measuring time
const long THinterval = 5000;              // interval at which to read sensor

										   //for DateTime
unsigned long previousTimeMillis = 0; //last connection to time server time
const long DTinterval = 1000; //time synch interval
String dateTime = "";

String UpTime = "";
String webPage = "";
String xml = "";

bool isLightOn = false;
String lightState = "Off";

bool isPowerOn = true;
String powerState = "ON";

int maxTranzLevel = 1023; //Maximim level for opening Tranzistor (213 - 4.2V on Gate, 12.0V on Drain) 

bool pirState = false;

int currentdist_cm, dist_cm = 0;
int distTreshold = 110; //Minimal distance on which Light will be turned ON
bool hardledOn = false;
bool hardledOff = true;

unsigned long previousExtDataMillis = 0; //last measuring of external temperature and atm pressure
const long extDataInterval = 5000; //get external sensor data interval
float externalTemp; 
int	atmPressure = 0;
int altitude = 0;

//int cycle=0;


//------------------------------
//Functions declaration

// Init functions
void dhtInit();
void webServerInit(void);
void WiFiInit();

//User functions
void getTemperature();
String getTime();
String moduleUpTime();
//String toString (float data);

//for Web Interface
String buildWebPage();
void handle_xml();
String buildXML();
void handle_root();
//functions for light control
void handle_lightOn();
void handle_lightOff();
//functions for Power control
void handle_powerOn();
void handle_powerOff();

//for motion detection
void detectMotion();

//for getting distance
void getDistance();


//------------------------------
// Libs instantiate

// Create an instance of the server
WiFiServer server(LISTEN_PORT);
//Creating webServer instance with listening of port 80
ESP8266WebServer webServer(80);
//Creating NewPing instance
NewPing usSensor(PING_PIN, PING_PIN, MAX_DISTANCE);
//Create BMP280 instance
Adafruit_BMP280 bmp; // SDA/SCL default to pins 4 & 5


//------------------------------
// Initialize the system
void setup()
{
	//Serial.begin(115200);
	//Serial.setDebugOutput(true); // Uncomment if debug serial messages needed
	Serial.println("");
	Serial.println("Setup Started....");
	Serial.println("-------------------------");
	delay(3000);

	//Initializing WiFI
	WiFiInit();
	// init and start Web server
	webServerInit();
	//Initializing DHT
	dhtInit();
	//Initializing BMP280 sensor
	bmpInit();
	
	//Setting pins mode and state
	pinMode(lightPin, OUTPUT); // setting light pin to output
	digitalWrite(lightPin, LOW); //Light is OFF by default

	pinMode(powerPin, OUTPUT); //setting Power pin to output
	digitalWrite(powerPin, HIGH); //Power is ON by default

	digitalWrite(pirPin, LOW); //pirPin is LOW by default
	pinMode(pirPin, INPUT); //setting PirPin to input

	Serial.println("Setup finished.");
	Serial.println("-------------------------");
	Serial.println("");
}

void loop()
{
	//Serial.println("START OF LOOP");
	/*cycle++;
	Serial.print ("cycle - ");
	Serial.println(cycle);
	Serial.println("In Loop before delay 50ms ");
	delay(50);
	Serial.println("In Loop after delay 50ms ");*/
	webServer.handleClient();
	delay(1);

	UpTime = moduleUpTime();

	getTime();

	//Serial.println("In Loop before getTemp");
	getTemperature();

	detectMotion();

	getExternalData();
	
	getDistance();

	//lightHardOnOff();

	//Serial.println("before client init");

	WiFiClient client = server.available();

	//delay(1000);
	//Serial.println("after client init");

	//
	//	/*while (!client)
	//	{
	//		Serial.println("Client not init");
	//		delay(500);
	//	}
	//	 Serial.println("Client init succeed");
	//*/
	// /* if (!client) {
	//    Serial.println("Client not init");
	//	  return;
	//  } else {
	//	   Serial.println("Client init succeed");
	//  }*/
	//
	// /* Serial.println("before client available");
	//  while (!client.available()) {
	//    Serial.println("Client not available");
	//	  delay(100);
	//  }*/
	//  
	//	//Serial.print("Module UP time - ");
	//	//Serial.print(moduleUpTime());
	//	//Serial.println("");
	//
	//  //Serial.println("END OF LOOP");
}

