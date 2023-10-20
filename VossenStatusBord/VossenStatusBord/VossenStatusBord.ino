#include <WiFi.h>
#include <PubSubClient.h>
//#include <WiFiUdp.h>
#include <EEPROM.h>

#include "WiFiFromEEPROM.h"

void callback(char* topic, byte* payload, unsigned int length);

const int PinOnDelayValue = 500; //us output on

//Wifi stuff
WiFiClient Wifi;

//MQTT broker
const char* server = "broker.hivemq.com";
PubSubClient MQTTClient(Wifi);

//Data storage
int AlphaData = 0;
int Alpha2Data = 0;
int BravoData = 0;
int Bravo2Data = 0;
int CharlieData = 0;
int Charlie2Data = 0;
int DeltaData = 0;
int Delta2Data = 0;
int EchoData = 0;
int Echo2Data = 0;
int FoxtrotData = 0;
int Foxtrot2Data = 0;

//Pinouts
const int Alpha = 2;
const int Alpha2 = 15;
const int Bravo = 5;
const int Bravo2 = 4;
const int Charlie = 19;
const int Charlie2 = 18;
const int Delta = 22;
const int Delta2 = 21;
const int Echo = 26;
const int Echo2 = 23;
const int Foxtrot = 33;
const int Foxtrot2 = 25;


const int red = 27;
const int blue = 14;
const int green = 32;



void setup() {

	/*
								   _____      _               
								  / ____|    | |              
								 | (___   ___| |_ _   _ _ __  
								  \___ \ / _ \ __| | | | '_ \ 
								  ____) |  __/ |_| |_| | |_) |
								 |_____/ \___|\__|\__,_| .__/ 
													   | |    
													   |_|  
	*/
  pinMode(Alpha, OUTPUT);
  pinMode(Alpha2, OUTPUT);
  pinMode(Bravo, OUTPUT);
  pinMode(Bravo2, OUTPUT);
  pinMode(Charlie, OUTPUT);
  pinMode(Charlie2, OUTPUT);
  pinMode(Delta, OUTPUT);
  pinMode(Delta2, OUTPUT);
  pinMode(Echo, OUTPUT);
  pinMode(Echo2, OUTPUT);
  pinMode(Foxtrot, OUTPUT);
  pinMode(Foxtrot2, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);

  digitalWrite(Alpha, HIGH);
  digitalWrite(Alpha2, HIGH);
  digitalWrite(Bravo, HIGH);
  digitalWrite(Bravo2, HIGH);
  digitalWrite(Charlie, HIGH);
  digitalWrite(Charlie2, HIGH);
  digitalWrite(Delta, HIGH);
  digitalWrite(Delta2, HIGH);
  digitalWrite(Echo, HIGH);
  digitalWrite(Echo2, HIGH);
  digitalWrite(Foxtrot, HIGH);
  digitalWrite(Foxtrot2, HIGH);
  
	Serial.begin(115200);
	
	// Start EEPROM
	EEPROM.begin(4096);
  
	//Get SSID & Password from eeprom
	get_ssid_password_EEPROM();
  
	//Connect to the read wifi network
	connect_WiFi();

	MQTTClient.setServer(server, 1883);
	MQTTClient.setCallback(callback);
	delay(50);

	AlphaData = 2;
	
}

