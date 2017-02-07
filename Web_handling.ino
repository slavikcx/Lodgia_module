//Web handling function

void handle_root()
{
	webPage = buildWebPage();
	webServer.send(200, "text/html", webPage);
	/*//Serial.println("");
	//Serial.print("Web Page sent - ");
	//Serial.println(webPage);
	//Serial.println("");*/

}

void handle_xml()
{
	xml = buildXML();
	webServer.send(200, "text/xml", xml);
	/*//Serial.println("");
	//Serial.println("xml Sent");
	//Serial.println(xml);
	//Serial.println("");*/

}

void handle_lightOn()
{
	lightOn();
	webServer.send(200, "text/html", "OK");
}

void handle_powerOn() {
	if (!isPowerOn)
	{
		//Serial.println("Switching On Power");
		digitalWrite(powerPin, HIGH);
		isPowerOn = true;
		powerState = "ON";
		webServer.send(200, "text/html", "OK");
	}
}

void handle_powerOff() {
	if (isPowerOn)
	{
		//Serial.println("Switching Off Power");
		digitalWrite(powerPin, LOW);
		isPowerOn = false;
		powerState = "Off";
		webServer.send(200, "text/html", "OK");
	}
}

void handle_lightOff()
{
	lightOff();
	webServer.send(200, "text/html", "OK");
}


