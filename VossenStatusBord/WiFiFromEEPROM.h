
String ssid = "123456789012345678901234567890";
String password = "098765432109876543210987654321";

//#define DebugMode

void get_ssid_password_EEPROM() {

	int address = 0;
	ssid = "";
	password = "";

	if (EEPROM.read(address) == '0') {

		#ifdef Debugmode
		Serial.println("EEPROM not empty, reading: ");
		#endif

		char value;
		int i = 0;
		address++;

		for (;(value = EEPROM.read(address)) != '\0' && i < 32; address++) {
			ssid += char(value);
			i++;
		}

		address++;

		#ifdef DebugMode
		Serial.print(ssid);
		#endif

		if (i >= 32) {
			Serial.println("EEPROM ssid too many characters");
			return;
		}

		#ifdef DebugMode
		Serial.print(" ");
		#endif

		i = 0;
		for (; (value = EEPROM.read(address)) != '\0' && i < 32; address++) {
			password+= char(value);
			i++;
		}

		#ifdef DebugMode
		Serial.print(password);
		#endif

		if (i >= 32) {
			Serial.println("EEPROM password too many characters");
			return;
		}

	} else {
		#ifdef DebugMode
		Serial.print("EEPROM was empty");
		#endif
	}

}

void connect_WiFi() {
	
	
	int i = 0;
	int address = 0;
	int Timeout = 0;
	char temp;

	Serial.println();
	Serial.print("Connecting to: ");
	Serial.println(ssid);
	Serial.print("With: ");
	Serial.println(password);

	/* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
	would try to act as both a client and an access-point and could cause
	network-issues with your other WiFi-devices on your WiFi-network. */
	WiFi.mode(WIFI_STA);
	
	char ssid_char[32];
	strcpy(ssid_char, ssid.c_str());
	WiFi.begin(ssid_char, password.c_str());
	
	while (WiFi.status() != WL_CONNECTED) {

		delay(1000);
		Timeout++;

		#ifdef DebugMode
		Serial.print(".");
		#endif

		if (Timeout > 60) {

			Serial.print("\nError connecting to the WiFi network");
			Serial.print("\nPlease enter new ssid: ");
			int IsAnyoneThere = 0;
			while (!(Serial.available() > 0) && IsAnyoneThere < 300) {
				IsAnyoneThere++;
				delay(100);
			}
			
			if (IsAnyoneThere < 300) {

				EEPROM.write(address, '0');
				ssid = "";
				password = "";
				address++;

				while(Serial.available() > 0){
					temp = (char)Serial.read();
					if(!((temp == '\n') || (temp == '\r'))){
						ssid += temp;
						EEPROM.write(address, ssid[i]);
						address++;
						i++;
						delay(5);
					}
				}
				
				EEPROM.write(address, '\0');
				address++;

				Serial.print("\n\nPlease enter password: ");
				while (!(Serial.available() > 0)) {
					;
				}

				i = 0;
				while(Serial.available() > 0){
					temp = (char)Serial.read();
					if(!((temp == '\n') || (temp == '\r'))){
						password += temp;
						EEPROM.write(address, password[i]);
						address++;
						i++;
						delay(5);
					}
				}

				EEPROM.write(address, '\0');

				EEPROM.commit();

			}

			Serial.println();
			Serial.print("Connecting to: ");
			Serial.println(ssid);
			Serial.print("With: ");
			Serial.println(password);

			/* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
			would try to act as both a client and an access-point and could cause
			network-issues with your other WiFi-devices on your WiFi-network. */
			WiFi.mode(WIFI_STA);
			//ssid += '\0';
			ssid.toCharArray(ssid_char, ssid.length());
			WiFi.begin(ssid_char, password.c_str());

			Timeout = 0;
		}

	}

	Serial.println("WiFi connected");

	#ifdef DebugMode
	Serial.println("");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	#endif

}