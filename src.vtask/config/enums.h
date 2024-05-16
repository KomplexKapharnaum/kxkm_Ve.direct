#ifndef ENUMS
#define ENUMS

#include <Arduino.h>

// The state in which the device can be. This mainly affects what
// is drawn on the display.
enum DEVICE_STATE
{
  CONNECTING_WIFI,
#if AWS_ENABLED == true
  CONNECTING_AWS,
#endif
#if NTP_TIME_SYNC_ENABLED == true
  FETCHING_TIME,
#endif
#if HA_ENABLED == true
  CONNECTING_HA,
#endif
#if INFLUXDB_ENABLED == true
  CONNECTING_INFLUXDB,
#endif
  UP,
};

// Place to store all the variables that need to be displayed.
// All other functions should update these!
struct DisplayValues
{
  #if CT_ENABLE == true
  float Amps;
  float Watts;
  #endif
  #if VeDirect_ENABLE == true
  float Battery_Voltage;
  float Battery_Percentage;
  float Battery_Current;
  float PV_Voltage;
  float PV_Power;
  float Operation_State;
  float Tracker_Mode;
  float Off_Reason;
  float Error_Code;
  float Load_Status;
  double Load_Current;
  float Yeld_Total;
  float Yeld_Today;
  float Max_Power_Today;
  float Yeld_Yesterday;
  float Max_Power_Yesterday;
  #endif
  bool wifi_connected;
  int8_t wifi_strength;
  DEVICE_STATE currentState;
  String time;
};

#if DEBUG == true
#define serial_print(x) Serial.print(x)
#define serial_println(x) Serial.println(x)
#else
#define serial_print(x)
#define serial_println(x)
#endif

#endif