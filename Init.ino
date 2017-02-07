//File includes all initialisation function


void dhtInit()
{

	dht.begin();           // initialize temperature sensor

	Serial.println("DHT sensor initialization succeed.");
	Serial.println("");
}

void bmpInit() 
{
	if (!bmp.begin(0x76)) {
		Serial.println("Could not find a valid BMP280 sensor, check wiring!");
		
	}
}

void WiFiInit()
{
	

	Serial.println("Scaning network...");

	// WiFi.scanNetworks will return the number of networks found
	int n = WiFi.scanNetworks();
	Serial.println("scan done");
	if (n == 0)
	{
		Serial.println("no networks found");
		Serial.println("");
		Serial.println("Starting device as Access Point...");

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

	}	else	{
		Serial.print(n);
		Serial.println(" networks found");

		for (int i = 0; i < n; ++i)
		{
			Serial.print("Checking network ");
			Serial.println(i);

			if (WiFi.SSID(i) == "SlavaWIFI") 
			{
				Serial.print(WiFi.SSID(i));
				Serial.println(" network found");
				Serial.println("connecting...");

				WiFi.mode(WIFI_STA);
				byte tries = 20;
				WiFi.begin(homeSSID, homePassword);

				// checking connection 10 times or untill connection succeed
				while (--tries && WiFi.localIP() == defaultIp)
				{
					Serial.print(".");
					Serial.println(WiFi.localIP());
					delay(1000);
				}
				if (WiFi.localIP() != defaultIp)
				{
					Serial.println("");
					Serial.println("WiFi connected");
					Serial.println("IP address: ");
					Serial.println(WiFi.localIP());
					break;
				}
				else 
				{
					Serial.print("Unable to connect to ");
					Serial.println(WiFi.SSID(i));
				}
			}

			if (WiFi.SSID(i) == "Delphi_Guests")
			{
				Serial.print(WiFi.SSID(i));
				Serial.println(" network found");
				Serial.println("connecting...");

				WiFi.mode(WIFI_STA);
				byte tries = 20;
				WiFi.begin(workSSID, workPassword);

				// checking connection 10 times or untill connection succeed
				while (--tries && WiFi.localIP() == defaultIp)
				{
					Serial.print(".");
					Serial.println(WiFi.localIP());
					delay(1000);
				}
				if (WiFi.localIP() != defaultIp)
				{
					Serial.println("");
					Serial.println("WiFi connected");
					Serial.println("IP address: ");
					Serial.println(WiFi.localIP());
					break;
				}
				else
				{
					Serial.print("Unable to connect to ");
					Serial.println(WiFi.SSID(i));
				}
			}

		}
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
		Serial.println(WiFi.localIP());

	}
	
	/*
	// tryin got connect to WiFi net
	WiFi.mode(WIFI_STA);
	byte tries = 20;
	WiFi.begin(ssid, password);
	
	

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
		//IPAddress apIP(192, 168, 4, 1);
		//WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
		//WiFi.softAP("ESP8266_AP", "12345");
		
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
		
		Serial.println("");
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
	}
	*/

	// Start WiFI server
	server.begin();
	Serial.println("WiFi Server started - port 8080");

	Serial.println("WiFI initialization succeed.");
	Serial.println("");
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
	Serial.println("Web server initialization succeed.");
	Serial.println("");
}

