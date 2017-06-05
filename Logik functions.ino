//Module logic functions

void LightOn()
{
	String methodName = "LightOn";

	if (!isLightOn)
	{
		//Serial.println(dateTime + " | " + methodName + " | " + "Switching On light...");
		for (int i = 0; i <= maxTranzLevel; i++)
		{
			analogWrite(lightPin, i);
			delay(1);
		}
		
		isLightOn = true;
		lightState = "ON";

		//Serial.println(dateTime + " | " + methodName + " | " + "Light is " + lightState);
	} else 
	{
		//Serial.println(dateTime + " | " + methodName + " | " + "Light is already " + lightState);
	}
}

void LightOff()
{
	String methodName = "LightOff";
	if (isLightOn)
	{
		//Serial.println(dateTime + " | " + methodName + " | " + "Switching Off light...");
		for (int i = maxTranzLevel; i >= 0; i--)
		{
			analogWrite(lightPin, i);
			delay(1);
		}
		isLightOn = false;
		lightState = "Off";
		
		//Serial.println(dateTime + " | " + methodName + " | " + "Light is " + lightState);

	} else 
	{
		//Serial.println(dateTime + " | " + methodName + " | " + "Light is already " + lightState);
	}
}

void LightHardOnOff()
{
	if (dist_cm <= hardLedTreshold)
	{
		if (HardLightCheckCounter>=3)
		{
			//Checking if hard Light ON is need
			if (isHardLightOn == false && HardLightCheckCounter >= 3) {  //Turn On Light

				////Serial.println(" ");
				////Serial.println("Hard Turn ON");
				LightOn();
				isHardLightOn = !isHardLightOn; // changing hard switch state
				dist_cm = 60; // adding 60sm to current destance to prevent imidiate hard light Off
				HardLightCheckCounter = 0;
			}

			//Checking if hard Light Off is need
			if (isHardLightOn == true && HardLightCheckCounter >= 3) {  //Turn Off  Light

				////Serial.println(" ");
				////Serial.println("Hard Turn OFF");
				LightOff();
				isHardLightOn = !isHardLightOn; // changing hard switch state
				dist_cm = 60; // adding 60sm to current distance to prevent automatic led On just after turning hard Off 
				HardLightCheckCounter = 0;
			}

		} else if (millis() - previousHardLightMillis >= 1000) 
		{
			previousHardLightMillis = millis();
			HardLightCheckCounter++;

			////Serial.print("HardLightCheckCounter - ");
			////Serial.println(HardLightCheckCounter);
		}
		
	}
	 //HardLightCheckCounter = 0;
}

void TimeSync() {

	String methodName = "TimeSync";

	if (isAccessPoint == false) //checking if not AP
	{
			WiFiClient client;
			
			////Serial.print(dateTime + " | " + methodName + " | " + "Connecting to " + timeServerHost);
			
			if (!client.connect(timeServerHost, timeServerHttpPort)) {
				//Serial.println(dateTime + " | " + methodName + " | " + "Time server connection failed");

				dateTime = "Time server N_A ";
				
			}
			else {

				////Serial.println(dateTime + " | " + methodName + " | " + "Sending request for current time");
				client.print("HEAD / HTTP/1.1\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; ESP8266 12E;)\r\n\r\n"); //12E <-> NodeMcu Lua
				
				delay(10);

				String line = client.readStringUntil('\r');

				////Serial.println(dateTime + " | " + methodName + " | " + "Got responce from time server: " + line);
				//////Serial.println(line);

				//Parsing date time from server responce
				dateTime = line.substring(0, 11); // getting date
				dateTime += line.substring(20, 25); //getting year
				dateTime += line.substring(10, 20); // getting time
				//Serial.println(dateTime + " | " + methodName + " | " + "Parsed data: " + dateTime);

			}

	} else {

		String previousTime = dateTime.substring(16, 21); // getting previous time if exist
		dateTime = "WiFi net N_A    " + previousTime;
		//Serial.println (dateTime + " | " + methodName + " | " + dateTime);
	}

}

