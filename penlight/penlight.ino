#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ArtnetWifi.h>

#ifndef STASSID
#define STASSID "mg24connect718"
#define STAPSK  "24186263592"
#endif

#define R 14
#define G 12
#define B 4
#define led 2

WiFiUDP UdpSend;
ArtnetWifi artnet;



const char* ssid = STASSID;
const char* password = STAPSK;

void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("TVoC001");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
			String type;
			if (ArduinoOTA.getCommand() == U_FLASH) {
				type = "sketch";
			} else { // U_FS
				type = "filesystem";
			}

			// NOTE: if updating FS this would be the place to unmount FS using FS.end()
			Serial.println("Start updating " + type);
		});
  ArduinoOTA.onEnd([]() {
			Serial.println("\nEnd");
		});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
			Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
		});
  ArduinoOTA.onError([](ota_error_t error) {
			Serial.printf("Error[%u]: ", error);
			if (error == OTA_AUTH_ERROR) {
				Serial.println("Auth Failed");
			} else if (error == OTA_BEGIN_ERROR) {
				Serial.println("Begin Failed");
			} else if (error == OTA_CONNECT_ERROR) {
				Serial.println("Connect Failed");
			} else if (error == OTA_RECEIVE_ERROR) {
				Serial.println("Receive Failed");
			} else if (error == OTA_END_ERROR) {
				Serial.println("End Failed");
			}
		});
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(led, LOW);



  //
  //
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  analogWrite(R, 0);
  analogWrite(G, 0);
  analogWrite(B, 0);

  
  artnet.setArtDmxFunc([](DMX_FUNC_PARAM){
			boolean tail = false;
    
			analogWrite(R, data[0] * 2);
			analogWrite(G, data[1] * 2);
			analogWrite(B, data[2] * 2);

			Serial.print("DMX: Univ: ");
			Serial.print(universe, DEC);
			Serial.print(", Seq: ");
			Serial.print(sequence, DEC);
			Serial.print(", Data (");
			Serial.print(length, DEC);
			Serial.print("): ");
			Serial.println();

			if (length > 16) {
				length = 16;
				tail = true;
			}
			// send out the buffer
			for (int i = 0; i < length; i++) {
				Serial.print(data[i], HEX);
				Serial.print(" ");
			}
		});
	artnet.begin(); 
}

void loop() {
  ArduinoOTA.handle();
  digitalWrite(led, LOW);

  //
  //
  artnet.read();
}