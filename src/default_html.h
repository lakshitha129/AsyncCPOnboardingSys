#ifndef DEFAULT_HTML_H
#define DEFAULT_HTML_H

const char* DEFAULT_HTML = R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Setup</title>
    <style>
        :root {
            --primary-color: #2196F3;
            --error-color: #f44336;
            --success-color: #4CAF50;
        }
        
        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
        }
        
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
            line-height: 1.6;
            color: #333;
            background: #f5f5f5;
        }
        
        .container {
            max-width: 400px;
            margin: 2rem auto;
            padding: 1rem;
        }
        
        .card {
            background: white;
            border-radius: 8px;
            padding: 1.5rem;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }
        
        h1, h2 {
            color: #1976D2;
            margin-bottom: 1rem;
        }
        
        .form-group {
            margin-bottom: 1rem;
        }
        
        label {
            display: block;
            margin-bottom: 0.5rem;
            font-weight: 500;
        }
        
        input, select, button {
            width: 100%;
            padding: 0.75rem;
            border: 1px solid #ddd;
            border-radius: 4px;
            font-size: 1rem;
        }
        
        input:focus, select:focus {
            outline: none;
            border-color: var(--primary-color);
            box-shadow: 0 0 0 2px rgba(33,150,243,0.2);
        }
        
        button {
            background: var(--primary-color);
            color: white;
            border: none;
            cursor: pointer;
            font-weight: 500;
            transition: background 0.3s ease;
        }
        
        button:hover {
            background: #1976D2;
        }
        
        button:disabled {
            background: #ccc;
            cursor: not-allowed;
        }
        
        .hidden {
            display: none;
        }
        
        .alert {
            padding: 0.75rem;
            border-radius: 4px;
            margin-bottom: 1rem;
        }
        
        .alert-error {
            background: #ffebee;
            color: var(--error-color);
            border: 1px solid var(--error-color);
        }
        
        .alert-success {
            background: #e8f5e9;
            color: var(--success-color);
            border: 1px solid var(--success-color);
        }
        
        .progress {
            width: 100%;
            height: 4px;
            background: #ddd;
            margin: 1rem 0;
            border-radius: 2px;
            overflow: hidden;
        }
        
        .progress-bar {
            height: 100%;
            background: var(--primary-color);
            width: 0%;
            transition: width 0.3s ease;
        }
        
        .network-item {
            display: flex;
            align-items: center;
            padding: 0.5rem;
            border: 1px solid #ddd;
            border-radius: 4px;
            margin-bottom: 0.5rem;
        }
        
        .network-signal {
            margin-right: 0.5rem;
        }
        
        .network-secure {
            margin-left: auto;
            color: #666;
        }
    </style>
</head>
<body>
    <div class="container">
        <div id="wifi-setup" class="card">
            <h1>WiFi Setup</h1>
            <div id="status-message"></div>
            <div class="progress hidden">
                <div class="progress-bar"></div>
            </div>
            <div class="form-group">
                <label for="networks">Available Networks</label>
                <select id="networks">
                    <option value="">Scanning...</option>
                </select>
            </div>
            <div class="form-group">
                <label for="password">Password</label>
                <input type="password" id="password" placeholder="WiFi Password">
            </div>
            <button onclick="connect()" id="connect-btn">Connect</button>
        </div>
        
        <div id="custom-setup" class="card hidden">
            <h2>Device Setup</h2>
            <div id="custom-content"></div>
            <button onclick="savePreferences()" id="save-btn">Save & Restart</button>
        </div>
    </div>

    <script>
        let networks = [];
        let currentStep = 'wifi';
        
        async function scanNetworks() {
            try {
                const response = await fetch('/scan');
                const data = await response.json();
                networks = data.networks;
                
                const select = document.getElementById('networks');
                select.innerHTML = '';
                
                networks.sort((a, b) => b.rssi - a.rssi);
                
                networks.forEach(network => {
                    const option = document.createElement('option');
                    option.value = network.ssid;
                    const signal = network.rssi > -50 ? '📶' : 
                                 network.rssi > -70 ? '📶' : '📶';
                    option.text = `${network.ssid} (${signal} ${network.rssi}dBm)`;
                    select.appendChild(option);
                });
            } catch (error) {
                showMessage('Network scan failed. Please refresh.', 'error');
            }
        }

        function showMessage(message, type = 'info') {
            const statusDiv = document.getElementById('status-message');
            statusDiv.className = `alert alert-${type}`;
            statusDiv.textContent = message;
        }

        async function connect() {
            const ssid = document.getElementById('networks').value;
            const password = document.getElementById('password').value;
            const connectBtn = document.getElementById('connect-btn');
            
            connectBtn.disabled = true;
            showMessage('Connecting...', 'info');
            
            try {
                const response = await fetch('/connect', {
                    method: 'POST',
                    headers: {'Content-Type': 'application/x-www-form-urlencoded'},
                    body: `ssid=${encodeURIComponent(ssid)}&password=${encodeURIComponent(password)}`
                });
                
                if (response.ok) {
                    showMessage('Connected successfully!', 'success');
                    document.getElementById('wifi-setup').classList.add('hidden');
                    document.getElementById('custom-setup').classList.remove('hidden');
                    currentStep = 'custom';
                } else {
                    showMessage('Connection failed. Please try again.', 'error');
                }
            } catch (error) {
                showMessage('Connection failed. Please try again.', 'error');
            }
            
            connectBtn.disabled = false;
        }

        async function savePreferences() {
            const saveBtn = document.getElementById('save-btn');
            saveBtn.disabled = true;
            
            try {
                const customData = gatherCustomData();
                const response = await fetch('/save', {
                    method: 'POST',
                    headers: {'Content-Type': 'application/x-www-form-urlencoded'},
                    body: new URLSearchParams(customData)
                });
                
                if (response.ok) {
                    showMessage('Settings saved! Device will restart...', 'success');
                } else {
                    showMessage('Failed to save settings.', 'error');
                    saveBtn.disabled = false;
                }
            } catch (error) {
                showMessage('Failed to save settings.', 'error');
                saveBtn.disabled = false;
            }
        }

        function gatherCustomData() {
            // Override this function in your custom implementation
            return {};
        }

        // Start network scanning when page loads
        scanNetworks();
        // Refresh network list every 10 seconds
        setInterval(scanNetworks, 10000);
    </script>
</body>
</html>
)";

#endif // DEFAULT_HTML_H