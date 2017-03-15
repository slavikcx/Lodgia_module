//Functions for getting data from sensors or net

void detectMotion() {

	//pirState = digitalRead(pirPin); // checkin for motion detection
	//Serial.print("Motion - ");
	//Serial.println(pirState);	
	//Serial.println(digitalRead(pirPin));

	if (digitalRead(pirPin) != pirState)
	{
		pirState = digitalRead(pirPin); // checkin for motion detection
		Serial.print("Motion - ");
		Serial.println(pirState);
	}
}


void getTemperature() {

	//Serial.println("In GetTemperatre");
	unsigned long currentMillis = millis();

	if (currentMillis - previousDHTMillis >= THinterval) {

		// save the last time you read the sensor 
		previousDHTMillis = currentMillis;

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

void getDistance()
{
	if (millis() - previousDistanceMillis >= distanceMeasuringPeriod) {
		currentdist_cm = ultrasonic.distanceRead();
		if (currentdist_cm != dist_cm && currentdist_cm != 0 && currentdist_cm < 100) {
			dist_cm = currentdist_cm;
			Serial.print("Distance:                  ");
			Serial.print(dist_cm);
			Serial.println("cm");
		}
	}
}

void getExternalData()
{
	unsigned long currentExtDataMillis = millis();

	if (currentExtDataMillis - previousExtDataMillis >= extDataInterval)
	{
		// save the last time you read external data 
		previousExtDataMillis = currentExtDataMillis;

		Serial.print("ExtTemperature = ");
		externalTemp = bmp.readTemperature();
		Serial.print(externalTemp);
		Serial.println(" *C");

		Serial.print("ExtPressure = ");
		atmPressure = bmp.readPressure() / 133.33; //need to divide on 133.33 for mmHg
		Serial.print(atmPressure); 
		Serial.println(" mmHg");

		Serial.println();
		
	}
}