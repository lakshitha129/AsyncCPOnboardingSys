#ifndef ASYNC_CP_ONBOARDING_SYS_H
#define ASYNC_CP_ONBOARDING_SYS_H

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <ArduinoJson.h>

class AsyncCPOnboardingSys {
public:
    typedef std::function<void(JsonDocument& preferences)> PreferencesCallback;
    typedef std::function<void(void)> ConnectionCallback;

    AsyncCPOnboardingSys();
    void begin(const char* apSSID = "ESP32-Setup", const char* apPassword = NULL);
    void setPreferencesCallback(PreferencesCallback callback);
    void setOnConnectedCallback(ConnectionCallback callback);
    void setCustomHTML(const char* html);
    void stop();
    bool isPortalActive();
    void loop(); // New method for background tasks
    
private:
    AsyncWebServer server;
    DNSServer dnsServer;
    Preferences preferences;
    PreferencesCallback preferencesCallback;
    ConnectionCallback connectedCallback;
    
    String customHTML;
    bool portalActive;
    uint8_t connectionAttempts;
    static const uint8_t MAX_CONNECTION_ATTEMPTS = 4;
    unsigned long lastScanTime;
    static const unsigned long SCAN_INTERVAL = 10000; // 10 seconds

    // Optimization: Cache for network scan results
    struct NetworkInfo {
        String ssid;
        int32_t rssi;
        uint8_t encType;
    };
    std::vector<NetworkInfo> networkCache;

    void setupCaptivePortal();
    void setupDNS();
    void handleWiFiScan(AsyncWebServerRequest *request);
    void handleConnect(AsyncWebServerRequest *request);
    void handlePreferences(AsyncWebServerRequest *request);
    void handleSave(AsyncWebServerRequest *request);
    bool tryConnect(const char* ssid, const char* password);
    void loadSavedCredentials();
    void updateNetworkCache();
    
    static String getContentType(const String& filename);
    static String processor(const String& var);

    // New methods for optimization
    void handleDNS();
    void cleanupConnections();
    void optimizeMemory();
};

#endif // ASYNC_CP_ONBOARDING_SYS_H