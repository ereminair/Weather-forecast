#include "weather.h"

void Parse(std::vector<std::string> &cities, std::vector<SummaryCiti> &weather_vector, Params &config) {
    nlohmann::json towns = nlohmann::json::parse(std::ifstream(kTownsPath));
    cities = towns["town"];

    config.frequency = towns["frequency"];
    config.count_days = towns["count_days"];
    config.index = 0;

    std::cout << "Please, waiting..." << std::endl;

    for (auto &citi: cities) {
        weather_vector.emplace_back(citi);
    }
}

void InitialiseTable(std::vector<SummaryCiti> &weather_vector, Params &config,
                     std::vector<std::vector<std::string>> &table_vec, ftxui::Table &table_c,
                     std::shared_ptr<ftxui::Node>& document, ftxui::Screen& screen) {
    system("cls");
    weather_vector[config.index].PrintWeather();

    table_vec = weather_vector[config.index].GetCreateTableWeather();
    table_c = ftxui::Table(table_vec);
    SetTableProperties(table_c, table_vec[0].size(), table_vec.size());
    document = table_c.Render();
    screen = ftxui::Screen::Create(ftxui::Dimension::Full(), ftxui::Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
}


void RunTable(std::vector<std::string> &cities, std::vector<SummaryCiti> &weather_vector, Params &config,
                     std::vector<std::vector<std::string>> &table_vec, ftxui::Table &table_c,
                     std::shared_ptr<ftxui::Node>& document, ftxui::Screen& screen) {
    bool is_exit = false;
    auto start = std::chrono::system_clock::now();
    while(!is_exit) {
        if(GetAsyncKeyState(0x4E) & 0x80060) {
            config.index++;
            if(config.index >= cities.size()) {
                config.index = 0;
            }

            UpdateConsole(weather_vector, config.index, table_vec, table_c, document, screen);
            Sleep(250);
        }
        if(GetAsyncKeyState(0x50) & 0x8000) {
            config.index--;
            if(config.index < 0) {
                config.index = cities.size() - 1;
            }

            UpdateConsole(weather_vector, config.index, table_vec, table_c, document, screen);
            Sleep(250);
        }
        if(GetAsyncKeyState(0xBB) & 0x8000) {
            config.count_days++;
            if(config.count_days > 16) {
                config.count_days = 16;
                for(auto& elemen_vector: weather_vector) {
                    elemen_vector.SetCountDays(config.count_days);
                }

            }

            UpdateConsole(weather_vector, config.index, table_vec, table_c, document, screen);
            Sleep(250);
        }
        if(GetAsyncKeyState(0xBD) & 0x8000) {
            config.count_days--;
            if(config.count_days < 1) {
                config.count_days = 1;
            }

            for(auto& elemen_vector: weather_vector) {
                elemen_vector.SetCountDays(config.count_days);
            }

            UpdateConsole(weather_vector, config.index, table_vec, table_c, document, screen);
            Sleep(250);
        }
        if(GetAsyncKeyState(0x1B) & 0x8000) {
            is_exit = true;
        }
        auto end = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::minutes>(end - start).count() >= config.frequency) {
            for(auto& elemen_vector: weather_vector) {
                elemen_vector.WeatherUpdate();
            }
            UpdateConsole(weather_vector, config.index, table_vec, table_c, document, screen);
            start = std::chrono::system_clock::now();
        }
    }
}


void SummaryCiti::PrintWeather() {
    std::cout << "Weather forecast: " << city_ << ", " << country_ << std::endl;
    std::cout << CreateWeatherToday(this_day_weather) << std::endl;

};

std::string Wind_Direction(float wind_directon) {
    std::string direction;
    if (wind_directon >= 0 && wind_directon <= 45) {
        direction = "N";
    } else if (wind_directon > 46 && wind_directon <= 90) {
        direction = "NE";
    } else if (wind_directon > 90 && wind_directon <= 135) {
        direction = "E";
    } else if (wind_directon > 135 && wind_directon <= 180) {
        direction = "S";
    } else if (wind_directon > 180 && wind_directon <= 225) {
        direction = "SW";
    } else if (wind_directon > 225 && wind_directon <= 270) {
        direction = "W";
    } else if (wind_directon > 270 && wind_directon <= 315) {
        direction = "NW";
    } else {
        direction = "N";
    }

    return direction;
}

std::string ChekCodeWeather(int weather_code) {
    if (kCodeWeather.find(weather_code) != kCodeWeather.end()) {
        return kCodeWeather.at(weather_code);
    } else {
        return "Error: Unknown weather was transmitted";
    }
}

