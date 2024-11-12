#include <AsyncCPOnboardingSys.h>
#include <ArduinoJson.h>

AsyncCPOnboardingSys portal;

// Custom HTML with additional setup fields
const char* customHTML = R"(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Custom Setup</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        /* Include the default styles from default_html.h */
        /* Add your custom styles here */
        .custom-field {
            margin-bottom: 1rem;
        }
    </style>
</head>
<body>
    <div class="container">
        <div id="wifi-setup" class="card">
            <!-- WiFi setup section remains the same -->
        </div>
        
        <div id="custom-setup" class="card hidden">
            <h2>Device Configuration</h2>
            <div class="custom-field">
                <label for="device-name">Device Name</label>
                <input type="text" id="device-name" placeholder="Enter device name">
            </div>
            <div class="custom-field">
                <label for="update-interval">Update Interval (minutes)</label>
                <input type="number" id="update-interval" min="1" max="60" value="5">
            </div>
            <div class="custom-field">
                <label for="mode">Operating Mode</label>
                <select id="mode">
                    <option value="normal">Normal</option>
                    <option value="power-save">Power Save</option>
                    <option value="performance">Performance</option>
                </select>
            </div>
            <button onclick="savePreferences()" id="save-btn">Save Configuration</button>
        </div>
    </div>

    <script>
        // Include the default JavaScript
        // Add custom JavaScript for handling the additional fields
        function gatherCustomData() {
            return {
                deviceName: document.getElementById('device-name').value,
                updateInterval: document.getElementById('update-interval').value,
                operatingMode: document.getElementById('mode').value
            };
        }
    </script>
</body>
</html>
)";

void handlePreferences(JsonDocument& preferences) {
    String deviceName = preferences["deviceName"].as<String>();
    int updateInterval = preferences["updateInterval"].as<int>();
    String operatingMode = preferences["operatingMode"].as<String>();
    
    // Store preferences in NVS or use them as needed
    Serial.printf("Device Name: %s\n", deviceName.c_str());
    Serial.printf("Update Interval: %d minutes\n", updateInterval);
    Serial.printf("Operating Mode: %s\n", operatingMode.c_str());
}

void onConnected() {
    Serial.println("WiFi connected successfully!");
}

void setup() {
    Serial.begin(115200);
    
    portal.setCustomHTML(customHTML);
    portal.setPreferencesCallback(handlePreferences);
    portal.setOnConnectedCallback(onConnected);
    portal.begin("ESP32-Custom-Setup");
}

void loop() {
    delay(10);
}