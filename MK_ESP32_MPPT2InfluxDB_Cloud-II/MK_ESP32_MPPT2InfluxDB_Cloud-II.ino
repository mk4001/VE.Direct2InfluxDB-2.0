/*
  © MK 15 Jan 2024
  Write Victron Energy MPPT fields to INfluxDB on Cloud
  based on ESP32 Wroom using Uses VeDirectFrameHandler library
*/

/*************************************************************************************
 ReadVeDirectFrameHandler

 Uses VeDirectFrameHandler library

 This example and library tested with NodeMCU 1.0 using Software Serial.
 If using with a platform containing 2 harware UART's, use those, not SoftwareSerial.
 Tested with Victron BMV712.

 VEDirect Device:
   pin 1 - gnd
   pin 2 - RX
   pin 3 - TX
   pin 4 - power

 History:
   2020.05.05 - 0.3 - initial release

**************************************************************************************/

#include "Arduino.h"
#include "VeDirectFrameHandler.h"

#include <WiFi.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

long rec_count = 0;

const char* ssid = "WLAN 7490-2.4GHz";
const char* password = "06431928919070006302";

// InfluxDB Cloud Configuration
const char* influxDBUrl = "https://eu-central-1-1.aws.cloud2.influxdata.com";
const char* influxDBToken = "vlYHoJH6PrcDepvl8MCt886dJhmIrfFwvS8I8M0d5OmAgxTIZEzA0wm1EQMrkkRZ9xpQZsV1nLdEVER0ilWHog==";
const char* influxDBOrg = "447ab0b6dc55f6f8";
const char* influxDBBucket = "Victron";

/*
// InfluxDB on Premise Configuration
const char* influxDBUrl = "http://rpi3bp:8086";
const char* influxDBToken = "kGVg7zzUkhQjoXBgSQ-tDXSVRl4j7XmRt16VVUsu_Imkd7lwjX2TMYycV3Pfa4FUZHR6Pmh46db2qXwwhJW-_A==";
const char* influxDBOrg = "ae1e888a12c8e19c";
const char* influxDBBucket = "Victron";
*/

// Time zone info
#define TZ_INFO "UTC2"

InfluxDBClient influxDB(influxDBUrl, influxDBOrg, influxDBBucket, influxDBToken, InfluxDbCloud2CACert);

// Declare Data point
Point point("SmartSolar");

VeDirectFrameHandler myve;

//Define Seria port #2 PIN 16(RX) - 17(TX - not used)
#define SERIAL_PORT Serial2


void setup() {
  Serial.begin(115200);  // output serial port

  SERIAL_PORT.begin(19200);  // input serial port (VE device)
  SERIAL_PORT.flush();
  Serial.println("DEBUG-setup");

   // log helper
  myve.setErrorHandler(&LogHelper);
  // hex protocol callback
  myve.addHexCallback(&HexCallback, (void*)42);


  connectToWiFi();

  // Accurate time is necessary for certificate validation and writing in batches
  // We use the NTP servers in your area as provided by: https://www.pool.ntp.org/zone/
  // Syncing progress and the time will be printed to Serial.
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Check server connection
  if (influxDB.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(influxDB.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(influxDB.getLastErrorMessage());
  }

  point.addTag("device", "mppt_victron");

}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WIFI - CONNECTION LOST...");
    delay(1000);
    //ESP.restart();
  }
  ReadVEData();
  PrintData();
  //EverySecond();
}