std::string ChekCodeDay(int weather_code) {
    if (kDayOfWeek.find(weather_code) != kDayOfWeek.end()) {
        return kDayOfWeek.at(weather_code);
    } else {
        return "Error: Unknown weather was transmitted";
    }
}

std::string SummaryCiti::CreateWeatherToday(const nlohmann::json &transmit_weather) {
    int weather_code = transmit_weather["current_weather"]["weathercode"];
    float wind_direction = transmit_weather["current_weather"]["winddirection"];
    int wind_speed = static_cast<int>(transmit_weather["current_weather"]["windspeed"]);
    int temperature = static_cast<int>(transmit_weather["current_weather"]["temperature"]);
    std::string weather_today =
            ChekCodeWeather(weather_code) + '\n' + '\t' + std::to_string(temperature) + " C" + '\n' + '\t' +
            std::to_string(wind_speed) + " km/h" + '\n' + '\t' + Wind_Direction(wind_direction);

    return weather_today;
}

SummaryCiti::SummaryCiti(std::string name_city) : city_(std::move(name_city)) {
    try {
        cpr::Response api_ninjas_city = cpr::Get(cpr::Url{kCoordinateUrl},
                                                 cpr::Parameters{{"name", city_}},
                                                 cpr::Header{{"X-Api-Key", kToken}});
        api_parse_ = nlohmann::json::parse(api_ninjas_city.text);
        latitude = api_parse_[0]["latitude"];
        longitude = api_parse_[0]["longitude"];
        country_ = api_parse_[0]["country"];
        nlohmann::json cfg = nlohmann::json::parse(std::ifstream(kTownsPath));
        count_days_ = cfg["count_days"];
        cpr::Response open_meteo = cpr::Get(cpr::Url{kForecastUrl},
                                            cpr::Parameters{{"latitude",        std::to_string(latitude)},
                                                            {"longitude",       std::to_string(longitude)},
                                                            {"current_weather", "true"}});

        this_day_weather = nlohmann::json::parse(open_meteo.text);
        cpr::Response open_meteo_next_days = cpr::Get(cpr::Url{kForecastUrl},
                                                      cpr::Parameters{{"latitude",      std::to_string(latitude)},
                                                                      {"longitude",     std::to_string(longitude)},
                                                                      {"hourly",        "temperature_2m"},
                                                                      {"forecast_days", "16"},
                                                                      {"hourly",        "weathercode"},
                                                                      {"hourly",        "windspeed_10m"},
                                                                      {"hourly",        "winddirection_10m"}});

        next_day_weather = nlohmann::json::parse(open_meteo_next_days.text);
//        std::cout  << std::endl << "THIS DAY:  " << this_day_weather << std::endl;
//        std::cout << "NEXT DAY:  " << next_day_weather << std::endl;
        return;
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        system("pause");
    }
}

void SummaryCiti::WeatherUpdate() {
    try {
        cpr::Response open_meteo = cpr::Get(cpr::Url{kForecastUrl},
                                            cpr::Parameters{{"latitude",        std::to_string(latitude)},
                                                            {"longitude",       std::to_string(longitude)},
                                                            {"current_weather", "true"}});

        this_day_weather = nlohmann::json::parse(open_meteo.text);
        cpr::Response open_meteo_next_days = cpr::Get(cpr::Url{kForecastUrl},
                                                      cpr::Parameters{{"latitude",      std::to_string(latitude)},
                                                                      {"longitude",     std::to_string(longitude)},
                                                                      {"hourly",        "temperature_2m"},
                                                                      {"forecast_days", "16"},
                                                                      {"hourly",        "weathercode"},
                                                                      {"hourly",        "windspeed_10m"},
                                                                      {"hourly",        "winddirection_10m"}});

        next_day_weather = nlohmann::json::parse(open_meteo_next_days.text);
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        system("pause");
    }
}

