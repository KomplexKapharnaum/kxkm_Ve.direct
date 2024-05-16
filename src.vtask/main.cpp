#include <Arduino.h>
#include <M5Stack.h>
#include "EmonLib.h"
#include "VeDirectFrameHandler.h"


#include "WiFi.h"

#include "config/config.h"
#include "config/enums.h"

#include "tasks/updateDisplay.h"
#include "tasks/fetch-time-from-ntp.h"
#include "tasks/mqtt-aws.h"
#include "tasks/wifi-connection.h"
#include "tasks/wifi-update-signalstrength.h"
#include "tasks/measure-electricity.h"
#include "tasks/mqtt-home-assistant.h"
#include "tasks/influxdb.h"

DisplayValues gDisplayValues;

EnergyMonitor emon1;

VeDirectFrameHandler myve;

#include <VE_DFH_functions.h>

// Place to store local measurements before sending them off to AWS
unsigned short measurements[LOCAL_MEASUREMENTS];
unsigned char measureIndex = 0;

void setup()
{
#if DEBUG == true
  Serial.begin(115200);
#endif

  // Initialize the M5 display
  M5.begin();
  M5.Lcd.begin();
  // Init the display
  M5.Lcd.clearDisplay();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextWrap(false);

#if CT_ENABLE == true
  // Initialize emon library
  emon1.current(ADC_INPUT, 30);
#endif
#if VeDirect_ENABLE == true
  Serial2.begin(19200);
  Serial2.flush();
  // log helper
  myve.setErrorHandler(&LogHelper);
  // hex protocol callback
  myve.addHexCallback(&HexCallback, (void *)42);

#endif
  // ----------------------------------------------------------------
  // TASK: Connect to WiFi & keep the connection alive.
  // ----------------------------------------------------------------
  xTaskCreatePinnedToCore(
      keepWiFiAlive,
      "keepWiFiAlive", // Task name
      5000,            // Stack size (bytes)
      NULL,            // Parameter
      1,               // Task priority
      NULL,            // Task handle
      ARDUINO_RUNNING_CORE);

// ----------------------------------------------------------------
// TASK: Connect to AWS & keep the connection alive.
// ----------------------------------------------------------------
#if AWS_ENABLED == true
  xTaskCreate(
      keepAWSConnectionAlive,
      "MQTT-AWS", // Task name
      5000,       // Stack size (bytes)
      NULL,       // Parameter
      5,          // Task priority
      NULL        // Task handle
  );
#endif
// ----------------------------------------------------------------
// TASK: Connect to Home Assistant & keep the connection alive.
// ----------------------------------------------------------------
#if HA_ENABLED == true
  xTaskCreate(
      keepHAConnectionAlive,
      "MQTT-HA Connect",
      5000,
      NULL,
      4,
      NULL);
#endif

// ----------------------------------------------------------------
// TASK: Connect to InfluxDB & keep the connection alive.
// ----------------------------------------------------------------
#if INFLUXDB_ENABLED == true
  xTaskCreate(
      keepInfluxDBConnectionAlive,
      "InfluxDB Connection",
      5000,
      NULL,
      4,
      NULL);
#endif

  // ----------------------------------------------------------------
  // TASK: Update the display every second
  //       This is pinned to the same core as Arduino
  //       because it would otherwise corrupt the OLED
  // ----------------------------------------------------------------
  xTaskCreatePinnedToCore(
      updateDisplay,
      "UpdateDisplay", // Task name
      10000,           // Stack size (bytes)
      NULL,            // Parameter
      3,               // Task priority
      NULL,            // Task handle
      ARDUINO_RUNNING_CORE);

  // ----------------------------------------------------------------
  // Task: measure electricity consumption ;)
  // ----------------------------------------------------------------
  xTaskCreate(
      measureElectricity,
      "Measure electricity", // Task name
      5000,                  // Stack size (bytes)
      NULL,                  // Parameter
      4,                     // Task priority
      NULL                   // Task handle
  );

// ----------------------------------------------------------------
// TASK: update time from NTP server.
// ----------------------------------------------------------------
#if NTP_TIME_SYNC_ENABLED == true
  xTaskCreate(
      fetchTimeFromNTP,
      "Update NTP time",
      5000, // Stack size (bytes)
      NULL, // Parameter
      1,    // Task priority
      NULL  // Task handle
  );
#endif

  // ----------------------------------------------------------------
  // TASK: update WiFi signal strength
  // ----------------------------------------------------------------
  xTaskCreate(
      updateWiFiSignalStrength,
      "Update WiFi strength",
      1000, // Stack size (bytes)
      NULL, // Parameter
      2,    // Task priority
      NULL  // Task handle
  );

#if HA_ENABLED == true
  xTaskCreate(
      HADiscovery,
      "MQTT-HA Discovery", // Task name
      5000,                // Stack size (bytes)
      NULL,                // Parameter
      5,                   // Task priority
      NULL                 // Task handle
  );

  xTaskCreate(
      keepHAConnectionAlive,
      "MQTT-HA Connect",
      5000,
      NULL,
      4,
      NULL);
#endif
}

void loop()
{
  vTaskDelay(10000 / portTICK_PERIOD_MS);
  for (int i = 0; i < myve.veEnd; i++)
  {
    if (myve.veName[i] == ("V"))
    {
      float Battery_Voltage = myve.veValue[i].toFloat() / 1000;
    }
  }
}