void Clock()
{
	//////Serial.println("in Clock");
	byte hh, mm, ss = 0;
	unsigned long currentMillis = millis();
	String newTime ="";

	
	if(currentMillis - previousClockMillis > 1000) //checking if delta between mills more that 1 second or it will be lost 
	{
		/*
		////Serial.print("old time - ");
		////Serial.println(dateTime);
		////Serial.println("");
		////Serial.print("substring 16,24 - ");
		////Serial.println(dateTime.substring(16, 24));
*/
		if (dateTime.substring(19,24) == syncTime) // 19,24 - 01:00 - sync every 1 hour; 16,24 - 01:00:00 - sync every 1 day
		{
			////Serial.println("Syncing time...");
		
			TimeSync();

			////Serial.print("time after sync - ");
			////Serial.println(dateTime);
			////Serial.println("");

		}else{

			//////Serial.println("Using internal clock...");

			//////Serial.print("currentMillis - ");
			//////Serial.println(currentMillis);
			//////Serial.print("previousClockMillis - ");
			//////Serial.println(previousClockMillis);

			unsigned long clockDelta = (currentMillis - previousClockMillis)/1000;
			
			previousClockMillis = currentMillis;

			////Serial.print("clock delta - ");
			////Serial.println(clockDelta);

			////Serial.print("previousClockMillis - ");
			////Serial.println(previousClockMillis);

			ss = atoi(dateTime.substring(22, 24).c_str());
			////Serial.print("ss - ");
			////Serial.println(ss);

			ss = ss	+ clockDelta; //converting string to int and summing with clock delta
			////Serial.print("ss + delta - ");
			////Serial.println(ss);


			if (ss >= 60)
			{
				mm = atoi(dateTime.substring(19, 21).c_str()) + ss/60;
				////Serial.print("mm - ");
				////Serial.println(mm);
				ss = ss%60;
			} else {
				mm = mm = atoi(dateTime.substring(19, 21).c_str());
			}

			if (mm >= 60)
			{
				hh = atoi(dateTime.substring(16, 18).c_str()) + mm/60;
				////Serial.print("hh - ");
				////Serial.println(hh);
				mm = mm%60;
			} else{
				hh = atoi(dateTime.substring(16, 18).c_str());
			}

			////Serial.print("calculated time:");
			////Serial.print("ss - ");
			////Serial.println(ss);
			/*//Serial.print("mm - ");
			//Serial.println(mm);
			//Serial.print("hh - ");
			//Serial.println(hh);
			//Serial.print("");

			//Serial.print("dateTime before cutting - ");
			//Serial.println(dateTime);
			*/// modifying dateTime
			dateTime = dateTime.substring(0, 16);

			////Serial.print("dateTime after cutting (0, 16) - ");
			////Serial.println(dateTime);

			if (hh<10)dateTime += "0";
			dateTime += (String)hh + ":";
			if (mm<10)dateTime += "0";
			dateTime += (String)mm + ":";
			if (ss<10)dateTime += "0";
			dateTime += (String)ss;

			////Serial.print("new time - ");
			////Serial.println(dateTime);
			////Serial.println("");
		}
	}

}

void ModulUpTime() {
	String methodName = "ModulUpTime";
	
	moduleUpTime = ConvertMillistoTime(millis());

	////Serial.println(dateTime + " | " + methodName + " | " + "CModule UP time: " + moduleUpTime);
}

void GetBrightness()
{
	String methodName = "GetBrightness";
	
	brightness = analogRead(A0);
	//Serial.println(dateTime + " | " + methodName + " | " + "Brightnes: " + brightness);
}

String ConvertMillistoTime(unsigned long millisValue)
{
	String Time = "";
	unsigned long ss;
	byte mm, hh, dd;
	ss = millisValue / 1000; //converting millis to seconds
	dd = ss / 86400; // getting days 
	hh = (ss - dd * 86400) / 3600; // getting hours 
	mm = (ss - dd * 86400 - hh * 3600) / 60; //getting minutes 
	ss = ss - dd * 86400 - hh * 3600 - mm * 60; // getting seconds 

	Time += (String)dd += " days ";
	if (hh<10)Time += "0";
	Time += (String)hh + ":";
	if (mm<10)Time += "0";
	Time += (String)mm + ":";
	if (ss<10)Time += "0";
	Time += (String)ss;
	return Time;
}