std::vector<std::vector<std::string>> CreateTableWeather(const nlohmann::json &forecast, int day) {
    std::vector<std::vector<std::string>> table;
    table.push_back({"Morning", "Afternoon", "Evening", "Night"});

    for (int i = 0; i < day; i++) {
        std::string data_today = forecast["hourly"]["time"][i * 24];
        std::string month_str = data_today.substr(5, 2);
        std::string day_str = data_today.substr(8, 2);
        std::string year_str = data_today.substr(0, 4);

        int year = std::stoi(year_str);
        int month = std::stoi(month_str);
        int day = std::stoi(day_str);
        std::tm timeinfo = {};
        timeinfo.tm_year = year - 1900;
        timeinfo.tm_mon = month - 1;
        timeinfo.tm_mday = day;
        std::time_t time = std::mktime(&timeinfo);
        const std::tm *timeinfo_weekday = std::localtime(&time);
        int weekday = timeinfo_weekday->tm_wday;
        std::string week_day = ChekCodeDay(weekday);
        table.push_back({week_day, day_str, month_str, year_str});

        int w_id_morning = forecast["hourly"]["weathercode"][i * 24];
        int w_id_afternoon = forecast["hourly"]["weathercode"][i * 24 + 6];
        int w_id_evening = forecast["hourly"]["weathercode"][i * 24 + 12];
        int w_id_night = forecast["hourly"]["weathercode"][i * 24 + 18];

        std::string morning_w = ChekCodeWeather(w_id_morning);
        std::string afternoon_w = ChekCodeWeather(w_id_afternoon);
        std::string evening_w = ChekCodeWeather(w_id_evening);
        std::string night_w = ChekCodeWeather(w_id_night);
        table.push_back({morning_w, afternoon_w, evening_w, night_w});

        float temp_morning = forecast["hourly"]["temperature_2m"][i * 24 + 6];
        float temp_afternoon = forecast["hourly"]["temperature_2m"][i * 24 + 12];
        float temp_evening = forecast["hourly"]["temperature_2m"][i * 24 + 18];
        float temp_night = forecast["hourly"]["temperature_2m"][i * 24];
        table.push_back({std::to_string(static_cast<int>(temp_morning)) + "°C",
                         std::to_string(static_cast<int>(temp_afternoon)) + "°C",
                         std::to_string(static_cast<int>(temp_evening)) + "°C",
                         std::to_string(static_cast<int>(temp_night)) + "°C"});

        float wind_speed_morning = forecast["hourly"]["windspeed_10m"][i * 24 + 6];
        float wind_speed_afternoon = forecast["hourly"]["windspeed_10m"][i * 24 + 12];
        float wind_speed_evening = forecast["hourly"]["windspeed_10m"][i * 24 + 18];
        float wind_speed_night = forecast["hourly"]["windspeed_10m"][i * 24];
        table.push_back({std::to_string(static_cast<int>(wind_speed_morning)) + "km/h",
                         std::to_string(static_cast<int>(wind_speed_afternoon)) + "km/h",
                         std::to_string(static_cast<int>(wind_speed_evening)) + "km/h",
                         std::to_string(static_cast<int>(wind_speed_night)) + "km/h"});

        float wind_deg_morning = forecast["hourly"]["winddirection_10m"][i * 24 + 6];
        float wind_deg_afternoon = forecast["hourly"]["winddirection_10m"][i * 24 + 12];
        float wind_deg_evening = forecast["hourly"]["winddirection_10m"][i * 24 + 18];
        float wind_deg_night = forecast["hourly"]["winddirection_10m"][i * 24];
        table.push_back({Wind_Direction(wind_deg_morning), Wind_Direction(wind_deg_afternoon),
                         Wind_Direction(wind_deg_evening), Wind_Direction(wind_deg_night)});
    }
    return table;
}

std::vector<std::vector<std::string>> SummaryCiti::GetCreateTableWeather() {
    return CreateTableWeather(next_day_weather, count_days_);
}

void SetTableProperties(ftxui::Table &table, int width, int height) {
    table.SelectAll().Border(ftxui::LIGHT);
    table.SelectRow(0).DecorateCellsAlternateRow(color(ftxui::Color::Cyan));

    for (int i = 0; i < width; ++i) {
        table.SelectColumn(i).Border(ftxui::LIGHT);
    }

    for (int i = 1; i < height; i += 5) {
        table.SelectRow(i).Border(ftxui::LIGHT);
    }
}

void UpdateConsole(std::vector<SummaryCiti> &weather_vector, int index,
                   std::vector<std::vector<std::string>> &table_vec, ftxui::Table table_c,
                   ftxui::Element &document, ftxui::Screen &screen) {
    system("cls");
    weather_vector[index].PrintWeather();
    table_vec = weather_vector[index].GetCreateTableWeather();
    table_c = ftxui::Table(table_vec);
    SetTableProperties(table_c, table_vec[0].size(), table_vec.size());
    document = table_c.Render();
    screen = ftxui::Screen::Create(ftxui::Dimension::Full(), ftxui::Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
}

void SummaryCiti::SetCountDays(int count_days) {
    count_days_ = count_days;
}