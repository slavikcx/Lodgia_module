//Functions for getting data from sensors or net

void detectMotion() 
{
	String methodName = "detectMotion";

	if (digitalRead(pirPin) != pirState)
	{
		pirState = digitalRead(pirPin); // checkin for motion detection
		//Serial.println(dateTime + " | " + methodName + " | " + "Motion - " + pirState);
	}
	else
	{
		//Serial.println(dateTime + " | " + methodName + " | " + "No changes.");
	}
}

void getTemperature() {

	String methodName = "getTemperature";
	////Serial.println("In GetTemperatre");
	unsigned long currentMillis = millis();

	if (currentMillis - previousDHTMillis >= THinterval) {

		// save the last time you read the sensor 
		previousDHTMillis = currentMillis;

		// Get temperature event and print its value.
		sensors_event_t event;

		dht.temperature().getEvent(&event);
		if (isnan(event.temperature)) 
		{
			//Serial.println(dateTime + " | " + methodName + " | " + "Error reading temperature!");
		}
		else 
		{
			insideTemperature = event.temperature - 1; //1 - calibration
			//Serial.println(dateTime + " | " + methodName + " | " + "Temperature: " + event.temperature + " *C");
		}
		// Get humidity event and print its value.
		dht.humidity().getEvent(&event);

		if (isnan(event.relative_humidity)) 
		{
			//Serial.println(dateTime + " | " + methodName + " | " + "Error reading humidity!");
		}
		else 
		{
			insideHumidity = event.relative_humidity;
			//Serial.println(dateTime + " | " + methodName + " | " + "Humidity: " + event.relative_humidity + " %");
		}
	}
	else
	{
		//Serial.println(dateTime + " | " + methodName + " | " + "No Updates.");
	}
	////Serial.println("exition from get Temperature");
}

void getDistance()
{
	String methodName = "getDistance";

	if (millis() - previousDistanceMillis >= distanceMeasuringPeriod) 
	{
		currentdist_cm = ultrasonic.distanceRead();
		//Serial.println(dateTime + " | " + methodName + " | " + "Getting distance.");
		if (currentdist_cm != dist_cm && currentdist_cm != 0 && currentdist_cm < 100) 
		{
			dist_cm = currentdist_cm;
			//Serial.println(dateTime + " | " + methodName + " | " + "Distance: " + dist_cm + " cm.");
		}
		else 
		{
			//Serial.println(dateTime + " | " + methodName + " | " + "Distance not changed.");
		}
	}
	else
	{
		//Serial.println(dateTime + " | " + methodName + " | " + "No Updates.");
	}
}

void getExternalData()
{
	String methodName = "getExternalData";
	unsigned long currentExtDataMillis = millis();
	bool isTempUpdated, isPressureUpdated = false;

	if (currentExtDataMillis - previousExtDataMillis >= extDataInterval)
	{
		
		float tempExternalTemp = bmp.readTemperature(); //1 - calibration
		//Serial.println(dateTime + " | " + methodName + " | " + "tempExternalTemp = " + tempExternalTemp);
		externalTemp = tempExternalTemp; //need to delete if uncomment below
		
		//Serial.println(dateTime + " | " + methodName + " | " + "ExtTemperature = " + externalTemp + " *C; "+ "( tempExternalTemp" + tempExternalTemp + "isTempUpdated " + isTempUpdated + " )");
		

		int tempAtmPressure = bmp.readPressure() / 133.33; //need to divide on 133.33 for mmHg
		//Serial.println(dateTime + " | " + methodName + " | " + "tempAtmPressure = " + tempAtmPressure);
		atmPressure = tempAtmPressure; //need to delete if uncomment below
		previousExtDataMillis = currentExtDataMillis; //need to delete if uncomment below

		//Serial.println(dateTime + " | " + methodName + " | " + "ExtPressure = " + atmPressure + " mmHg; " + "( tempAtmPressure " + tempAtmPressure + "isPressureUpdated " + isPressureUpdated + " );  previousExtDataMillis " + previousExtDataMillis);
		
	}
	else
	{
		//Serial.println(dateTime + " | " + methodName + " | " + "No Updates.");
	}
}