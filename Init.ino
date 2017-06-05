//File includes all initialisation function

void DHTInit()
{
	String methodName = "DHTInit";

	//Serial.println(dateTime + " | " + methodName + " | " + "DHT sensor initialization started...");

	dht.begin();           // initialize temperature sensor

	//Serial.println(dateTime + " | " + methodName + " | " + "DHT sensor initialization finished.");
	
}

void BMPInit() 
{
	String methodName = "BMPInit";

	//Serial.println(dateTime + " | " + methodName + " | " + "BMP sensor initialization started...");

	if (!bmp.begin(0x76)) {
		//Serial.println(dateTime + " | " + methodName + " | " + "Could not find a valid BMP280 sensor, check wiring!");
		
	} else {
		//Serial.println(dateTime + " | " + methodName + " | " + "BMP sensor initialization finished.");	
	}
}

void PirInit() 
{
	String methodName = "PirInit";

	//Serial.println(dateTime + " | " + methodName + " | " + "PIR sensor initialization started...");

	pinMode(pirPin, INPUT); //setting PirPin to input
	digitalWrite(pirPin, LOW); //pirPin is LOW by default

	for (int i = 0; i < 3; i++)
	{
		//Serial.print(".");
		delay(1000);
	}
	//Serial.println("");
	//Serial.println(dateTime + " | " + methodName + " | " + "PIR sensor initialization finished...");
	
}

void WiFiInit()
{
	String methodName = "WiFiInit";

	//Serial.println(dateTime + " | " + methodName + " | " + "Scaning network...");
	//logMessage += (dateTime + " | " + "1" + " | " + "1~\n");

	// WiFi.scanNetworks will return the number of networks found
	int n = WiFi.scanNetworks();
	//Serial.println(dateTime + " | " + methodName + " | " + "WiFI networks scan done.");
	//logMessage += (dateTime + " | " + "1" + " | " + "2~\n");

	if (n != 0)
	{
		//////Serial.print(n);
		//Serial.println(dateTime + " | " + methodName + " | " + n +" networks found.");
		//logMessage += (dateTime + " | " + "1" + " | " + n + "6~\n");


		for (int i = 0; i < n; ++i)
		{
			//Serial.println(dateTime + " | " + methodName + " | " + "Checking network " + i);
			//logMessage += (dateTime + " | " + "1" + " | " + "7" + i + "~\n");

			////Serial.println(i);
			
			if (WiFi.SSID(i) == homeSSID) 
			{
				//Serial.println(dateTime + " | " + methodName + " | " + WiFi.SSID(i) + " network found.");
				//logMessage += (dateTime + " | " + "1" + " | " + WiFi.SSID(i) + "6~\n");

				//Serial.println(dateTime + " | " + methodName + " | " + "connecting...");
				//logMessage += (dateTime + " | " + "1" + " | " + "8~\n");

				WiFi.mode(WIFI_STA);
				byte tries = 20;
				WiFi.begin(homeSSID, homePassword);

				// checking connection 10 times or untill connection succeed
				while (--tries && WiFi.localIP() == defaultIp)
				{
					//////Serial.print(".");
					//Serial.println(dateTime + " | " + "1" + " | " + WiFi.localIP());
					//logMessage += (dateTime + " | " + "1" + " | " + WiFi.localIP() + "~");

					delay(1000);
				}
				if (WiFi.localIP() != defaultIp)
				{
					
					
					Serial.print(dateTime + " | " + methodName + " | " + "WiFi connected. " + "IP address: ");
					Serial.println(WiFi.localIP());

					//logMessage += (dateTime + " | " + "1" + " | " + "9" + "10");
					//logMessage += WiFi.localIP();
					//logMessage += "~";
					//////Serial.print("IP address: ");

					
					break;
				}
				else 
				{
					//Serial.println(dateTime + " | " + methodName + " | " + "Unable to connect to " + WiFi.SSID(i));
					//logMessage += (dateTime + " | " + "1" + " | " + "11" + WiFi.SSID(i) +" ~");

					////Serial.println(WiFi.SSID(i));
					
				}
			}

			if (WiFi.SSID(i) == workSSID)
			{
				//Serial.println(dateTime + " | " + methodName + " | " + WiFi.SSID(i) + " network found.");
				//logMessage += (dateTime + " | " + "1" + " | " + WiFi.SSID(i) + "6~");

				/*////Serial.print(WiFi.SSID(i));
				//Serial.println(" network found");*/
				//Serial.println(dateTime + " | " + methodName + " | " + "connecting...");
				//logMessage += (dateTime + " | " + "1" + " | " + "8~");

				////Serial.println("connecting...");

				WiFi.mode(WIFI_STA);
				byte tries = 20;
				WiFi.begin(workSSID, workPassword);

				// checking connection 10 times or untill connection succeed
				while (--tries && WiFi.localIP() == defaultIp)
				{
					//////Serial.print(".");
					//Serial.println(dateTime + " | " + methodName + " | " + WiFi.localIP());
					//logMessage += (dateTime + " | " + "1" + " | " + WiFi.localIP() + "~");

					////Serial.println(WiFi.localIP());
					delay(1000);
				}
				if (WiFi.localIP() != defaultIp)
				{
					//Serial.println(dateTime + " | " + methodName + " | " + "WiFi connected. " + "IP address: " + WiFi.localIP());
					//logMessage += (dateTime + " | " + "1" + " | " + "9" + "10" + WiFi.localIP() + "~");

/*
					//Serial.println("");
					//Serial.println("WiFi connected");
					//Serial.println("IP address: ");
					//Serial.println(WiFi.localIP());*/
					break;
				}
				else
				{
					//Serial.println(dateTime + " | " + methodName + " | " + "Unable to connect to " + WiFi.SSID(i));
					//logMessage += (dateTime + " | " + "1" + " | " + "11" + WiFi.SSID(i) + " ~");

					/*////Serial.print("Unable to connect to ");
					//Serial.println(WiFi.SSID(i));*/
				}
			}

			
		}
		isAccessPoint = false;
	}
	 
	if (WiFi.localIP() == defaultIp)		
	{
		//Serial.println(dateTime + " | " + methodName + " | " + "No known networks found.");
		//logMessage += (dateTime + " | " + "1" + " | " + "3~");

		//Serial.println(dateTime + " | " + methodName + " | " + "Starting device as Access Point...");
		//logMessage += (dateTime + " | " + "1" + " | " + "4~");
		
		WiFi.softAP(APSSID, APPassword);


		//Serial.print(dateTime + " | " + methodName + " | " + "WiFi running as Access Point: " + APSSID + "; Password: " + APPassword + "; IP address: ");
		//Serial.println(WiFi.softAPIP());
		//logMessage += (dateTime + " | " + "1" + " | " + "5" + ssidAP + "; Password: " + passwordAP + "; IP address: " + WiFi.softAPIP() + "~");
		isAccessPoint = true;
	}
	
	
	// Start WiFI server
	server.begin();
	//Serial.println(dateTime + " | " + methodName + " | " + "WiFi Server started - port 8080");
	//logMessage += (dateTime + " | " + "1" + " | " + "13~");

	//Serial.println(dateTime + " | " + methodName + " | " + "WiFI initialization finished.");
	//logMessage += (dateTime + " | " + "1" + " | " + "14~");
	
}

