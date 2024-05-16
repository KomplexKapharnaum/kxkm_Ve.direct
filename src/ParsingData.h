/*
  © MK 15 Jan 2024
  Write Victron Energy MPPT fields to INfluxDB on Cloud
  based on ESP32 Wroom using Uses VeDirectFrameHandler library
*/

// Parsing MPPT serial data and InfluxDB Cloud writing

void parseSerialData(String field, String value)
{

  // Serial.print(field);  Serial.println("-"+value); // DEBUG ONLY

  // Main or channel 1 (battery) voltage (mV)
  if (field.equals("V"))
  {
    float voltage = value.toFloat() / 1000;
    point.addField("Battery_voltage", voltage);
    data_value[0] = voltage;
  }

  // Main or channel 1 battery current (mA)
  else if (field.equals("I"))
  {
    float current = value.toFloat() / 1000;
    point.addField("Battery_current", current);
    data_value[1] = current;
  }

  // Panel voltage (mV)
  else if (field.equals("VPV"))
  {
    float vpv = value.toFloat() / 1000;
    point.addField("PV_voltage", vpv);
    data_value[2] = vpv;
  }

  // Panel Power (W)
  else if (field.equals("PPV"))
  {
    float ppv = value.toFloat();
    point.addField("PV_power", ppv);
    data_value[3] = ppv;
  }

  // State of Operation
  else if (field.equals("CS"))
  {
    float state = value.toFloat();
    point.addField("Operation_state", state);
    data_value[4] = state;
  }

  // Tracker operation mode
  else if (field.equals("MPPT"))
  {
    float mppt = value.toFloat();
    point.addField("Tracker_mode", mppt);
    data_value[5] = mppt;
  }

  // Off reason
  else if (field.equals("OR"))
  {
    float off = value.toFloat();
    point.addField("Off_reason", off);
    data_value[6] = off;
  }

  // Error code
  else if (field.equals("ERR"))
  {
    float err = value.toFloat();
    point.addField("Error_code", err);
    data_value[7] = err;
  }

  // Load output state (ON/OFF)
  else if (field.equals("LOAD"))
  {
    data_value[8] = value.toFloat();
    if (value == "OFF")
    {
      point.addField("Load_status", 0);
    }
    else if (value == "ON")
    {
      point.addField("Load_status", 1);
    }
  }

  // Load current (mA)
  else if (field.equals("IL"))
  {
    float il = value.toFloat() / 1000;
    point.addField("Load_current", il);
    data_value[9] = il;
  }

  // Yield total (0.01 kWh)
  else if (field.equals("H19"))
  {
    float yeld = value.toFloat() / 1000;
    point.addField("Yeld_total", yeld);
    data_value[10] = yeld;
  }

  // Yield today (0.01 kWh)
  else if (field.equals("H20"))
  {
    float yeld = value.toFloat() / 1000;
    point.addField("Yeld_today", yeld);
    data_value[11] = yeld;
  }

  // Maximum power today (W)
  else if (field.equals("H21"))
  {
    float power = value.toFloat();
    point.addField("Max_pw_today", power);
    data_value[12] = power;
  }

  // Yield yesterday (0.01 kWh)
  else if (field.equals("H22"))
  {
    float yeld = value.toFloat() / 1000;
    point.addField("Yeld_yesterday", yeld);
    data_value[13] = yeld;
  }

  // Maximum power yesterday (W)
  else if (field.equals("H23"))
  {
    float power = value.toFloat();
    point.addField("Max_pw_yesterday", power);
    data_value[14] = power;
  }

  else if (field.equals("HSDS"))
  {
  }
}

void parseLCDData(String field, String value)
{

  // Serial.print(field);  Serial.println("-"+value); // DEBUG ONLY

  // Main or channel 1 (battery) voltage (mV)
  if (field.equals("V"))
  {
    float voltage = value.toFloat() / 1000;
    data_value[0] = voltage;
  }

  // Main or channel 1 battery current (mA)
  else if (field.equals("I"))
  {
    float current = value.toFloat() / 1000;
    data_value[1] = current;
  }

  // Panel voltage (mV)
  else if (field.equals("VPV"))
  {
    float vpv = value.toFloat() / 1000;
    data_value[2] = vpv;
  }

  // Panel Power (W)
  else if (field.equals("PPV"))
  {
    float ppv = value.toFloat();
    data_value[3] = ppv;
  }

  // State of Operation
  else if (field.equals("CS"))
  {
    float state = value.toFloat();
    data_value[4] = state;
  }

  // Tracker operation mode
  else if (field.equals("MPPT"))
  {
    float mppt = value.toFloat();
    data_value[5] = mppt;
  }

  // Off reason
  else if (field.equals("OR"))
  {
    float off = value.toFloat();
    data_value[6] = off;
  }

  // Error code
  else if (field.equals("ERR"))
  {
    float err = value.toFloat();
    data_value[7] = err;
  }

  // Load output state (ON/OFF)
  else if (field.equals("LOAD"))
  {
    data_value[8] = value.toFloat();
  }

  // Load current (mA)
  else if (field.equals("IL"))
  {
    data_value[9] = value.toFloat() / 1000;
  }

  // Yield total (0.01 kWh)
  else if (field.equals("H19"))
  {
    data_value[10] = value.toFloat() / 1000;
  }

  // Yield today (0.01 kWh)
  else if (field.equals("H20"))
  {
    data_value[11] = value.toFloat() / 1000;
  }

  // Maximum power today (W)
  else if (field.equals("H21"))
  {
    data_value[12] = value.toFloat();
  }

  // Yield yesterday (0.01 kWh)
  else if (field.equals("H22"))
  {
    data_value[13] = value.toFloat() / 1000;
  }

  // Maximum power yesterday (W)
  else if (field.equals("H23"))
  {
    data_value[14] = value.toFloat();
  }

  else if (field.equals("HSDS"))
  {
  }
}

void send_to_influxdb()
{
  // Report RSSI of currently connected network
  int rssi = WiFi.RSSI();
  point.addField("rssi", rssi);
  data_value[17] = rssi;
  // InfluxDB Cloud flush on last field received
  client.writePoint(point);

  // End of the iteration - force write of all the values into InfluxDB as single transaction
  Serial.print("Flushing data into InfluxDB Nb. : ");
  Serial.print(rec_count);
  if (!client.flushBuffer())
  {
    Serial.println(" => failed");
    Serial.print("InfluxDB flush failed: ");
    Serial.println(client.getLastErrorMessage());
    Serial.print("Full buffer: ");
    Serial.println(client.isBufferFull() ? "Yes" : "No");
  }
  else
  {
    Serial.println(" => flush OK");
    client.resetBuffer();
  }
    // Clear fields for next usage. Tags remain the same.
    point.clearFields();
    
  rec_count++;
  check_count++;
  if (check_count == CHECK_READ_DB_COUNT)
  {
    check_influx_db();
    // esp_restart();
    check_count = 0;
  }
}
