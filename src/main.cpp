/**
 * @file main.cpp
 * @brief Écrire les champs Victron Energy MPPT dans InfluxDB sur le Cloud en utilisant M5 Stack avec la bibliothèque VeDirectFrameHandler.
 * @details Cet exemple et cette bibliothèque ont été testés sur une installation solaire/plomb autonome et monitorée à distance.
 * @version 0.3
 * @date 2024-01-15
 * @see https://github.com/mk4001/VE.Direct2InfluxDB-2.0
 * @project Artiste à l'école de la compagnie Komplex Kapharnaum
 */

/*************************************************************************************/

/**
 * @brief Define Serial port #2 PIN 16(RX) - 17(TX - not used) for VE.Direct device
 */
#define SERIAL_PORT Serial2

/**
 * @brief Update time for values suitable for your network.
 */
#define UPDATE_TIME 5000
#define UPDATE_TIME_LCD 1000
#define UPDATE_CHK_INFLUXDB 100

#define MAX_LCD_MENU 3

/**
 * @brief WiFi AP SSID and password
 */
#define WIFI_SSID "AtelierCreationItinerant"
#define WIFI_PASSWORD "jeVeuxDuWifi!"
#define WIFI_TIMEOUT 5 // 10 seconds
#define DEVICE_NAME "Victron-MPPT"

/**
 * @brief InfluxDB v2 server details
 */
#define INFLUXDB_URL "https://us-east-1-1.aws.cloud2.influxdata.com"
#define INFLUXDB_TOKEN "QTIEjAI8p9zsaxTnBYExPLyJFJNNGrx-Dz9DU0cTa6tInggXDHX77APTGsoZ7-rXgK8JqRZw6ptSdDJaoZtL8A=="
#define INFLUXDB_ORG "650b91508ddb9cf3"
#define INFLUXDB_BUCKET "victron"
#define DEVICE "SmartSolar"
#define WRITE_PRECISION WritePrecision::MS
#define MAX_BATCH_SIZE 20
#define WRITE_BUFFER_SIZE 50

/**
 * @brief Set timezone string according to https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
 */
#define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"

/**
 * @brief NTP servers for time synchronization.
 */
#define NTP_SERVER1 "pool.ntp.org"
#define NTP_SERVER2 "time.nis.gov"
#define TIME_SYNC_INTERVAL 5 // Time sync interval in minutes

unsigned long sync_time = 0; // Sync time for batching InfluxDB
static long rec_count = 0;   // Record count for InfluxDB
static int check_count = 0;  // Check count for InfluxDB
static int CHECK_READ_DB_COUNT = UPDATE_CHK_INFLUXDB;
static int lcd_menu = 1; // Menu for LCD

static float data_value[20] = {};                                                               // Array to store data values
static float data_value_old[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Array to store data values
static int error_data_received = 0;                                                             // Error counter for data received in query InfluxDB

#include "Arduino.h"
#include <M5Stack.h>
#include <WiFiMulti.h>
WiFiMulti wifiMulti;

// InfluxDB Client
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
// InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);

// Declare Data point
Point point(DEVICE);

// Number for loops to sync time using NTP
int iterations = 0;

#include <lcd.h>
#include <connectToWiFi.h>

// VE.Direct Frame Handler
#include "VeDirectFrameHandler.h"
VeDirectFrameHandler myve;

// Callback function for hex frame
#include <ParsingData.h>
#include <VE_DFH_functions.h>

/**
 * @brief Setup function
 */
void setup()
{
  delay(2000);
  Serial.begin(115200); // output serial port

  SERIAL_PORT.begin(19200); // input serial port (VE device)
  SERIAL_PORT.flush();
  Serial.println("DEBUG-setup");

  // log helper
  myve.setErrorHandler(&LogHelper);
  // hex protocol callback
  myve.addHexCallback(&HexCallback, (void *)42);

  // Alternatively, set insecure connection to skip server certificate validation
  // save space in flash (and RAM) by not including the certificate
  client.setInsecure();

  lcd_start();
  connectToWiFi();
  wifi_status_check();

  point.addTag("device", "mppt_victron");
  sync_time = millis();
}

/**
 * @brief Main loop function
 */
void loop()
{
  M5.update();
  if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200))
  {
    if (lcd_menu > 1)
      lcd_menu--;
    else
      lcd_menu = MAX_LCD_MENU;
    lcd_clear();
  }
  else if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200))
  {
    lcd_menu = 1;
    lcd_clear();
  }
  else if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(1000, 200))
  {
    if (lcd_menu < MAX_LCD_MENU)
      lcd_menu++;
    else
      lcd_menu = 1;
    lcd_clear();
  }

  // Sync time for batching once per hour for more accurate time
  wifi_status_check();
  if (millis() - sync_time >= (TIME_SYNC_INTERVAL * 60 * 1000))
  {
    timeSync(TZ_INFO, NTP_SERVER1, NTP_SERVER2);
    sync_time = millis();
  }
  Every_time(UPDATE_TIME);
}
