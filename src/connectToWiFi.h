/*
  © MK 15 Jan 2024
  Write Victron Energy MPPT fields to INfluxDB on Cloud
  based on ESP32 Wroom using Uses VeDirectFrameHandler library
*/

// Setup wifi
/**
 * Task: monitor the WiFi connection and keep it alive!
 * If the connection is lost, the ESP32 will attempt to reconnect.
 */

void connectToWiFi()
{
  unsigned long wifi_time = millis();
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED && millis() - WIFI_TIMEOUT * 1000 >= wifi_time)
  {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connection failed!");
    Serial.println("wait 5 minutes and restart...");
    delay(30000);
    esp_restart();
  }
  Serial.println("connected");
  // print your WiFi shield's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void connect_to_influxdb()
{
  // Check server connection
  if (client.validateConnection())
  {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
    data_value[16] = 1;
  }
  else
  {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
    data_value[16] = 0;
  }
  // Enable messages batching and retry buffer
  client.setWriteOptions(WriteOptions().writePrecision(WRITE_PRECISION).batchSize(MAX_BATCH_SIZE).bufferSize(WRITE_BUFFER_SIZE).flushInterval(10).retryInterval(1).maxRetryAttempts(5));
  client.setHTTPOptions(HTTPOptions().connectionReuse(true).httpReadTimeout(2000));
}

void wifi_status_check()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WIFI - CONNECTION LOST...");
    data_value[15] = 0;
    delay(1000);
    connectToWiFi();
  }
  else if (data_value[15] == 0)
  {
    Serial.println("WIFI - CONNECTED...");
    // Accurate time is necessary for certificate validation and writing in batches
    // Syncing progress and the time will be printed to Serial.
    timeSync(TZ_INFO, NTP_SERVER1, NTP_SERVER2);
    connect_to_influxdb();
    data_value[15] = 1;
  }
}

void check_influx_db()
{
  // Prepare query parameters
  QueryParams params;
  String query = "from(bucket: \"victron\") \
    |> range(start: -5m) \
    |> filter(fn: (r) => \
      r._measurement == \"SmartSolar\" and \
      r._field == \"rssi\" \
    )";
  // Send query to the server and get result
  FluxQueryResult result = client.query(query, params);
  // Iterate over rows. Even there is just one row, next() must be called at least once.
  int c = 0;
  while (result.next() && c < 1)
  {
    Serial.println("Querying datanase OK : ");
    data_value[16] == 1;
    CHECK_READ_DB_COUNT = UPDATE_CHK_INFLUXDB;
    c++;
  }
  if (!c)
  {
    CHECK_READ_DB_COUNT = 5;
    Serial.println(" No data found");
    data_value[16] == 0;
    error_data_received++;
    if (error_data_received > 20)
    {
      esp_restart();
    }
    else if (error_data_received > 10)
    {
      connect_to_influxdb();
    }
    // Check if there was an error
    if (result.getError() != "")
    {
      Serial.print("Query result error: ");
      Serial.println(result.getError());
    }
    // Close the result
    result.close();
    delay(1000);
  }
}