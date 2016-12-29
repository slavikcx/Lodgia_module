# Lodjia_module
ESP8266 sketch for Lodjia module of Smart Flat

22.12.16 - Test web page added using aREST and aREST_UI libs. Actual T and H shows on Tests page.
	 - Average counting of T and H implemented. On Test page displays average values.
26.12.16 - Getting time from NTP server and show it on Test page implemented.

29.12.16 - Remove of using aREST and aREST_UI libs becouse there is no abilitu to ajust webpage. Instead of this start using ESP8266WebServer lib and cretae functions to generate web page and handle request from client. Add cuntrol of PIN using html buttons.