#ifndef DRAW_FUNCTIONS
#define DRAW_FUNCTIONS

#include <WiFi.h>
#include "../config/enums.h"
#include "../config/config.h"

extern DisplayValues gDisplayValues;
extern unsigned char measureIndex;

void drawTime()
{
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print(gDisplayValues.time);
}

void drawSignalStrength()
{
  const byte X = 51;
  const byte X_SPACING = 2;

  // Draw the four base rectangles
  M5.Lcd.fillRect(X, 8 - 2, 1, 2, WHITE);                 // Bar 1
  M5.Lcd.fillRect(X + X_SPACING, 8 - 2, 1, 2, WHITE);     // Bar 2
  M5.Lcd.fillRect(X + X_SPACING * 2, 8 - 2, 1, 2, WHITE); // Bar 3
  M5.Lcd.fillRect(X + X_SPACING * 3, 8 - 2, 1, 2, WHITE); // Bar 4

  // Draw bar 2
  if (gDisplayValues.wifi_strength > -70)
  {
    M5.Lcd.fillRect(X + X_SPACING, 8 - 4, 1, 4, WHITE);
  }

  // Draw bar 3
  if (gDisplayValues.wifi_strength > -60)
  {
    M5.Lcd.fillRect(X + X_SPACING * 2, 8 - 6, 1, 6, WHITE);
  }

  // Draw bar 4
  if (gDisplayValues.wifi_strength >= -50)
  {
    M5.Lcd.fillRect(X + X_SPACING * 3, 8 - 8, 1, 8, WHITE);
  }
}

void drawMeasurementProgress()
{
  const byte Y = SCREEN_WIDTH - 20;
  M5.Lcd.drawRect(0, Y, measureIndex * 2, 2, WHITE);
}

/**
 * The screen that is displayed when the ESP has just booted
 * and is connecting to WiFi & AWS.
 */
void drawBootscreen()
{
  byte X = 14;
  byte Y = 70;
  byte WIDTH = 6;
  byte MAX_HEIGHT = 35;
  byte HEIGHT_STEP = 10;
  byte X_SPACING = 10;

  M5.Lcd.fillRect(X, Y, WIDTH, MAX_HEIGHT - HEIGHT_STEP * 3, WHITE);
  M5.Lcd.fillRect(X + X_SPACING, Y - HEIGHT_STEP, WIDTH, MAX_HEIGHT - HEIGHT_STEP * 2, WHITE);
  M5.Lcd.fillRect(X + X_SPACING * 2, Y - HEIGHT_STEP * 2, WIDTH, MAX_HEIGHT - HEIGHT_STEP, WHITE);
  M5.Lcd.fillRect(X + X_SPACING * 3, Y - HEIGHT_STEP * 3, WIDTH, MAX_HEIGHT, WHITE);

  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, Y + MAX_HEIGHT / 2);
  M5.Lcd.println("Connecting");

  if (gDisplayValues.currentState == CONNECTING_WIFI)
  {
    M5.Lcd.println("   WiFi");
  }
#if AWS_ENABLED == true
  if (gDisplayValues.currentState == CONNECTING_AWS)
  {
    M5.Lcd.println("   AWS");
  }
#endif
#if HA_ENABLED == true
  if (gDisplayValues.currentState == CONNECTING_HA)
  {
    M5.Lcd.println("   HA");
  }
#endif
#if INFLUXDB_ENABLED == true
  if (gDisplayValues.currentState == CONNECTING_INFLUXDB)
  {
    M5.Lcd.println("   InfluxDB");
  }
#endif
#if NTP_TIME_SYNC_ENABLED == true
  if (gDisplayValues.currentState == FETCHING_TIME)
  {
    M5.Lcd.println("   Time");
  }
#endif
}

/**
 * Draw the current amps & watts in the middle of the M5.Lcd.
 */
void drawAmpsWatts()
{
#if CT_ENABLE == true
  String watts = String(gDisplayValues.watt, 0);
  String amps = String(gDisplayValues.amps, 2);

  String lblWatts = "Watt";
  String lblAmps = "Amps";

  const int startY = 30;

  // Calculate how wide (pixels) the text will be once rendered.
  // Each character = 6 pixels, with font size 2, that is 12 pixels.
  // -1 because of the spacing between letters (last one doesn't)
  int widthAmps = (amps.length() * 12) - 1;
  int widthLblAmps = lblAmps.length() * 6 - 1;

  int widthWatts = watts.length() * 12 - 1;
  int widthLblWatts = lblWatts.length() * 6 - 1;

  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor((SCREEN_HEIGHT - widthAmps) / 2, startY);
  M5.Lcd.print(amps);

  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor((SCREEN_HEIGHT - widthLblAmps) / 2, startY + 15);
  M5.Lcd.print(lblAmps);

  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor((SCREEN_HEIGHT - widthWatts) / 2, startY + 40);
  M5.Lcd.print(watts);

  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor((SCREEN_HEIGHT - widthLblWatts) / 2, startY + 60);
  M5.Lcd.print(lblWatts);
#endif
#if VeDirect_ENABLE == true
  String Battery_Voltage = String(gDisplayValues.Battery_Voltage, 0);
  String Battery_Percentage = String(gDisplayValues.Battery_Percentage, 0);
  String Battery_Current = String(gDisplayValues.Battery_Current, 2);
  String PV_Voltage = String(gDisplayValues.PV_Voltage, 0);
  String PV_Power = String(gDisplayValues.PV_Power, 0);
  String Operation_State = String(gDisplayValues.Operation_State, 0);
  String Tracker_Mode = String(gDisplayValues.Tracker_Mode, 0);
  String Off_Reason = String(gDisplayValues.Off_Reason, 0);
  String Error_Code = String(gDisplayValues.Error_Code, 0);
  String Load_Status = String(gDisplayValues.Load_Status, 0);
  String Load_Current = String(gDisplayValues.Load_Current, 2);
  String Yeld_Total = String(gDisplayValues.Yeld_Total, 0);
  String Yeld_Today = String(gDisplayValues.Yeld_Today, 0);
  String Max_Power_Today = String(gDisplayValues.Max_Power_Today, 0);
  String Yeld_Yesterday = String(gDisplayValues.Yeld_Yesterday, 0);
  String Max_Power_Yesterday = String(gDisplayValues.Max_Power_Yesterday, 0);

  String lblBattery_Voltage = "Battery Voltage";
  String lblBattery_Percentage = "Battery Percentage";
  String lblBattery_Current = "Battery Current";
  String lblPV_Voltage = "PV Voltage";
  String lblPV_Power = "PV Power";
  String lblOperation_State = "Operation State";
  String lblTracker_Mode = "Tracker Mode";
  String lblOff_Reason = "Off Reason";
  String lblError_Code = "Error Code";
  String lblLoad_Status = "Load Status";
  String lblLoad_Current = "Load Current";
  String lblYeld_Total = "Yeld Total";
  String lblYeld_Today = "Yeld Today";
  String lblMax_Power_Today = "Max Power Today";
  String lblYeld_Yesterday = "Yeld Yesterday";
  String lblMax_Power_Yesterday = "Max Power Yesterday";
#endif
}

#endif