void WebServerInit(void)
{
	String methodName = "WebServerInit";

	//logMessage += (dateTime + " | " + "2" + " | " + "1~");
	//logMessage += (dateTime + " | " + "2" + " | " + "2~");
	//Serial.println(dateTime + " | " + methodName + " | " + "Web server initialization started.");
	//Serial.println(dateTime + " | " + methodName + " | " + "Mapping Web server handling method...");

	webServer.on("/", WebServerHandleRoot); // handling main webpage
	webServer.on("/xml", WebServerHandleXML); //handling xml

	webServer.on("/lightOn", WebServerHandleLightOn); // light On handling
	webServer.on("/lightOff", WebServerHandleLightOff); // light Off handling

	webServer.on("/powerOn", WebServerHandlePowerOn); // light On handling
	webServer.on("/powerOff", WebServerHandlePowerOff); // light Off handling

	webServer.on("/blindsOpen", WebServerHandleBlindsOpen); // blinds Open handling
	webServer.on("/blindsClose", WebServerHandleBlindsClose); // blinds Close handling

	//Serial.println(dateTime + " | " + methodName + " | " + "Starting Web server...");
	//logMessage += (dateTime + " | " + "2" + " | " + "3~");

	webServer.begin();

	//Serial.println(dateTime + " | " + methodName + " | " + "HTTP server started - port " + webSereverPort);
	//logMessage += (dateTime + " | " + "2" + " | " + "4" + webSereverPort + "~");

	////Serial.println("");
	//Serial.println(dateTime + " | " + methodName + " | " + "Web server initialization finished.");
	//logMessage += (dateTime + " | " + "2" + " | " + "5~");

	////Serial.println("");
}

void TimeInit()
{
	String methodName = "TimeInit";
	//Serial.println(dateTime + " | " + methodName + " | " + "Getting current time from net...");
	//logMessage += (dateTime + " | " + "2" + " | " + "1~");

	TimeSync();

	previousClockMillis = millis();

	//Serial.println(dateTime + " | " + methodName + " | " + "Time initialization finished.");
	//logMessage += (dateTime + " | " + "2" + " | " + "2~");
}