void reconnect() {

	if (WiFi.status() != WL_CONNECTED) {
			
			Serial.print("\nWifi disconnected, attempting reconnect");
			WiFi.disconnect();
			WiFi.mode(WIFI_STA);
			char ssid_char[32];
			strcpy(ssid_char, ssid.c_str());
			WiFi.begin(ssid_char, password.c_str());

			while(WiFi.status() != WL_CONNECTED){
				Serial.print(".");
		 }
	}

	// Loop until we're reconnected
	while (!MQTTClient.connected()) {
		
		Serial.println("\nAttempting MQTT connection...");
		
		// Attempt to connect
		if (MQTTClient.connect("VossenStatusBord")) {
			Serial.println("Connected");
	
			// Once connected, publish an announcement...
			MQTTClient.publish("MrFire/Vossen/Startup", "Wemos Online");
			// and subscribe
			MQTTClient.subscribe("MrFire/Vossen/#");
			
		} else {
			Serial.print("Failed, rc=");
			Serial.print(MQTTClient.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}

}


void loop() {

  if (!MQTTClient.connected()) {
    reconnect();
  }
	
	PrintLeds();

	
	/*
							 _        _______  _______  _______ 
							( \      (  ___  )(  ___  )(  ____ )
							| (      | (   ) || (   ) || (    )|
							| |      | |   | || |   | || (____)|
							| |      | |   | || |   | ||  _____)
							| |      | |   | || |   | || (      
							| (____/\| (___) || (___) || )      
							(_______/(_______)(_______)|/ 
	*/
	
	
	
	MQTTClient.loop();

}


void callback(char* topic, byte* payload, unsigned int length)
{

	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	Serial.print((char*)payload);
	
	if(strcmp(topic, "MrFire/Vossen/Ping") == 0){

		Serial.println("Answering: [MrFire/Vossen/Startup] Wemos Online");
		MQTTClient.publish("MrFire/Vossen/Startup", "Wemos Online");
		
	
	} else if(strcmp(topic, "MrFire/Vossen/Alpha") == 0){
		AlphaData = ((int)*payload) - '0';
		Serial.print(".");
	} else if(strcmp(topic, "MrFire/Vossen/Alpha2") == 0){
		Alpha2Data = ((int)*payload) - '0';
		Serial.print(".");
	} else if(strcmp(topic, "MrFire/Vossen/Bravo") == 0){
		BravoData = ((int)*payload) - '0';
		Serial.print(".");
	} else if(strcmp(topic, "MrFire/Vossen/Bravo2") == 0){
		Bravo2Data = ((int)*payload) - '0';
		Serial.print(".");
	} else if(strcmp(topic, "MrFire/Vossen/Charlie") == 0){
		CharlieData = ((int)*payload) - '0';
		Serial.print(".");
	} else if(strcmp(topic, "MrFire/Vossen/Charlie2") == 0){
		Charlie2Data = ((int)*payload) - '0';
		Serial.print(".");
	} else if(strcmp(topic, "MrFire/Vossen/Delta") == 0){
		DeltaData = ((int)*payload) - '0';
		Serial.print(".");
	} else if(strcmp(topic, "MrFire/Vossen/Delta2") == 0){
		Delta2Data = ((int)*payload) - '0';
		Serial.print(".");
	} else if(strcmp(topic, "MrFire/Vossen/Echo") == 0){
		EchoData = ((int)*payload) - '0';
		Serial.print(".");
	} else if(strcmp(topic, "MrFire/Vossen/Echo2") == 0){
		Echo2Data = ((int)*payload) - '0';
		Serial.print(".");
	} else if(strcmp(topic, "MrFire/Vossen/Foxtrot") == 0){
		FoxtrotData = ((int)*payload) - '0';
		Serial.print(".");
	} else if(strcmp(topic, "MrFire/Vossen/Foxtrot2") == 0){
		Foxtrot2Data = ((int)*payload) - '0';
		Serial.print(".");
	}
	
	Serial.println();

}

void PrintLeds(){
	/*
						 __    __      _ _       
						/ / /\ \ \_ __(_) |_ ___ 
						\ \/  \/ / '__| | __/ _ \
						 \  /\  /| |  | | ||  __/
						  \/  \/ |_|  |_|\__\___|
                         
	*/

	//Write Alpha
  digitalWrite(Alpha, LOW);
  if (AlphaData == 0){
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
  }else if (AlphaData == 2){
    digitalWrite(green, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }else if (AlphaData == 1){
    digitalWrite(green, HIGH);
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }
  digitalWrite(Alpha, HIGH);
  
  //Write Alpha2
  digitalWrite(Alpha2, LOW);
  if (Alpha2Data == 0){
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
  }else if (Alpha2Data == 2){
    digitalWrite(green, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }else if (Alpha2Data == 1){
    digitalWrite(green, HIGH);
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }
  digitalWrite(Alpha2, HIGH);
  
  //Write Bravo
  digitalWrite(Bravo, LOW);
  if (BravoData == 0){
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
  }else if (BravoData == 2){
    digitalWrite(green, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }else if (BravoData == 1){
    digitalWrite(green, HIGH);
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }
  digitalWrite(Bravo, HIGH);
  
  //Write Bravo2
  digitalWrite(Bravo2, LOW);
  if (Bravo2Data == 0){
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
  }else if (Bravo2Data == 2){
    digitalWrite(green, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }else if (Bravo2Data == 1){
    digitalWrite(green, HIGH);
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }
  digitalWrite(Bravo2, HIGH);
  
  //Write Charlie
  digitalWrite(Charlie, LOW);
  if (CharlieData == 0){
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
  }else if (CharlieData == 2){
    digitalWrite(green, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }else if (CharlieData == 1){
    digitalWrite(green, HIGH);
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }
  digitalWrite(Charlie, HIGH);
  
  //Write Charlie2
  digitalWrite(Charlie2, LOW);
  if (Charlie2Data == 0){
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
  }else if (Charlie2Data == 2){
    digitalWrite(green, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }else if (Charlie2Data == 1){
    digitalWrite(green, HIGH);
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }
  digitalWrite(Charlie2, HIGH);
  
  //Write Delta
  digitalWrite(Delta, LOW);
  if (DeltaData == 0){
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
  }else if (DeltaData == 2){
    digitalWrite(green, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }else if (DeltaData == 1){
    digitalWrite(green, HIGH);
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }
  digitalWrite(Delta, HIGH);
  
  //Write Delta2
  digitalWrite(Delta2, LOW);
  if (Delta2Data == 0){
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
  }else if (Delta2Data == 2){
    digitalWrite(green, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }else if (Delta2Data == 1){
    digitalWrite(green, HIGH);
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }
  digitalWrite(Delta2, HIGH);
  
  //Write Echo
  digitalWrite(Echo, LOW);
  if (EchoData == 0){
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
  }else if (EchoData == 2){
    digitalWrite(green, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }else if (EchoData == 1){
    digitalWrite(green, HIGH);
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }
  digitalWrite(Echo, HIGH);
  
  //Write Echo2
  digitalWrite(Echo2, LOW);
  if (Echo2Data == 0){
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
  }else if (Echo2Data == 2){
    digitalWrite(green, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }else if (Echo2Data == 1){
    digitalWrite(green, HIGH);
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }
  digitalWrite(Echo2, HIGH);
  
  //Write Foxtrot
  digitalWrite(Foxtrot, LOW);
  if (FoxtrotData == 0){
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
  }else if (FoxtrotData == 2){
    digitalWrite(green, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }else if (FoxtrotData == 1){
    digitalWrite(green, HIGH);
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }
  digitalWrite(Foxtrot, HIGH);
  
  //Write Foxtrot2
  digitalWrite(Foxtrot2, LOW);
  if (Foxtrot2Data == 0){
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
  }else if (Foxtrot2Data == 2){
    digitalWrite(green, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }else if (Foxtrot2Data == 1){
    digitalWrite(green, HIGH);
    digitalWrite(red, HIGH);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(red, LOW);
    delayMicroseconds(PinOnDelayValue);
    digitalWrite(green, LOW);
  }
  digitalWrite(Foxtrot2, HIGH);

}




	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
