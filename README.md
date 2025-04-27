# Weather-Forecasting-using-OpenWeatherMap-API
Made a Weather Forecasting using API from OpenWeatherMap 

---

# Weather Checker

A simple C program that fetches and displays the current weather information for any city using the OpenWeatherMap API.

## Features
- Get real-time temperature, humidity, and pressure
- View weather conditions and descriptions
- Display sunrise and sunset times based on local timezone
- See wind speed and country code
- User-friendly terminal interface

## Requirements
- **libcurl** (for making HTTP requests)
- **cJSON** (for parsing JSON responses)
- **OpenWeatherMap API key**

## How to Build
Make sure you have `libcurl` and `cJSON` installed on your system.

### Ubuntu/Linux
```bash
sudo apt update
sudo apt install libcurl4-openssl-dev libcjson-dev gcc
gcc openweather.c -o weather -lcurl -lcjson
```

### Windows (MinGW)
Install libcurl and cJSON development libraries, then compile with:
```bash
gcc openweather.c -o weather.exe -lcurl -lcjson
```

## How to Run
```bash
./weather
```
Then, enter the name of the city when prompted.

## Notes
- The program uses a hardcoded **API key**:  
  `enter your api key`
- If you plan to distribute or publish this app, **replace the API key** with your own from [OpenWeatherMap](https://openweathermap.org/api).

## Example
```
Welcome to the Weather Checker!
------------------------------------------------------------
This program shows the current weather in any city.

Enter the city name: Paris

Temperature: 18.45 °C
It feels like: 17.80 °C
Minimum Temperature: 17.00 °C
Maximum Temperature: 20.00 °C

Pressure: 1012 mBar
Humidity: 72 %
Weather Condition: clear sky
Wind Speed: 5.13 kph

Sunrise: 05:45:23
Sunset: 20:58:12
Country Code: FR
```

## File Structure
| File | Description |
| --- | --- |
| `openweather.c` | Main C file containing all logic |

## License
This project is for educational purposes. Please respect OpenWeatherMap's [terms of service](https://openweathermap.org/terms).
