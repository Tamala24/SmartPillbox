/**
 * @file  d_wifi_lib.ino
 * @brief File containing functions for WiFi connection management
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2024 Andrea Nieto Gil. All rights reserved.
 */

#define WIFI_CONNECTION_TIMEOUT_SECONDS 15  ///< Timeout for WiFi connection attempts in seconds

// Use TLS/SSL communications if the root CA certificate is defined
#ifdef SSL_ROOT_CA
  WiFiClientSecure espWifiClient; ///< WiFi client for secure connections
#else
  WiFiClient espWifiClient; ///< WiFi client for non-secure connections
#endif

const char* wifiSSID = NET_SSID; ///< WiFi network SSID
const char* wifiPasswd = NET_PASSWD; ///< WiFi network password

/**
 * @brief Function to manage the WiFi connection in the loop
 */
void wifi_loop() 
{
  // Reconnect if the WiFi is not connected
  if (!WiFi.isConnected())
    wifi_reconnect(WIFI_CONNECTION_TIMEOUT_SECONDS);
}

/**
 * @brief Function to establish the WiFi connection
 */
void wifi_connect() 
{
  delay(10);  // Short delay

  WiFi.mode(WIFI_STA);  // Set WiFi mode to station
  trace("MAC Address: ");
  traceln(WiFi.macAddress());  // Print the MAC address

#ifdef SSL_ROOT_CA
  // Set root CA certificate
  espWifiClient.setCACert(SSL_ROOT_CA);
  traceln("Enabling TLS/SSL Communications ...");
#endif

#ifdef SSL_CLIENT_CERTIFICATE
  // Set client certificate and private key
  espWifiClient.setCertificate(SSL_CLIENT_CERTIFICATE);
  espWifiClient.setPrivateKey(SSL_CLIENT_PRIVATE_KEY);
  traceln("Allowing SSL validation with Client Certificate");
#endif

  // Attempt to reconnect to WiFi
  wifi_reconnect(WIFI_CONNECTION_TIMEOUT_SECONDS);
}

/**
 * @brief Function to reconnect to WiFi if disconnected
 * @param retries Number of connection attempts
 */
void wifi_reconnect(uint retries) 
{
  trace("Connecting to ");
  traceln(wifiSSID);
  WiFi.begin(wifiSSID, wifiPasswd);  // Begin WiFi connection

  uint8_t r = 0;
  // Retry connection until successful or retries exhausted
  while (WiFi.status() != WL_CONNECTED && r < retries) 
  {
    r++;
    delay(1000);  // Wait 1 second between attempts
    trace(".");
  }
  traceln("");

  if (WiFi.isConnected()) 
  {
    debugln("-=- Connected to the WiFi network");
    debug("Local ESP32 IP: ");
    debugln(WiFi.localIP().toString());  // Print local IP address
  }
  else 
  {
    errorln("-X- Cannot connect to the WiFi network");  // Print error message if connection fails
  }
}

/*** End of file ****/