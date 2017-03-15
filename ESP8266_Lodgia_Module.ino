


//define Libraries

#include <Ultrasonic.h>
//#include <SoftwareSerial.h>
#include <Servo.h>
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
//#include <NewPing.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>



//-------------------------------
// The port to listen for incoming TCP connections
#define LISTEN_PORT           8080 // WiFI Server listen port
// defines for DHT lib
#define DHTTYPE DHT11
#define DHTPIN  0 //already puled up with on board resistor
//for newPing lib
//#define PING_PIN 13  //Pin on ESP which connected to trigger  on US sensor
//#define MAX_DISTANCE 100 //Max distance which measuring US sensor in sm. Max distance ~ 400-500sm.


//Define pins
int lightPin = 2; //light pin 13
//int powerPin = 16; //power pin 16 (no need to pull up with 470 and 10k resistors)
int pirPin = 14; //PIR sensor pin 14
int servoPin = 16; //Servo pin 3
//int rotatePin = 16; //pin for rotate counting
int UsTriggerPin = 12;
int UsEchoPin = 13;

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
unsigned long previousDHTMillis = 0;        // last measuring time
const long THinterval = 5000;              // interval at which to read sensor

//for DateTime
unsigned long previousTimeMillis = 0; //last connection to time server time
const long DTinterval = 1000; //time synch interval
String dateTime, syncTime = "01:00"; // Time when time synchronization occures - hh:mm:ss
bool isSyncSuccess = false;
unsigned long previousClockMillis = 0; // for counting clock delta

String moduleUpTime = "";

//Web
String webPage = "";
String xml = "";

//Light and brightness
bool isLightOn = false;
String lightState = "Off";
int maxTranzLevel = 1023; //Maximim level for opening Tranzistor (213 - 4.2V on Gate, 12.0V on Drain)
int brightness = 0; // current brightness
int brigntnessTreshold = 100; // setting minimal brightness when light will be ON
int offDelay = 6; //turn off delay in sec 
int lightOnCounter = 0;
unsigned long previousLightCounterMillis = 0;

bool isHardLightOn = false;
unsigned long previousHardLightMillis = 0;
int HardLightCheckCounter = 0; 

//Power
bool isPowerOn = true;
String powerState = "ON";
String powerOutageTime = "";

//PIR
bool pirState = false;

//Distance
int currentdist_cm, dist_cm = 0;
//int averageDist_cm = MAX_DISTANCE;
//int distMeasureCount = 1;
int distTreshold = 50; //Minimal distance on which Light will be turned ON
int hardLedTreshold =5; // minimal distance for hard light on/off
unsigned long previousDistanceMillis = 0;
int distanceMeasuringPeriod = 50; //in ms

//External data
unsigned long previousExtDataMillis = 0; //last measuring of external temperature and atm pressure
const long extDataInterval = 5000; //get external sensor data interval
float externalTemp; 
int	atmPressure = 0;
int altitude = 0;

//Rolet
bool isRoletOpen = false;
String roletState = "Closed";

//int cycle=0;

//------------------------------
// Libs instantiate

// Create an instance of the server
WiFiServer server(LISTEN_PORT);
//Creating webServer instance with listening of port 80
ESP8266WebServer webServer(80);
//Creating NewPing instance
//NewPing usSensor(PING_PIN, PING_PIN, MAX_DISTANCE);
Ultrasonic ultrasonic(UsEchoPin, UsTriggerPin);

//Create BMP280 instance
Adafruit_BMP280 bmp; // SDA/SCL default to pins 4 & 5
//Create Servo instance
Servo roletka1;

//------------------------------
// Initialize the system
void setup()
{
	Serial.begin(115200);
	//Serial.setDebugOutput(true); // Uncomment if debug serial messages needed
	Serial.println("");
	Serial.println("Setup Started....");
	Serial.println("-------------------------");
	delay(3000);

	//Initializing WiFI
	WiFiInit();
	// init and start Web server
	webServerInit();
	//Getting time
	timeInit();
	//Initializing DHT
	dhtInit();
	//Initializing BMP280 sensor
	bmpInit();

	PirInit();
	
	//Setting pins mode and state
	pinMode(lightPin, OUTPUT); // setting light pin to output
	digitalWrite(lightPin, LOW); //Light is OFF by default

	//pinMode(powerPin, OUTPUT); //setting Power pin to output
	//digitalWrite(powerPin, HIGH); //Power is ON by default

	//digitalWrite(pirPin, LOW); //pirPin is LOW by default
	
	roletka1.attach(servoPin); //attaching serv pin

	//pinMode(rotatePin, INPUT); //setting pin for rotation count

	Serial.println("Setup finished.");
	Serial.println("-------------------------");
	Serial.println("");
}

void loop()
{
	//Serial.println("START OF LOOP");
	//cycle++;
	//Serial.print ("cycle - ");
	//Serial.println(cycle);
	
	webServer.handleClient();
	delay(1);

	//getting module up time
	ModulUpTime();
	// calling internal clock function for updating dateTime
	Clock();
	//getting internall temperature and humidity
	getTemperature();
	//getting motion
	detectMotion();
	//getting external tenperature  and pressure
	getExternalData();
	//getting distance
	getDistance();
	//getting inside brightness
	GetBrightness();

	//Serial.println(digitalRead(rotatePin));
	//(50);

	//handling Light hard On/OFF
	//LightHardOnOff();
	//handling light auto ON/OFF
	LightAutoOnOff();

	WiFiClient client = server.available();

	//Serial.println("END OF LOOP");
}

