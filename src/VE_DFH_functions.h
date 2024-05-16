// Send data to InfluxDB
void PrintData()
{
  for (int i = 0; i < myve.veEnd; i++)
  {
    parseSerialData(myve.veName[i], myve.veValue[i]);
  }
  point.addField("time elapsed", millis() / 1000); // Add time elapsed in seconde since last boot
}

// Read data from VE device
void ReadVEData()
{
  while (SERIAL_PORT.available())
  {
    myve.rxData(SERIAL_PORT.read());
  }
  // yield();
}

// Print data every time
void Every_time(int time)
{
  ReadVEData();
  static unsigned long prev_millis;
  static int count = -UPDATE_TIME_LCD;
  static bool data_changed = false;

  if (millis() - prev_millis >= count) // print every second
  {
    for (int i = 0; i < myve.veEnd; i++)
    {
      parseLCDData(myve.veName[i], myve.veValue[i]);
    }
    lcd_clear();
    count += UPDATE_TIME_LCD;
  }
  if (millis() - prev_millis > time) // print every time
  {
    PrintData();
    send_to_influxdb();
    count = 0;
    prev_millis = millis();
  }
  delay(10);
}

// hex frame callback function
void HexCallback(const char *buffer, int size, void *data)
{
  char tmp[100];
  memcpy(tmp, buffer, size * sizeof(char));
  tmp[size] = 0;
  // Serial.print("received hex frame: ");
  // Serial.println(tmp);
}

// log helper
void LogHelper(const char *module, const char *error)
{
  Serial.print(module);
  Serial.print(":");
  Serial.println(error);
}