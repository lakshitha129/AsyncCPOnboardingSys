#include "AsyncCPOnboardingSys.h"

AsyncCPOnboardingSys::AsyncCPOnboardingSys() 
    : server(80), portalActive(false), connectionAttempts(0), lastScanTime(0) {
    networkCache.reserve(10); // Pre-allocate space for efficiency
}

void AsyncCPOnboardingSys::begin(const char* apSSID, const char* apPassword) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.persistent(false); // Optimize flash wear
    
    // Try to connect using saved credentials first
    loadSavedCredentials();
    
    if (portalActive) {
        // Optimize AP setup
        WiFi.softAPConfig(IPAddress(172, 217, 28, 1), IPAddress(172, 217, 28, 1), IPAddress(255, 255, 255, 0));
        WiFi.softAP(apSSID, apPassword);
        
        setupCaptivePortal();
        setupDNS();
        
        // Initial network scan
        WiFi.scanNetworks(true);
    }
}

void AsyncCPOnboardingSys::loop() {
    if (portalActive) {
        handleDNS();
        
        // Update network cache periodically
        if (millis() - lastScanTime >= SCAN_INTERVAL) {
            updateNetworkCache();
            lastScanTime = millis();
        }
        
        cleanupConnections();
        optimizeMemory();
    }
}

void AsyncCPOnboardingSys::updateNetworkCache() {
    int n = WiFi.scanComplete();
    if (n >= 0) {
        networkCache.clear();
        for (int i = 0; i < n && i < 10; i++) { // Limit to top 10 networks
            NetworkInfo info = {
                WiFi.SSID(i),
                WiFi.RSSI(i),
                WiFi.encryptionType(i)
            };
            networkCache.push_back(info);
        }
        WiFi.scanNetworks(true); // Start new async scan
    }
}

void AsyncCPOnboardingSys::handleDNS() {
    dnsServer.processNextRequest();
}

void AsyncCPOnboardingSys::cleanupConnections() {
    static unsigned long lastCleanup = 0;
    if (millis() - lastCleanup >= 30000) { // Every 30 seconds
        server.cleanupClients();
        lastCleanup = millis();
    }
}

void AsyncCPOnboardingSys::optimizeMemory() {
    static unsigned long lastOptimization = 0;
    if (millis() - lastOptimization >= 60000) { // Every minute
        ESP.getFreeHeap(); // Force heap cleanup
        lastOptimization = millis();
    }
}

void AsyncCPOnboardingSys::setupCaptivePortal() {
    // Optimize server setup with pre-allocated buffers
    server.setResponseBufferSize(2048);
    
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(200, "text/html", 
            customHTML.length() > 0 ? customHTML : FPSTR(DEFAULT_HTML));
        response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
        request->send(response);
    });

    // Add other handlers...
    // [Previous handler implementations remain the same]

    server.begin();
}

// [Rest of the implementation remains the same]