# AsyncCPOnboardingSys

A lightweight, asynchronous ESP32 captive portal library for streamlined device onboarding and setup. This library provides an elegant solution for WiFi configuration and custom preference setup with a modern, responsive web interface.

## Features

- **Asynchronous Operation**: Built on AsyncTCP and ESPAsyncWebServer for efficient handling of multiple connections
- **Modern Web Interface**: Responsive design with clean, user-friendly UI
- **Customizable Setup Flow**: Easily extend with custom configuration steps
- **Persistent Storage**: Automatic handling of WiFi credentials and custom preferences
- **Smart Reconnection**: Automatic retry logic with configurable attempts
- **DNS Server**: Built-in DNS server for captive portal detection
- **Progress Feedback**: Visual feedback for network operations
- **Error Handling**: Comprehensive error handling and user feedback

## Credits

This library is based on several excellent open-source projects:

- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) by Me No Dev
- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP) by Me No Dev
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson) by Benoit Blanchon
- [async-esp-fs-webserver](https://github.com/cotestatnt/async-esp-fs-webserver) by cotestatnt

## Installation

### PlatformIO

Add to your `platformio.ini`:

```ini
lib_deps =
    AsyncCPOnboardingSys
```

### Manual Installation

1. Download this repository
2. Extract to your Arduino/libraries folder
3. Restart your IDE

## Basic Usage

```cpp
#include <AsyncCPOnboardingSys.h>

AsyncCPOnboardingSys portal;

void handlePreferences(JsonDocument& preferences) {
    // Handle custom preferences here
}

void onConnected() {
    // Called when WiFi connection is established
}

void setup() {
    portal.setPreferencesCallback(handlePreferences);
    portal.setOnConnectedCallback(onConnected);
    portal.begin("ESP32-Setup");
}

void loop() {
    // Your main code here
}
```

## Advanced Usage

### Custom Setup Interface

1. Create your custom HTML:
```cpp
const char* customHTML = R"(
    // Your custom HTML here
    // See examples/CustomSetup for a complete example
)";
```

2. Set up preference handling:
```cpp
void handlePreferences(JsonDocument& preferences) {
    String customValue = preferences["customKey"].as<String>();
    // Handle your custom preferences
}
```

3. Initialize the portal:
```cpp
portal.setCustomHTML(customHTML);
portal.setPreferencesCallback(handlePreferences);
portal.begin("ESP32-Custom-Setup");
```

### Extending the Library

The library is designed to be easily extensible. Key extension points:

1. **Custom HTML Interface**: Create your own setup interface while maintaining the core WiFi setup functionality
2. **Preference Handling**: Implement custom preference storage and retrieval
3. **Connection Callbacks**: Add custom logic for successful connections
4. **Error Handling**: Implement custom error handling and user feedback

## API Reference

### Core Methods

- `begin(const char* apSSID, const char* apPassword = NULL)`
  - Starts the portal with the specified AP credentials
  - Returns: void

- `setPreferencesCallback(PreferencesCallback callback)`
  - Sets the callback for handling custom preferences
  - Returns: void

- `setOnConnectedCallback(ConnectionCallback callback)`
  - Sets the callback for successful WiFi connection
  - Returns: void

- `setCustomHTML(const char* html)`
  - Sets custom HTML for the portal interface
  - Returns: void

- `stop()`
  - Stops the portal and cleanup resources
  - Returns: void

- `isPortalActive()`
  - Checks if the portal is currently active
  - Returns: bool

### Events

The library provides several events you can hook into:

- WiFi Connection Success/Failure
- Preference Save/Load
- Portal State Changes

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

### Development Setup

1. Clone the repository
2. Install PlatformIO
3. Open the project in VSCode with PlatformIO extension
4. Run the example projects

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

Special thanks to the authors and contributors of the libraries this project is based on:

- Me No Dev for ESPAsyncWebServer and AsyncTCP
- Benoit Blanchon for ArduinoJson
- cotestatnt for async-esp-fs-webserver

Their excellent work made this library possible.