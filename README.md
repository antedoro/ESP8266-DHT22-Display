# ESP8266 DHT22 Temperature and Humidity Monitor with SSD1306 OLED Display

This project demonstrates how to read temperature and humidity data from a DHT22 sensor using an ESP8266 microcontroller and display the results on an SSD1306 OLED display.

## Components Required

- ESP8266 NodeMCU or any compatible ESP8266 module
- DHT22 Temperature and Humidity Sensor
- SSD1306 OLED Display (128x64)
- Jumper wires
- Breadboard (optional)

## Pin Connections

### DHT22 Connections

| DHT22 Pin | ESP8266 Pin | Description                |
|-----------|-------------|----------------------------|
| VCC       | 3.3V        | Power supply               |
| GND       | GND         | Ground                     |
| DATA      | D5 (GPIO 14)| Digital signal             |

### SSD1306 Connections

| SSD1306 Pin | ESP8266 Pin | Description                |
|-------------|-------------|----------------------------|
| VCC         | 3.3V        | Power supply               |
| GND         | GND         | Ground                     |
| SDA         | D2 (GPIO 4) | Data line                  |
| SCL         | D1 (GPIO 5) | Clock line                 |

**Note:** Ensure that the ESP8266 is powered with 3.3V, as higher voltages can damage the board.

## Libraries Required

You will need to install the following libraries through the Arduino Library Manager:

1. **Adafruit SSD1306**: For controlling the OLED display.
2. **Adafruit GFX**: Required for graphical functions.
3. **DHT sensor library**: For reading data from the DHT22 sensor.
4. **ESPAsyncWebServer**: For creating a web server to serve sensor data.

To install these libraries, follow these steps:

1. Open the Arduino IDE.
2. Go to **Sketch** > **Include Library** > **Manage Libraries...**
3. Search for and install:
   - `Adafruit SSD1306`
   - `Adafruit GFX`
   - `DHT sensor library`
   - `ESPAsyncWebServer`
   - `ESPAsyncTCP` (may be required for ESPAsyncWebServer)

## Code Upload

1. Open the Arduino IDE.
2. Copy the provided code into a new sketch.
3. Replace the following lines in the code with your Wi-Fi network credentials:
   ```cpp
   const char* ssid = "YOUR_SSID"; // Replace with your Wi-Fi network name
   const char* password = "YOUR_PASSWORD"; // Replace with your Wi-Fi password

4. Select your ESP8266 board from **Tools** > **Board** > `NodeMCU 1.0 (ESP-12E Module)` (or whichever ESP8266 board you are using).
5. Connect your ESP8266 to your computer via USB.
6. Select the correct COM port from **Tools** > **Port**.
7. Click the upload button to compile and upload the code to your ESP8266.

## Accessing the Data

After uploading the code and connecting the ESP8266 to your Wi-Fi, you can read the temperature and humidity data in two ways:

1. **Through Serial Connection**:
   - Open the Serial Monitor in the Arduino IDE (set to 115200 baud rate).
   - You will see the temperature and humidity values printed in the Serial Monitor output.

2. **Via Web Browser**:
   - Open the Serial Monitor to find the IP address assigned to the ESP8266 by your router.
   - Enter the IP address in a web browser to access the temperature and humidity readings. The readings will be updated every 10 seconds.

## Display

The OLED display will show the current temperature and humidity values read from the DHT22 sensor.

## Troubleshooting

- Ensure all connections are secure and correct.
- Check the Serial Monitor for error messages or connection issues.
- If the display does not initialize, check the I2C connections and ensure the correct address is set in the code.

## License

This project is open-source and available for modification and redistribution. Enjoy building and experimenting!
