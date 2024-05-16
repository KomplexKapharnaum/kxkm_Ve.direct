#ifndef TASK_MEASURE_ELECTRICITY
#define TASK_MEASURE_ELECTRICITY

#include <Arduino.h>
#include "EmonLib.h"
#include "VeDirectFrameHandler.h"

#include "../config/config.h"
#include "../config/enums.h"
#include "mqtt-aws.h"
#include "mqtt-home-assistant.h"
#include "influxdb.h"

extern DisplayValues gDisplayValues;
extern EnergyMonitor emon1;
extern VeDirectFrameHandler myve;

#include <VE_DFH_functions.h>

extern unsigned short measurements[];
extern unsigned char measureIndex;

void measureElectricity(void *parameter)
{
  for (;;)
  {
    serial_println("[ENERGY] Measuring...");
    long start = millis();
#if CT_ENABLE == true
    double amps = emon1.calcIrms(1480);
    double watts = amps * HOME_VOLTAGE;
    gDisplayValues.amps = amps;
    gDisplayValues.watt = watts;
    measurements[measureIndex] = watts;
    measureIndex++;
#endif

#if VeDirect_ENABLE == true
    while (Serial2.available())
    {
      myve.rxData(Serial2.read());
    }
    yield();
  
    for (int i = 0; i < myve.veEnd; i++)
    {
      if (myve.veName[i] == ("V"))
      {
        float Battery_Voltage = myve.veValue[i].toFloat() / 1000;
      }
      else if (myve.veName[i] == ("I"))
      {
        float Battery_Current = myve.veValue[i].toFloat() / 1000;
      }
      else if (myve.veName[i] == ("VPV"))
      {
        float PV_Voltage = myve.veValue[i].toFloat() / 1000;
      }
      else if (myve.veName[i] == ("PPV"))
      {
        float PV_Power = myve.veValue[i].toFloat();
      }
      else if (myve.veName[i] == ("CS"))
      {
        float Operation_State = myve.veValue[i].toFloat();
      }
      else if (myve.veName[i] == ("MPPT"))
      {
        float Tracker_Mode = myve.veValue[i].toFloat();
      }
      else if (myve.veName[i] == ("OR"))
      {
        float Off_Reason = myve.veValue[i].toFloat();
      }
      else if (myve.veName[i] == ("ERR"))
      {
        float Error_Code = myve.veValue[i].toFloat();
      }
      else if (myve.veName[i] == ("LOAD"))
      {
        if (myve.veValue[i] == "OFF")
        {
          bool Load_Status = 0;
        }
        else if (myve.veValue[i] == "ON")
        {
          bool Load_Status = 1;
        }
      }
      else if (myve.veName[i] == ("IL"))
      {
        float Load_Current = myve.veValue[i].toFloat() / 1000;
      }
      else if (myve.veName[i] == ("H19"))
      {
        float Yeld_Total = myve.veValue[i].toFloat() / 1000;
      }
      else if (myve.veName[i] == ("H20"))
      {
        float Yeld_Today = myve.veValue[i].toFloat() / 1000;
      }
      else if (myve.veName[i] == ("H21"))
      {
        float Max_pw_Today = myve.veValue[i].toFloat();
      }
      else if (myve.veName[i] == ("H22"))
      {
        float Yeld_Yesterday = myve.veValue[i].toFloat() / 1000
      }
    }
    
#endif
  }

  measurements[measureIndex] = batteryPower;
  measureIndex++;

  if (measureIndex == LOCAL_MEASUREMENTS)
  {
#if AWS_ENABLED == true
    xTaskCreate(
        uploadMeasurementsToAWS,
        "Upload measurements to AWS",
        10000, // Stack size (bytes)
        NULL,  // Parameter
        5,     // Task priority
        NULL   // Task handle
    );
#endif

#if HA_ENABLED == true
    xTaskCreate(
        sendEnergyToHA,
        "HA-MQTT Upload",
        10000, // Stack size (bytes)
        NULL,  // Parameter
        5,     // Task priority
        NULL   // Task handle
    );
#endif

#if INFLUXDB_ENABLED == true
    xTaskCreate(
        sendEnergyToInfluxDB,
        "InfluxDB Upload",
        10000, // Stack size (bytes)
        NULL,  // Parameter
        5,     // Task priority
        NULL   // Task handle
    );
#endif
    measureIndex = 0;
  }

  long end = millis();

  // Schedule the task to run again in 1 second (while
  // taking into account how long measurement took)
  vTaskDelay((1000 - (end - start)) / portTICK_PERIOD_MS);
}

#endif
