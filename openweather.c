#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

void print_intro() {
    system("cls");  
    printf("             Welcome to the Weather Checker! \n");
    printf("------------------------------------------------------------\n");
    printf("This program shows the current weather in any city.\n");
}

// Convert UNIX timestamp to local readable time 
void convertUnixTimestamp(time_t timestamp, int timezone_offset) {
    timestamp += timezone_offset;
    struct tm *time_info = gmtime(&timestamp);
    printf("%02d:%02d:%02d\n", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
}

// Function to parse JSON response and display weather data
void handle_data(char *buffer) {
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        printf("Error parsing JSON.\n");
        return;
    }

    cJSON *cod = cJSON_GetObjectItem(json, "cod");
    if (cJSON_IsNumber(cod) && cod->valueint != 200) {
        cJSON *message = cJSON_GetObjectItem(json, "message");
        if (cJSON_IsString(message))
            printf("Error: %s\n", message->valuestring);
        else
            printf("Error: City not found or invalid response.\n");
        cJSON_Delete(json);
        return;
    }

    setlocale(LC_ALL, ""); // For degree symbol

    // Main weather data
    cJSON *data = cJSON_GetObjectItem(json, "main");
    if (cJSON_IsObject(data)) {
        cJSON *temperature = cJSON_GetObjectItem(data, "temp");
        cJSON *feel = cJSON_GetObjectItem(data, "feels_like");
        cJSON *min_temp = cJSON_GetObjectItem(data, "temp_min");
        cJSON *max_temp = cJSON_GetObjectItem(data, "temp_max");
        cJSON *pressure = cJSON_GetObjectItem(data, "pressure");
        cJSON *humidity = cJSON_GetObjectItem(data, "humidity");

        if (cJSON_IsNumber(temperature)) printf("Temperature: %.2f \260C\n", temperature->valuedouble);
        if (cJSON_IsNumber(feel))        printf("It feels like: %.2f \260C\n", feel->valuedouble);
        if (cJSON_IsNumber(min_temp))    printf("Minimum Temperature: %.2f \260C\n", min_temp->valuedouble);
        if (cJSON_IsNumber(max_temp))    printf("Maximum Temperature: %.2f \260C\n", max_temp->valuedouble);
        if (cJSON_IsNumber(pressure))    printf("\nPressure: %d mBar\n", pressure->valueint);
        if (cJSON_IsNumber(humidity))    printf("Humidity: %d %%\n", humidity->valueint);
    }

    // Weather description
    cJSON *weather_array = cJSON_GetObjectItem(json, "weather");
    if (cJSON_IsArray(weather_array)) {
        cJSON *weather = cJSON_GetArrayItem(weather_array, 0);
        cJSON *description = cJSON_GetObjectItem(weather, "description");
        if (cJSON_IsString(description))
            printf("Weather Condition: %s\n", description->valuestring);
    }

    // Wind speed
    cJSON *wind = cJSON_GetObjectItem(json, "wind");
    if (cJSON_IsObject(wind)) {
        cJSON *speed = cJSON_GetObjectItem(wind, "speed");
        if (cJSON_IsNumber(speed))
            printf("Wind Speed: %.2f kph\n", speed->valuedouble);
    }

    // Sunrise, sunset and country
    cJSON *timezone = cJSON_GetObjectItem(json, "timezone");
    cJSON *sys = cJSON_GetObjectItem(json, "sys");
    if (cJSON_IsObject(sys) && cJSON_IsNumber(timezone)) {
        cJSON *sunrise = cJSON_GetObjectItem(sys, "sunrise");
        cJSON *sunset = cJSON_GetObjectItem(sys, "sunset");
        cJSON *country = cJSON_GetObjectItem(sys, "country");

        if (cJSON_IsNumber(sunrise)) {
            printf("\nSunrise: ");
            convertUnixTimestamp(sunrise->valueint, timezone->valueint);
        }
        if (cJSON_IsNumber(sunset)) {
            printf("Sunset: ");
            convertUnixTimestamp(sunset->valueint, timezone->valueint);
        }
        if (cJSON_IsString(country))
            printf("\nCountry Code: %s\n", country->valuestring);
    }

    cJSON_Delete(json);
}

// Callback for libcurl to store API response
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    if (total_size < 50) {
        printf("City Not Found :( \nTry Again!\n");
    } else {
        handle_data(ptr);
    }
    return total_size;
}

int main(void) {
    print_intro();

    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize curl\n");
        return EXIT_FAILURE;
    }

    const char *api_url = "http://api.openweathermap.org/data/2.5/weather?q=";
    const char *api_key = "&appid=e6d43eafcf69f246c6180b997c652ea7&units=metric";

    char city[50];
    printf("Enter the city name: ");
    scanf("%49s", city);
    printf("\n");

    // Create full URL
    size_t url_length = strlen(api_url) + strlen(city) + strlen(api_key) + 1;
    char *full_url = (char *)malloc(url_length);
    if (!full_url) {
        fprintf(stderr, "Memory allocation failed\n");
        curl_easy_cleanup(curl);
        return EXIT_FAILURE;
    }
    snprintf(full_url, url_length, "%s%s%s", api_url, city, api_key);

    // Set up curl
    curl_easy_setopt(curl, CURLOPT_URL, full_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    // Execute request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "CURL error: %s\n", curl_easy_strerror(res));
    }

    free(full_url);
    curl_easy_cleanup(curl);
    return EXIT_SUCCESS;
}
