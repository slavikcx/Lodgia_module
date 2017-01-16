//define Libraries


#include <DHT_U.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h> //HTTP WebServer

//-------------------------------
// The port to listen for incoming TCP connections
#define LISTEN_PORT           8080 // WiFI Server listen port
// defines for DHT lib
#define DHTTYPE DHT11
#define DHTPIN  4
//-------------------------------
//define constants
// WiFi parameters
const char* ssid = "Delphi_Guests";
const char* password = "tofave12";

//const char* ssid = "SlavaWIFI";
//const char* password = "ZXCde3_vgy765_1234567890_vvv";

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

//int cycle=0;

//Define pins
int lightPin = 5; //light pin
int powerPin = 13; //power pin 13
int pirPin = 0; //PIR sensor pin


//------------------------------
//Functions declaration

// Init functions
void dhtInit ();
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


//------------------------------
// Libs instantiate

// Create an instance of the server
WiFiServer server(LISTEN_PORT);
//Creating webServer instance with listening of port 80
ESP8266WebServer webServer(80);


//------------------------------
// Initialize the system
void setup()
{
  Serial.begin(115200); 
  //Serial.setDebugOutput(true); // Uncomment if debug serial messages needed
  Serial.println("");
  Serial.println("Setup Started....");
  Serial.println ("-------------------------");
  delay(3000);
  
  //Initializing WiFI
  WiFiInit();
  // init and start Web server
  webServerInit();
  //Initializing DHT
  dhtInit();

  //Setting pins mode and state
  pinMode(lightPin, OUTPUT); // setting light pin to output
  digitalWrite(lightPin, LOW); //Light is OFF by default

  pinMode(powerPin, OUTPUT); //setting Power pin to output
  digitalWrite(powerPin, HIGH); //Power is ON by default

  digitalWrite(pirPin, LOW); //pirPin is LOW by default
  pinMode(pirPin, INPUT); //setting PirPin to input
  
  Serial.println ("Setup finished.");
  Serial.println ("-------------------------");
  Serial.println ("");
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

	
	//Serial.println("before client init");

	WiFiClient client = server.available();
	
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

void webServerInit(void)
{
	
	webServer.on("/", handle_root); // handling main webpage
	webServer.on("/xml", handle_xml);//handling xml


	webServer.on("/lightOn", handle_lightOn); // light On handling
	webServer.on("/lightOff", handle_lightOff);// light Off handling

	webServer.on("/powerOn", handle_powerOn); // light On handling
	webServer.on("/powerOff", handle_powerOff);// light Off handling

	webServer.begin();
	Serial.println("HTTP server started - port 80");
	Serial.println("");
    Serial.println ("Web server initialization succeed.");
    Serial.println ("");
}

String buildWebPage()
{
	String page = "";

	page += "<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.01 Transitional//EN' 'http://www.w3.org/TR/html4/loose.dtd'>";
	//button and colors styles
	page += "<style>";
	page += ".button {height: 150px; width: 480px; text-align: center; text-decoration: none; font-size: 100px; opacity: 0.8; filter: alpha(opacity=80); border-radius: 30px;} ";
	page += ".blue {background-color: dodgerblue;}";
	page += ".green {background-color: mediumspringgreen}";
	page += ".red {background-color: tomato}";
	page += "</style>";
	
	//Java script
	page += "<SCRIPT>";
	page += "var xmlHttp=createXmlHttpObject();";
	
	page += "function createXmlHttpObject(){ if(window.XMLHttpRequest){ xmlHttp=new XMLHttpRequest();}else{ xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');} return xmlHttp;}";
	
	page += "function process(){";
	page += "if(xmlHttp.readyState==0 || xmlHttp.readyState==4){";
	page += "xmlHttp.open('PUT','xml',true); xmlHttp.onreadystatechange=handleServerResponse; xmlHttp.send(null);}";
	page += "setTimeout('process()',1000);}";

	page += "function handleServerResponse(){";
	page += "if(xmlHttp.readyState==4 && xmlHttp.status==200){";
	page += "xmlResponse=xmlHttp.responseXML;";

	page += "xmldoc = xmlResponse.getElementsByTagName('dateTime'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('dateTime').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('temperature'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('temperature').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('humidity'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('humidity').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('lightState'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('lightState').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('powerState'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('powerState').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('pirState'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('pirState').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('UpTime'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('UpTime').innerHTML = message;";


	page += "}}";
	
	page += "function sendButton(Parameter){";
	page += "server = '/' + Parameter; ";
	page += "request = new XMLHttpRequest();";
	page += "request.open('GET', server, true);";
	page += "request.send();}";


	page += "</SCRIPT>";


	page += "<html> <head> <title>Test page</title> </head>";
	page += "<body onload='process()'>"; //runnig function when page loaded

	page += "<h1>Test page                   <span style='font-size:9px'>--UP Time <span id='UpTime'></span>--</span></h1>";	//page header

	page += "<h2 id='dateTime'> </h2>"; //show date and time
	page += "<h2>Temperature: <span id='temperature'> </span></h2>"; //show temperature
	page += "<h2>Humidity: <span id='humidity'> </span></h2>"; //show humidity
	page += "<h2>Light <span id='lightState'> </span></h2>"; //show light state

	//page += "";
	
	page += "<h1> <button class='button green' onclick='sendButton(&quot;lightOn&quot;);'>Light On</button>";
	page += "<button class='button blue' onclick='sendButton(&quot;lightOff&quot;);'>Light Off</button> </h1>";

	page += "<h2>Power <span id='powerState'> </span></h2>"; //show power state

	page += "<h1> <button class='button green' onclick='sendButton(&quot;powerOn&quot;);'>Power On</button>";
	page += "<button class='button red' onclick='sendButton(&quot;powerOff&quot;);'>Power Off</button> </h1>";

	
	page += "<h2>Motion <span id='pirState'> </span></h2>"; //show pir state
		
	page += "</body> </html>";

	return page;
}

void handle_root()
{
	webPage = buildWebPage();
	webServer.send(200, "text/html", webPage);
	/*Serial.println("");
	Serial.print("Web Page sent - ");
	Serial.println(webPage);
	Serial.println("");*/
	
}

void handle_xml()
{
	xml = buildXML();
	webServer.send(200, "text/xml", xml);
	/*Serial.println("");
	Serial.println("xml Sent");
	Serial.println(xml);
	Serial.println("");*/

}

String buildXML()
{
	String xml = "";

	xml += "<?xml version='1.0'?>";
    xml += "<Donnees>"; 

    xml += "<dateTime>";
	xml += dateTime;
    xml += "</dateTime>";

	xml += "<UpTime>";
	xml += UpTime;
    xml += "</UpTime>";
    
	xml += "<temperature>";
	xml += insideTemperature;
	xml += "</temperature>";

	xml += "<humidity>";
	xml += insideHumidity;
	xml += "</humidity>";

	xml += "<lightState>";
	xml += lightState;
	xml += "</lightState>";

	xml += "<powerState>";
	xml += powerState;
	xml += "</powerState>";

	xml += "<pirState>";
	xml += pirState;
	xml += "</pirState>";

    xml += "</Donnees>";

	return xml;
}

void handle_lightOn()
{
	if (!isLightOn)
	{
		Serial.println("Switching On light");
		for (int i = 0; i <= maxTranzLevel; i++)
		{
			analogWrite(lightPin, i);
			delay(1);
		}
		
		isLightOn = true;
		lightState = "ON";
		webServer.send(200, "text/html", "OK");
	}
}
void handle_lightOff()
{
	if (isLightOn)
	{
		Serial.println("Switching Off light");
		for (int i = maxTranzLevel; i >= 0; i--)
		{
			analogWrite(lightPin, i);
			delay(2);
		}
		isLightOn = false;
		lightState = "Off";
		webServer.send(200, "text/html", "OK");
	}
}

void handle_powerOn() {
	if (!isPowerOn)
	{
		Serial.println("Switching On Power");
		digitalWrite(powerPin, HIGH);
		isPowerOn = true;
		powerState = "ON";
		webServer.send(200, "text/html", "OK");
	}
}
void handle_powerOff() {
	if (isPowerOn)
	{
		Serial.println("Switching Off Power");
		digitalWrite(powerPin, LOW);
		isPowerOn = false;
		powerState = "Off";
		webServer.send(200, "text/html", "OK");
	}
}

void detectMotion() {

	if (digitalRead(pirPin) != pirState)
	{
		pirState = digitalRead(pirPin); // checkin for motion detection
		Serial.print("Motion - ");
		Serial.println(pirState);
	}
	
}

void getTemperature(){
  
	//Serial.println("In GetTemperatre");
	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis >= THinterval) {
		
				// save the last time you read the sensor 
		previousMillis = currentMillis;

		// Get temperature event and print its value.
		sensors_event_t event;  

		 dht.temperature().getEvent(&event);
		if (isnan(event.temperature)) {
			Serial.println("Error reading temperature!");
		}
		 else {
			insideTemperature = event.temperature;
			Serial.print("Temperature: ");
			Serial.print(event.temperature);
			Serial.println(" *C");
		}
		// Get humidity event and print its value.
		dht.humidity().getEvent(&event);

		if (isnan(event.relative_humidity)) {
			Serial.println("Error reading humidity!");
		}
		else {
			insideHumidity = event.relative_humidity;
			Serial.print("Humidity: ");
			Serial.print(event.relative_humidity);
			Serial.println(" %");
		}
	}

  //Serial.println("exition from get Temperature");
}

