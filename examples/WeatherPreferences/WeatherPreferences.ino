#include <AsyncCPOnboardingSys.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

AsyncCPOnboardingSys portal;
const char* OPENWEATHER_API_KEY = "your_api_key_here";

// Custom HTML with city input
const char* customHTML = R"(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Setup</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial; margin: 0; padding: 20px; }
        .container { max-width: 400px; margin: 0 auto; }
        select, input, button { width: 100%; padding: 10px; margin: 10px 0; }
        .hidden { display: none; }
    </style>
</head>
<body>
    <div class="container">
        <div id="wifi-setup">
            <h2>WiFi Setup</h2>
            <select id="networks"></select>
            <input type="password" id="password" placeholder="WiFi Password">
            <button onclick="connect()">Connect</button>
        </div>
        
        <div id="city-setup" class="hidden">
            <h2>Location Setup</h2>
            <input type="text" id="city" placeholder="Enter your city">
            <button onclick="searchCity()">Search</button>
            <select id="city-results" class="hidden"></select>
            <button onclick="savePreferences()" id="save-btn" class="hidden">Save</button>
        </div>
    </div>
    <script>
        // JavaScript code for handling WiFi scanning, connection, and city search
        async function scanNetworks() {
            const response = await fetch('/scan');
            const data = await response.json();
            const select = document.getElementById('networks');
            data.networks.forEach(network => {
                const option = document.createElement('option');
                option.value = network.ssid;
                option.text = `${network.ssid} (${network.rssi}dBm)`;
                select.appendChild(option);
            });
        }

        async function connect() {
            const ssid = document.getElementById('networks').value;
            const password = document.getElementById('password').value;
            const response = await fetch('/connect', {
                method: 'POST',
                headers: {'Content-Type': 'application/x-www-form-urlencoded'},
                body: `ssid=${encodeURIComponent(ssid)}&password=${encodeURIComponent(password)}`
            });
            
            if (response.ok) {
                document.getElementById('wifi-setup').classList.add('hidden');
                document.getElementById('city-setup').classList.remove('hidden');
            }
        }

        async function searchCity() {
            const city = document.getElementById('city').value;
            const response = await fetch('/search-city', {
                method: 'POST',
                headers: {'Content-Type': 'application/x-www-form-urlencoded'},
                body: `city=${encodeURIComponent(city)}`
            });
            
            const data = await response.json();
            const select = document.getElementById('city-results');
            select.innerHTML = '';
            
            data.forEach(result => {
                const option = document.createElement('option');
                option.value = JSON.stringify({lat: result.lat, lon: result.lon});
                option.text = `${result.name}, ${result.state || ''}, ${result.country}`;
                select.appendChild(option);
            });
            
            select.classList.remove('hidden');
            document.getElementById('save-btn').classList.remove('hidden');
        }

        async function savePreferences() {
            const cityData = JSON.parse(document.getElementById('city-results').value);
            const response = await fetch('/save', {
                method: 'POST',
                headers: {'Content-Type': 'application/x-www-form-urlencoded'},
                body: `lat=${cityData.lat}&lon=${cityData.lon}`
            });
            
            if (response.ok) {
                alert('Settings saved! ESP32 will restart.');
            }
        }

        // Start network scanning when page loads
        scanNetworks();
    </script>
</body>
</html>
)";

void handlePreferences(JsonDocument& preferences) {
    // Handle the saved location preferences
    float lat = preferences["lat"];
    float lon = preferences["lon"];
    // Store or use the coordinates as needed
}

void onConnected() {
    Serial.println("Successfully connected to WiFi!");
}

void setup() {
    Serial.begin(115200);
    
    portal.setCustomHTML(customHTML);
    portal.setPreferencesCallback(handlePreferences);
    portal.setOnConnectedCallback(onConnected);
    portal.begin("ESP32-Weather-Setup");
}

void loop() {
    // Your main code here
    delay(10);
}