void LightAutoOnOff()
{
	String methodName = "LightAutoOnOff";
	// Checking current brightness. if it's high then nno need to check if we need to torn on light
	if (brightness <= brigntnessTreshold)
	{
		// checking sensors for turning On light
		if (dist_cm < distTreshold || pirState == true && isLightOn == false && isHardLightOn == false)
		{
			int distanceRetry = 0;
			int matchCount = 0;
			//Serial.println(dateTime + " | " + methodName + " | " + "Distance: " + dist_cm + "; pirState: " + pirState + "; isLightOn: " + isLightOn + "; isHardLightOn: " + isHardLightOn);
			
			if (dist_cm < distTreshold)
			{
				do
				{
					getDistance();

					//Serial.println(dateTime + " | " + methodName + " | " + "Verifying distance again. Distance: " + dist_cm);

					if (dist_cm < distTreshold)
					{

						//Serial.println(dateTime + " | " + methodName + " | " + "Distance match found. matchCount: " + matchCount);
						matchCount++;
					}
					delay(52);
					distanceRetry++;
				} while (distanceRetry < 3);

			}

			////Serial.println("Getting distance and motion again");
			//delay(10); //waiting 10 ms
			//getDistance(); // getting distnce again for excluding false turning on
			//detectMotion(); //checking for motion again for excluding false turning on

			if (dist_cm < distTreshold && matchCount>=3 || pirState == true)
			{
				//Serial.println(dateTime + " | " + methodName + " | " + "Turning ON light. dist_cm: " + dist_cm + "; matchCount: " + matchCount + "; pirState: " + pirState);
				LightOn(); //swithcing ON light
				previousLightCounterMillis = millis();
			}
			else 
			{
				//Serial.println(dateTime + " | " + methodName + " | " + "False light turning ON prevented! dist_cm: " + dist_cm + "; matchCount: " + matchCount + "; pirState: " + pirState);
				
			}
		}
	}

	// checking if we need to Turn Off light
	if (isLightOn == true && isHardLightOn == false && lightOnCounter >= offDelay)
	{
		//Serial.println(dateTime + " | " + methodName + " | " + "Turning OFF light. isLightOn: " + isLightOn + "; isHardLightOn: " + isHardLightOn + "; lightOnCounter: " + lightOnCounter);

		LightOff();
		lightOnCounter = 0;
	}

	//Counter
	if (isLightOn == true && isHardLightOn == false) // checking if light is on but not hard ON
	{
		//starting counter
		if (dist_cm > distTreshold && pirState == false) //if NO movement on any sensor - counter continues
		{
			lightOnCounter = (millis() - previousLightCounterMillis) / 1000;

			//Serial.println(dateTime + " | " + methodName + " | " + "Light counter: " + lightOnCounter + "; dist_cm: " + dist_cm + "; pirState: " + pirState);

		}
		else
		{
			//if there was movement on any sensor - resetting counter 
			//Serial.println(dateTime + " | " + methodName + " | " + "Resetting light counter. dist_cm: " + dist_cm + "; pirState: " + pirState);

			previousLightCounterMillis = millis();
		}
	}

}

void OpenBlinds()
{
	String methodName = "OpenBlinds";

	if(!isBlindsOpened)
	{
		int currentTurnAmount = 0;

		Serial.println(dateTime + " | " + methodName + " | " + "Opening blinds...");
		roletka1.attach(servoPin);
		Serial.println(dateTime + " | " + methodName + " | " + "Start rotation.");

		do {
			roletka1.write(180);

			if (digitalRead(rotateSensorPin) == HIGH)
			{
				isFullTurnDone = false;
			}
			else if (digitalRead(rotateSensorPin) == LOW && isFullTurnDone == false)
			{
				currentTurnAmount++;
				isFullTurnDone = true;
				Serial.println(dateTime + " | " + methodName + " | " + "Rotating... - isFullTurnDone = " + isFullTurnDone + "; currentTurnAmount = " + currentTurnAmount);
			}

			delay(10); //posible should be ajusted on real blinds

		} while (currentTurnAmount < openCloseTurnAmount);

		Serial.println(dateTime + " | " + methodName + " | " + "Rotating finished. Detaching...");

		delay(200); // to avoid stop rotation on rotate sensor
		roletka1.detach(); // stopping servo
		
		isBlindsOpened = true;
		blindsState = "Opened";

		Serial.println(dateTime + " | " + methodName + " | " + "Blinds is " + blindsState);
	} else 
	{
		Serial.println(dateTime + " | " + methodName + " | " + "Blinds is already " + blindsState +". isBlindsOpened - " + isBlindsOpened);
	}
}

void CloseBlinds()
{
	String methodName = "CloseBlinds";

	if (isBlindsOpened)
	{
		int currentTurnAmount = 0;

		Serial.println(dateTime + " | " + methodName + " | " + "Closing blinds...");
		roletka1.attach(servoPin);
		Serial.println(dateTime + " | " + methodName + " | " + "Start rotation.");

		do {
			roletka1.write(0);

			if (digitalRead(rotateSensorPin) == HIGH)
			{
				isFullTurnDone = false;
			}
			else if (digitalRead(rotateSensorPin) == LOW && isFullTurnDone == false)
			{
				currentTurnAmount++;
				isFullTurnDone = true;
				Serial.println(dateTime + " | " + methodName + " | " + "Rotating... - isFullTurnDone = " + isFullTurnDone + "; currentTurnAmount = " + currentTurnAmount);
			}

			delay(10); //posible should be ajusted on real blinds

		} while (currentTurnAmount < openCloseTurnAmount);
		
		Serial.println(dateTime + " | " + methodName + " | " + "Rotating finished. Detaching...");

		delay(200); // to avoid stop rotation on rotate sensor
		roletka1.detach(); //stoppig servo
		
		isBlindsOpened = false;
		blindsState = "Closed";

		Serial.println(dateTime + " | " + methodName + " | " + "Blinds is " + blindsState);
	} else 
	{
		Serial.println(dateTime + " | " + methodName + " | " + "Blinds is already " + blindsState +". isBlindsOpened - " + isBlindsOpened);
	}
}

