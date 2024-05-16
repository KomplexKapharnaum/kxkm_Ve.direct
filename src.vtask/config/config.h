#ifndef CONFIG
#define CONFIG

/**
 * Set this to false to disable Serial logging
 */
#define DEBUG true

/**
 * The name of this device (as defined in the AWS IOT console).
 * Also used to set the hostname on the network
 */
#define DEVICE_NAME "*****YOUR AWS IOT DEVICE NAME******"

/**
 * ADC input pin that is used to read out the CT sensor
 */
#define ADC_INPUT 36

/**
 * The voltage of your home, used to calculate the wattage.
 * Try setting this as accurately as possible.
 */
#define HOME_VOLTAGE 245.0

/**
 * The voltage of your home, used to calculate the wattage.
 * Try setting this as accurately as possible.
 */
#define BATTERY_VOLTAGE 24.0

/**
 * WiFi credentials
 */
#define WIFI_NETWORK "La-clic"
#define WIFI_PASSWORD "clic clac"

/**
 * Timeout for the WiFi connection. When this is reached,
 * the ESP goes into deep sleep for 30seconds to try and
 * recover.
 */
#define WIFI_TIMEOUT 20000 // 20 seconds

/**
 * How long should we wait after a failed WiFi connection
 * before trying to set one up again.
 */
#define WIFI_RECOVER_TIME_MS 20000 // 20 seconds

/**
 * Dimensions of the OLED display attached to the ESP
 */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

/**
 * Force Emonlib to assume a 3.3V supply to the CT sensor
 */
#define emonTxV3 1

/**
 * define the type of sensor in use
 */
#define CT_ENABLE false
#define VeDirect_ENABLE true

/**
 * Local measurements
 */
#define LOCAL_MEASUREMENTS 30


/**
 * The MQTT endpoint of the service we should connect to and receive messages
 * from.
 */
#define AWS_ENABLED false
#define AWS_IOT_ENDPOINT "**** YOUR AWS IOT ENDPOINT ****"
#define AWS_IOT_TOPIC "**** YOUR AWS IOT RULE ARN ****"

#define MQTT_CONNECT_DELAY 200
#define MQTT_CONNECT_TIMEOUT 20000 // 20 seconds


/**
 * Syncing time with an NTP server
 */
#define NTP_TIME_SYNC_ENABLED true
#define NTP_SERVER "pool.ntp.org"
#define NTP_OFFSET_SECONDS 3600
#define NTP_UPDATE_INTERVAL_MS 60000
#define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"

/**
 * Wether or not you want to enable Home Assistant integration
 */
#define HA_ENABLED false
#define HA_ADDRESS "*** YOUR HOME ASSISTANT IP ADDRESSS ***"
#define HA_PORT 8883
#define HA_USER "*** MQTT USER ***"
#define HA_PASSWORD "*** MQTT PASSWORD ***"

/**
 * InfluxDB configuration
 */
#define INFLUXDB_ENABLED true
// InfluxDB v2 server or cloud API authentication token (Use: InfluxDB UI -> Load Data -> Tokens -> <select token>)
// InfluxDB v2 server url, e.g. https://eu-central-1-1.aws.cloud2.influxdata.com (Use: InfluxDB UI -> Load Data -> Client Libraries)
#define INFLUXDB_URL "https://us-east-1-1.aws.cloud2.influxdata.com"
// InfluxDB v2 server or cloud API authentication token (Use: InfluxDB UI -> Load Data -> Tokens -> <select token>)
#define INFLUXDB_TOKEN "QTIEjAI8p9zsaxTnBYExPLyJFJNNGrx-Dz9DU0cTa6tInggXDHX77APTGsoZ7-rXgK8JqRZw6ptSdDJaoZtL8A=="
// InfluxDB v2 organization id (Use: InfluxDB UI -> Settings -> Profile -> <name under tile> )
#define INFLUXDB_ORG "650b91508ddb9cf3"
// InfluxDB v2 bucket name (Use: InfluxDB UI -> Load Data -> Buckets)
#define INFLUXDB_BUCKET "victron"

// Check which core Arduino is running on. This is done because updating the 
// display only works from the Arduino core.
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#endif