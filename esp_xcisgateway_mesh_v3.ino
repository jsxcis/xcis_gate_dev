#include <XcisSensor.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266Ping.h>

#include <RHRouter.h>
#include <RHMesh.h>
#include <RH_RF95.h>

uint8_t nodeId = 99; // Gateway LoraID

#include "EEPROMAnything.h"

#define debug

// ESP-12C Configuration NODEMCU1.0
#define RFM95_CS 15
#define RFM95_RST 16
#define RFM95_INT 5

// XCIS Gateway Configuration
//#define RFM95_CS 4
//#define RFM95_RST 16
//#define RFM95_INT 5

#define XCIS_RH_MESH_MAX_MESSAGE_LEN 32

//#define RH_HAVE_SERIAL

// Singleton instance of the radio driver

RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHMesh *manager;

// message buffer
uint8_t buf[XCIS_RH_MESH_MAX_MESSAGE_LEN];

ESP8266WebServer *server;

int loopCount = 0;

// Japan configuration
//const char *ssid = "Buffalo-G-A5F8";
//const char *password = "bvvved6gfgfar";
//IPAddress staticIP(10,141,109,50);
//IPAddress gateway(10,141,108,1);  
//IPAddress subnet(255,255,252,0);
// sharpnet configuration
const char *ssid = "sharpnet";
const char *password = "0294400648";
IPAddress staticIP(192,168,0,50);
IPAddress gateway(192,168,0,1);  
IPAddress subnet(255,255,255,0);

// Mobile configuration
//const char *ssid = "XCIS_Router";
//const char *password = "xcis12345";
//IPAddress staticIP(192,168,5,50);
//IPAddress gateway(192,168,5,1);  
//IPAddress subnet(255,255,255,0);


#define UPLINK 0
// Indicator LEDs - input pins
//const int networkLed=4;

void(* resetFunc)(void) = 0;

unsigned long delayStart = 0; // the time the delay started
bool delayRunning = false; // true if still waiting for delay to finish
unsigned long DELAY_TIME = 10000; // Scan Rate
unsigned long delayOnlineCheckStart = 0; // the time the delay started
bool delayOnlineCheck = false;
unsigned long CHECK_ONLINE_TIME = 60000; // 1 * 60000msec

XcisSensor sensors;

int wifiAttempt = 0;

struct sensor
{
  char loraID[16];
  char deviceType[16];
};

struct config_t
{
    boolean changeFlag;
    char gatewayName[64] = "";
    char ssid[32] = "";
    char pkey[64] = "";
    char softssid[32] = "XCIS_Sensor_Dev";
    char softpkey[64] = "xcis12345";
    char userName[10] = "";
    char password[10] = "";
    boolean staticIPFlag;
    char staticIP[32];
    char gateway[32];
    char mac[32];
    char subnet[32];
    char dns1[32];
    char portNumber[32];
    sensor scanList[64];
    int scanListLength;
    unsigned long scanRate;
} configuration;

// Web Pages
char *loginPage =
#include "login.h"
;
char *gatewayMain = 
#include "gatewayMain.h"
;
char *sensorList =
#include "sensorList.h"
;
char *sensorListHelp =
#include "sensorListHelp.h"
;
boolean networkState = false;
boolean loggedIn = false;
//=======================================================================
void checkOnline()
{
  #ifdef debug
  Serial.println("CheckOnline-------------------------------------");
  #endif
  sensors.checkSensorsOnline();
}
String response = "NULL";
void scanSensors()
{
    int sensorToScan = sensors.scanNextSensor();
    if (sensorToScan != -1)
    {
      String req = sensors.getSensorLoraID(sensorToScan) + ":REQUEST:D25:EOM";
      digitalWrite(UPLINK, LOW);//ON
      processCommand(req);
      digitalWrite(UPLINK, HIGH);//OFF
    }
  //digitalWrite(UPLINK, LOW);//ON
}

void setup()
{
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
    pinMode(RFM95_INT, INPUT); 
    
    pinMode(UPLINK, OUTPUT);
    digitalWrite(UPLINK, HIGH);

    delay(100);
    Serial.begin(115200);
    #ifdef debug
    Serial.printf("\n\nSdk version: %s\n", ESP.getSdkVersion());
    Serial.printf("Core Version: %s\n", ESP.getCoreVersion().c_str());
    Serial.printf("Boot Version: %u\n", ESP.getBootVersion());
    Serial.printf("Boot Mode: %u\n", ESP.getBootMode());
    Serial.printf("CPU Frequency: %u MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("Flash Chip Size: %s\n", String(ESP.getFlashChipSize()).c_str());
    #endif
    rst_info* rinfo = ESP.getResetInfoPtr();
    #ifdef debug
    Serial.printf("rinfo->reason:   %d, %s\n", rinfo->reason, ESP.getResetReason().c_str());
    #endif
    EEPROM.begin(4096);
    setDefaults();
    setDefaultScanlist();
    readConfiguration();
    #ifdef debug
    displayConfiguration();
    #endif
    manager = new RHMesh(rf95, nodeId);

    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
    
    if (!manager->init()) {
      Serial.println(F("init failed"));
    } else {
    Serial.println("done");
    }
    rf95.setTxPower(23, false);
    rf95.setFrequency(915.0);
    rf95.setCADTimeout(500);
    
    server = new ESP8266WebServer(80);
    if (connectWifi())
    {
      Serial.println("Connected to Wifi");
      launchWeb(0);
      displayConfiguration();
      //pingCheck();  
    }
    else
    {
      Serial.println("Could not connect to Wifi");
      // Configure access point
      Serial.print("Configuring access point...");
      WiFi.mode(WIFI_AP);
      WiFi.softAP(configuration.softssid, configuration.softpkey);
      IPAddress myIP = WiFi.softAPIP();
      Serial.print("Soft Access point IP address: ");
      Serial.println(myIP);
      launchWeb(0);
    }
    
    initaliseScanList();
    sensors.listSensors(false);
    
    delayStart = millis();   // start delay
    delayRunning = true; // not finished yet
    delayOnlineCheckStart = millis();
    delayOnlineCheck = true;
    Serial.println("RH LoRa Gateway init succeeded.");
    Serial.println("Ready");
    Serial.println("");
    Serial.println();
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress()); 
}
void loop()
{
   // check if online check delay has timed out after Xsec == CHECK_ONLINE_TIME
  if (delayOnlineCheck && ((millis() - delayOnlineCheckStart) >= CHECK_ONLINE_TIME))
  {
    delayOnlineCheckStart += CHECK_ONLINE_TIME; 
    //delayRunning = false; // // prevent this code being run more then once
    checkOnline();
  }
  // check if scan delay has timed out after Xsec == configuration.scanRate
  if (delayRunning && ((millis() - delayStart) >= configuration.scanRate))
  {
    delayStart += configuration.scanRate; 
    //delayRunning = false; // // prevent this code being run more then once
    scanSensors();
  }
 
  server->handleClient();
 
  processResponse(receiveFromMesh());

}
void restart()
{
  Serial.println("Restart called");
  delete server;
  ESP.restart();
}
void pingCheck()
{
    //Serial.print("Pinging Gateway:");
    IPAddress gateway;
    gateway.fromString(configuration.gateway);
    //Serial.println(gateway);
    //digitalWrite(networkLed,LOW);
    // Ping
    if (Ping.ping(gateway))
    {
      //Serial.println("Gateway connected");
      //digitalWrite(networkLed,HIGH);
    } 
    else 
    {
      //Serial.println("Error connecting to Gateway");
      //digitalWrite(networkLed,LOW);
    }
}
