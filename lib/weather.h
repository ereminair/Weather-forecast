#pragma once

#include "cpr/cpr.h"
#include "nlohmann/json.hpp"
#include "ftxui/dom/table.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>

const std::filesystem::path kTownsPath = "../towns.json";
const std::string kCoordinateUrl = "https://api.api-ninjas.com/v1/city";
const std::string kForecastUrl = "https://api.open-meteo.com/v1/forecast";
const std::string kToken = "AqvK8LLkZUZCdAxu/hMgXg==yYA371wj0jOdbgmr";


const std::map<int, std::string> kCodeWeather {
    {0, "Clear sky"},
    {1, "Mainly clear"},
    {2, "Partly cloudy"},
    {3, "overcast"},
    {45, "Fog"},
    {48, "depositing rime fog"},
    {51, "Drizzle: Light intensity"},
    {53, "Drizzle: Moderate intensity"},
    {55, "Drizzle: Dense intensity"},
    {56, "Freezing Drizzle: Light intensity"},
    {57, "Freezing Drizzle: Dense intensity"},
    {61, "Rain: Slight intensity"},
    {63, "Rain: Moderate intensity"},
    {65, "Rain: Heavy intensity"},
    {66, "Freezing Rain: Light intensity"},
    {67, "Freezing Rain: Heavy intensity"},
    {71, "Snow fall: Slight intensity"},
    {73, "Snow fall: Moderate intensity"},
    {75, "Snow fall: Heavy intensity"},
    {77, "Snow grains"},
    {80, "Rain showers: Slight intensity"},
    {81, "Rain showers: Moderate intensity"},
    {82, "Rain showers: Violent intensity"},
    {85, "Snow showers: Slight intensity"},
    {86, "Snow showers: Heavy intensity"},
    {95, "Thunderstorm: Slight or moderate"},
    {96, "Thunderstorm with slight hail"},
    {99, "Thunderstorm with heavy hail"}
};

const std::map<int, std::string> kDayOfWeek {
        {0, "Sunday"},
        {1, "Monday"},
        {2, "Tuesday"},
        {3, "Wednesday"},
        {4, "Thursday"},
        {5, "Friday"},
        {6, "Saturday"},

};



class SummaryCiti {
public:
    explicit SummaryCiti(std::string name_city);
    void PrintWeather();
    std::string CreateWeatherToday(const nlohmann::json& transmit_weather);
    void WeatherUpdate();
    std::vector<std::vector<std::string>> GetCreateTableWeather();
    void SetCountDays(int count_days);
    // std::string CreateData(const nlohmann::json& transit_weather);

private:
    std::string city_;
    std::string country_;
    nlohmann::json api_parse_;
    float latitude;
    float longitude;
    int count_days_;
    nlohmann::json this_day_weather;
    nlohmann::json next_day_weather;
};

struct Params{ // new
    int frequency;
    int count_days;
    int index;
};

void Parse(std::vector<std::string> &cities, std::vector<SummaryCiti> &weather_vector, Params& config); // new

void InitialiseTable(std::vector<SummaryCiti> &weather_vector, Params &config,
                     std::vector<std::vector<std::string>> &table_vec, ftxui::Table &table_c,
                     std::shared_ptr<ftxui::Node>& document, ftxui::Screen& screen); // new

void RunTable(std::vector<std::string> &cities, std::vector<SummaryCiti> &weather_vector, Params &config,
              std::vector<std::vector<std::string>> &table_vec, ftxui::Table &table_c,
              std::shared_ptr<ftxui::Node>& document, ftxui::Screen& screen); // new

std::vector<std::vector<std::string>> CreateTableWeather(const nlohmann::json& forecast, int day);

void SetTableProperties(ftxui::Table& table, int width, int height);

void UpdateConsole(std::vector<SummaryCiti>& weather_vector, int index,
                   std::vector<std::vector<std::string>>& table_vec, ftxui::Table table_c,
                   ftxui::Element& document, ftxui::Screen& screen);