String getTime() {

	unsigned long currentTimeMillis = millis();

	if (currentTimeMillis - previousTimeMillis >= 1000)
	{
		previousTimeMillis = currentTimeMillis;

		WiFiClient client;
		//String datetime = " ";
		Serial.print("");
		Serial.print("connecting to ");
		Serial.println(timeServerHost);

		if (!client.connect(timeServerHost, timeServerHttpPort)) {
			Serial.println("Time server connection failed");

			dateTime = "Time server unavailable";
			return dateTime;
		}
		else {

			Serial.println("SEND REQUEST");
			client.print("HEAD / HTTP/1.1\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; ESP8266 NodeMcu Lua;)\r\n\r\n");

			delay(100);


			while (client.available())
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

		dateTime = "Time server unavailable";
		return dateTime;
	}
}

void dhtInit ()
{

	dht.begin();           // initialize temperature sensor

  Serial.println ("DHT sensor initialization succeed.");
  Serial.println ("");
}

void WiFiInit()
{
	// tryin got connect to WiFi net
  WiFi.mode(WIFI_STA);
  byte tries = 20;
  WiFi.begin(ssid,password);
  
  IPAddress defaultIp(0, 0, 0, 0);
  // checking connection 10 times or untill connection succeed
  while (--tries && WiFi.localIP() == defaultIp)
  {
    Serial.print(".");
	Serial.println(WiFi.localIP());
    delay(1000);
  }

  if (WiFi.localIP() == defaultIp)
  {
    // if unable to connect ot WiFi starting Access Point
    Serial.println("");
    Serial.println("Starting device as Access Point");
    
	WiFi.disconnect();

	const char *ssidAP = "ESP8266AP";
	const char *passwordAP = "12345";

  // changen WiFI mode
  WiFi.mode(WIFI_AP);
 /* IPAddress apIP(192, 168, 4, 1);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("ESP8266_AP", "12345");
*/
	WiFi.softAP(ssidAP, passwordAP);
	IPAddress myIP = WiFi.softAPIP();

	Serial.print("WiFi running as Access Point: ");
	Serial.println(ssidAP);
	Serial.print("Password: ");
	Serial.println(passwordAP);
	Serial.print("IP address: ");
    Serial.println(myIP);

  }
  else {
    // Иначе удалось подключиться отправляем сообщение
    // о подключении и выводим адрес IP
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  // Start WiFI server
  server.begin();
  Serial.println("WiFi Server started - port 8080");

  Serial.println ("WiFI initialization succeed.");
  Serial.println ("");
}

String moduleUpTime(){
  String Time="";
  unsigned long ss;
  byte mm,hh;
  ss=millis()/1000;
  hh=ss/3600;
  mm=(ss-hh*3600)/60;
  ss=(ss-hh*3600)-mm*60;
  if(hh<10)Time+="0";
  Time+=(String)hh+":";
  if(mm<10)Time+="0";
  Time+=(String)mm+":";
  if(ss<10)Time+="0";
  Time+=(String)ss;
  return Time;
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