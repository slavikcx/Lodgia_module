//Functions for getting data from sensors or net

void detectMotion() {

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

void getDistance()
{
	delay(500);
	Serial.println("Reading distance");

	delay(50);// 50ms delay between measuring. 29ms minimum delay.
	unsigned int uS = usSensor.ping(); // Generation signal and getting time in us(uS). 
	dist_cm = uS / US_ROUNDTRIP_CM; // Converting got time in distance (0 - out of range)

	Serial.print("Distance: ");
	Serial.print(dist_cm);
	Serial.println("cm");
	Serial.println(" ");

	// checking if next distance measure will be <10cm to prevent false LED On
	if (dist_cm < 10) {
		Serial.println("dist_cm<10");

		currentdist_cm = dist_cm;

		Serial.print("dist_cm: ");
		Serial.println(dist_cm);
		Serial.print("currentdist_cm: ");
		Serial.println(currentdist_cm);
		Serial.println(" ");
		Serial.println("verifiyng distance again");

		delay(50);// 50ms delay between measuring. 29ms minimum delay.
		unsigned int uS = usSensor.ping(); // Generation signal and getting time in us(uS). 
		dist_cm = uS / US_ROUNDTRIP_CM; // Converting got time in distance (0 - out of range)

		Serial.print("Distance: ");
		Serial.print(dist_cm);
		Serial.println("cm");
		Serial.print("currentdist_cm: ");
		Serial.println(currentdist_cm);
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

		//Serial.print("Approx altitude = ");
		//altitude = bmp.readAltitude(1028.88); // this should be adjusted to your local forcase (for  49°13'20.85"Ñ  28°25'59.14"Â  - 1028.88 - default value -1013.25, for vinnitsa - 980.40)
		//Serial.print(altitude); 
		//Serial.println(" m");

		Serial.println();
		
	}
}