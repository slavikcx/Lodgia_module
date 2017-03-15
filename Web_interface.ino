//Web interface building function

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
	page += "xmldoc = xmlResponse.getElementsByTagName('insideTemperature'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('insideTemperature').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('humidity'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('humidity').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('lightState'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('lightState').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('powerState'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('powerState').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('pirState'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('pirState').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('moduleUpTime'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('moduleUpTime').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('outsideTemperature'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('outsideTemperature').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('pressure'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('pressure').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('distance'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('distance').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('roletState'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('roletState').innerHTML = message;";
	page += "xmldoc = xmlResponse.getElementsByTagName('brightness'); message = xmldoc[0].firstChild.nodeValue;  document.getElementById('brightness').innerHTML = message;";


	page += "}}";

	page += "function sendButton(Parameter){";
	page += "server = '/' + Parameter; ";
	page += "request = new XMLHttpRequest();";
	page += "request.open('GET', server, true);";
	page += "request.send();}";


	page += "</SCRIPT>";


	page += "<html> <head> <title>Lodjia</title> </head>";
	page += "<body onload='process()'>"; //runnig function when page loaded

	page += "<h1>Test page                   <span style='font-size:9px'>--UP Time <span id='moduleUpTime'></span>--</span></h1>";	//page header

	page += "<h2 id='dateTime'> </h2>"; //show date and time

	page += "<div style='position: relative; height: 250px; width: 1000px'>"; //outside image region position and size
	page += "<div style='position: absolute; top: 0; left: 0; width: 250px'>";
	page += "<img src='https://encrypted-tbn1.gstatic.com/images?q=tbn:ANd9GcRQWgi9bbMS4OA1IzrnlR2C5a3w7HmWrnJ5hBTfSaveZhQHrbFP' style='height: 250px; width: 480px' /></div>"; //external image source and size
	page += "<div style='position: absolute; top: 4%; left: 7%; width: 300px; font-size:600%; color:Brown; font-weight:bold'><span id='outsideTemperature'></span>oC</div>"; //show insideTemperature
	page += "<div style='position: absolute; top: 45%; left: 8%; width: 300px; font-size:600%; color:Brown; font-weight:bold'><span id='pressure'></span>mm</div>";//show atmospheric pressure
	
	page += "<div style='position: absolute; top: 0; left: 50%; width: 250px'>";
	page += "<img src='http://sharapova-design.ru/upload/29042013/information_items_297b.jpg' style='height: 250px; width: 480px' /></div>"; //external image source and size
	page += "<div style='position: absolute; top: 4%; left: 57%; width: 300px; font-size:600%; color:Brown; font-weight:bold'><span id='insideTemperature'></span>oC</div>"; //show insideTemperature
	page += "<div style='position: absolute; top: 45%; left: 58%; width: 200px; font-size:600%; color:Brown; font-weight:bold'><span id='humidity'></span>%</div>";//show humidity

	
	page += "</div>";
   
	
	


	//page += "<h2>Temperature: <span id='insideTemperature'> </span></h2>"; //show insideTemperature
	//page += "<h2>Humidity: <span id='humidity'> </span></h2>"; //show humidity
	page += "<h2>Light <span id='lightState'> </span></h2>"; //show light state

															 //page += "";

	page += "<h1> <button class='button green' onclick='sendButton(&quot;lightOn&quot;);'>Light On</button>";
	page += "<button class='button blue' onclick='sendButton(&quot;lightOff&quot;);'>Light Off</button> </h1>";

	page += "<h2>Power <span id='powerState'> </span></h2>"; //show power state

	page += "<h1> <button class='button green' onclick='sendButton(&quot;powerOn&quot;);'>Power On</button>";
	page += "<button class='button red' onclick='sendButton(&quot;powerOff&quot;);'>Power Off</button> </h1>";

	page += "<h2>Blinds <span id='roletState'> </span></h2>"; //show blinds state

	page += "<h1> <button class='button green' onclick='sendButton(&quot;blindsOpen&quot;);'>Open</button>";
	page += "<button class='button blue' onclick='sendButton(&quot;blindsClose&quot;);'>Close</button> </h1>";

	page += "<h2>Motion <span id='pirState'> </span></h2>"; //show pir state

	page += "<h2>Distance <span id='distance'> </span></h2>"; //show distance 

	page += "<h2>Brightness <span id='brightness'> </span></h2>"; //show brightness 

	page += "</body> </html>";

	return page;
}


String buildXML()
{
	String xml = "";

	xml += "<?xml version='1.0'?>";
	xml += "<Donnees>";

	xml += "<dateTime>";
	xml += dateTime;
	xml += "</dateTime>";

	xml += "<moduleUpTime>";
	xml += moduleUpTime;
	xml += "</moduleUpTime>";

	xml += "<insideTemperature>";
	xml += insideTemperature;
	xml += "</insideTemperature>";

	xml += "<outsideTemperature>";
	xml += externalTemp;
	xml += "</outsideTemperature>";

	xml += "<humidity>";
	xml += insideHumidity;
	xml += "</humidity>";

	xml += "<pressure>";
	xml += atmPressure;
	xml += "</pressure>";

	xml += "<lightState>";
	xml += lightState;
	xml += "</lightState>";

	xml += "<powerState>";
	xml += powerState;
	xml += "</powerState>";

	xml += "<roletState>";
	xml += roletState;
	xml += "</roletState>";

	xml += "<pirState>";
	xml += pirState;
	xml += "</pirState>";

	xml += "<distance>";
	xml += dist_cm;
	xml += "</distance>";

	xml += "<brightness>";
	xml += brightness;
	xml += "</brightness>";

	xml += "</Donnees>";

	return xml;
}

