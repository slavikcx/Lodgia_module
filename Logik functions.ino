//Module logic functions


void lightOn()
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
	}
}

void lightOff()
{
	if (isLightOn)
	{
		Serial.println("Switching Off light");
		for (int i = maxTranzLevel; i >= 0; i--)
		{
			analogWrite(lightPin, i);
			delay(1);
		}
		isLightOn = false;
		lightState = "Off";
	}
}

void lightHardOnOff()
{
	if (dist_cm<10 && currentdist_cm<10 && hardledOn == false) {  //Turn On Light

		Serial.println(" ");
		Serial.println("Hard Turn ON");
		lightOn();
		hardledOn = !hardledOn; // changing hard switch state
		dist_cm = dist_cm + 10; // adding 10sm to current destance to prevent imidiate hard light Off
	}

	//Checking if hard Light Off is need

	if (dist_cm<10 && currentdist_cm<10 && hardledOn == true) {  //Turn Off  Light

		Serial.println(" ");
		Serial.println("Hard Turn OFF");
		lightOff();
		hardledOn = !hardledOn; // changing hard switch state
		dist_cm = dist_cm + 10; // adding 10sm to current distance to prevent automatic led On just after turning hard Off 
	}

}

String getTime() {


	unsigned long currentTimeMillis = millis();
	if (WiFi.localIP() != AccessPointIP && WiFi.localIP() != defaultIp)
	{
		if (currentTimeMillis - previousTimeMillis >= DTinterval)
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

				//Serial.println("SEND REQUEST");
				client.print("HEAD / HTTP/1.1\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; ESP8266 NodeMcu Lua;)\r\n\r\n");

				delay(10);


				while (client.available())
				{
					String line = client.readStringUntil('\r');

					//Parsing date time from server responce
					dateTime = line.substring(0, 11); // getting date
					dateTime += line.substring(20, 25); //getting year
					dateTime += line.substring(10, 20); // getting time
					Serial.println(dateTime);
					Serial.println("");
					return dateTime;
				}
			}

			dateTime = "Time server unavailable";
			return dateTime;
		}
	}
	else {
		dateTime = "WiFi network not available";
	}

}

String moduleUpTime() {
	String Time = "";
	Time = convertMillisToTime(millis());
	return Time;
}

void getBrightness()
{
	Serial.println("Getting brightness");
	brightness = analogRead(A0);
	Serial.print("Brightnes - ");
	Serial.println(brightness);
}


String convertMillisToTime(unsigned long millisValue) //
{
	String Time = "";
	unsigned long ss;
	byte mm, hh, dd;
	ss = millisValue / 1000; //converting millis to seconds
	dd = ss / 86400; // geeting days 
	hh = (ss - dd * 86400) / 3600; // getting hours 
	mm = (ss - dd * 86400 - hh * 3600) / 60; //getting minutes 
	ss = ss - dd * 86400 - hh * 3600 - mm * 60; // getting seconds 

	Time += (String)dd += "days ";
	if (hh<10)Time += "0";
	Time += (String)hh + ":";
	if (mm<10)Time += "0";
	Time += (String)mm + ":";
	if (ss<10)Time += "0";
	Time += (String)ss;
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

