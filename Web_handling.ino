//Web handling function

void WebServerHandleRoot()
{
	String methodName = "WebServerHandleRoot";

	//Serial.println(dateTime + " | " + methodName + " | " + "Root request recieved. Building web page...");
	
	webPage = buildWebPage();

	//Serial.println(dateTime + " | " + methodName + " | " + "Building page finished. Sending page: "+ webPage);

	webServer.send(200, "text/html", webPage);
	/*//Serial.println("");
	////Serial.print("Web Page sent - ");
	//Serial.println(webPage);
	//Serial.println("");*/

}

void WebServerHandleXML()
{
	String methodName = "WebServerHandleXML";

	//Serial.println(dateTime + " | " + methodName + " | " + "XML request recieved. Building XML...");

	xml = buildXML();

	//Serial.println(dateTime + " | " + methodName + " | " + "Building XML finished. Sending XML: "+ xml);

	webServer.send(200, "text/xml", xml);

	/*//Serial.println("");
	//Serial.println("xml Sent");
	//Serial.println(xml);
	//Serial.println("");*/
}

void WebServerHandleLightOn()
{
	String methodName = "WebServerHandleLightOn";

	//Serial.println(dateTime + " | " + methodName + " | " + "Ligh ON request recieved. Turning on light ...");

	isLightOn = false; // to be able to turn Hard Light on eve if light already ON by sensor

	LightOn();

	isHardLightOn = true;

	//Serial.println(dateTime + " | " + methodName + " | " + "HardLightOn - " + isHardLightOn);
	//Serial.println(dateTime + " | " + methodName + " | " + "Sending response to client: 200," + "text/html, " + "OK");

	webServer.send(200, "text/html", "OK");
}

void WebServerHandleLightOff()
{
	String methodName = "WebServerHandleLightOn";

	//Serial.println(dateTime + " | " + methodName + " | " + "Ligh OFF request recieved. Turning off light ...");

	LightOff();

	isHardLightOn = false;

	webServer.send(200, "text/html", "OK");
}

void WebServerHandlePowerOn() {
	
	String methodName = "WebServerHandlePowerOn";

	//Serial.println(dateTime + " | " + methodName + " | " + "Power ON request recieved. Turning on power ...");

	if (!isPowerOn)
	{
		//Serial.println(dateTime + " | " + methodName + " | " + "Switching On Power...");
		
		//digitalWrite(powerPin, HIGH);
		
		isPowerOn = true;
		powerState = "ON";

		//Serial.println(dateTime + " | " + methodName + " | " + "Power is " + powerState + ". isPowerOn - " + isPowerOn);

		//Serial.println(dateTime + " | " + methodName + " | " + "Sending response to client: 200," + "text/html, " + "OK");

		webServer.send(200, "text/html", "OK");
	} else
	{
		//Serial.println(dateTime + " | " + methodName + " | " + "Power is already" + powerState);

		//Serial.println(dateTime + " | " + methodName + " | " + "Sending response to client: 200," + "text/html, " + "OK");

		webServer.send(200, "text/html", "OK");
	}
}

void WebServerHandlePowerOff() {

	String methodName = "WebServerHandlePowerOff";

	//Serial.println(dateTime + " | " + methodName + " | " + "Power OFF request recieved. Turning off power ...");

	if (isPowerOn)
	{
		//Serial.println(dateTime + " | " + methodName + " | " + "Switching Off Power...");

		//digitalWrite(powerPin, LOW);

		isPowerOn = false;
		powerState = "Off";

		//Serial.println(dateTime + " | " + methodName + " | " + "Power is " + powerState + ". isPowerOn - " + isPowerOn);

		//Serial.println(dateTime + " | " + methodName + " | " + "Sending response to client: 200," + "text/html, " + "OK");

		webServer.send(200, "text/html", "OK");
	} else
	{
		//Serial.println(dateTime + " | " + methodName + " | " + "Power is already" + powerState);

		//Serial.println(dateTime + " | " + methodName + " | " + "Sending response to client: 200," + "text/html, " + "OK");

		webServer.send(200, "text/html", "OK");
	}
}

void WebServerHandleBlindsOpen()
{
	String methodName = "WebServerHandleBlindsOpen";

	//Serial.println(dateTime + " | " + methodName + " | " + "Blinds open request recieved. Opening blinds ...");

	OpenBlinds();

	//Serial.println(dateTime + " | " + methodName + " | " + "isBlindsOpened - " + isBlindsOpened);
	//Serial.println(dateTime + " | " + methodName + " | " + "Sending response to client: 200," + "text/html, " + "OK");

	webServer.send(200, "text/html", "OK");
}

void WebServerHandleBlindsClose()
{
	String methodName = "WebServerHandleBlindsClose";

	//Serial.println(dateTime + " | " + methodName + " | " + "Blinds close request recieved. Closing blinds ...");

	CloseBlinds();

	//Serial.println(dateTime + " | " + methodName + " | " + "isBlindsOpened - " + isBlindsOpened);
	//Serial.println(dateTime + " | " + methodName + " | " + "Sending response to client: 200," + "text/html, " + "OK");

	webServer.send(200, "text/html", "OK");
}
