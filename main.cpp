#include "lib/weather.h"


int main(int argc, char **argv) {
    std::vector<std::string> cities;
    std::vector<SummaryCiti> weather_vector;
    Params config;

    Parse(cities, weather_vector, config);

    std::vector<std::vector<std::string>> table_vec;
    ftxui::Table table_c;
    std::shared_ptr<ftxui::Node> document;
    auto screen = ftxui::Screen::Create(ftxui::Dimension::Full(), ftxui::Dimension::Fit(document));

    InitialiseTable(weather_vector, config, table_vec, table_c, document, screen);

    RunTable(cities, weather_vector, config, table_vec, table_c, document, screen);


//    ''' Parse start
//    nlohmann::json towns = nlohmann::json::parse(std::ifstream(kTownsPath));
//    std::vector<std::string> cities = towns["town"];

//    int frequency = towns["frequency"];
//    int count_days = towns["count_days"];

//    int index = 0;

//    std::cout << "Please, waiting..." << std::endl;
//    std::vector<SummaryCiti> weather_vector;
//    for(auto& citi : cities) {
//        weather_vector.emplace_back(citi);
//    }
//    '''  Parse end

    // Initialise table start
//    system("cls");
//    weather_vector[index].PrintWeather();
//    auto table_vec = weather_vector[index].GetCreateTableWeather();
//    auto table_c = ftxui::Table(table_vec);
//    SetTableProperties(table_c, table_vec[0].size(), table_vec.size());
//    auto document = table_c.Render();
//    auto screen = ftxui::Screen::Create(ftxui::Dimension::Full(), ftxui::Dimension::Fit(document));
//    Render(screen, document);
//    screen.Print();
    // Initialise table start

    // RunTable
//    bool is_exit = false;
//    auto start = std::chrono::system_clock::now();
//    while(!is_exit) {
//        if(GetAsyncKeyState(0x4E) & 0x80060) {
//            index++;
//            if(index >= cities.size()) {
//                index = 0;
//            }
//
//            UpdateConsole(weather_vector, index, table_vec, table_c, document, screen);
//            Sleep(250);
//        }
//        if(GetAsyncKeyState(0x50) & 0x8000) {
//            index--;
//            if(index < 0) {
//                index = cities.size() - 1;
//            }
//
//            UpdateConsole(weather_vector, index, table_vec, table_c, document, screen);
//            Sleep(250);
//        }
//        if(GetAsyncKeyState(0xBB) & 0x8000) {
//            count_days++;
//            if(count_days > 16) {
//                count_days = 16;
//            for(auto& elemen_vector: weather_vector) {
//                elemen_vector.SetCountDays(count_days);
//            }
//
//        }
//
//            UpdateConsole(weather_vector, index, table_vec, table_c, document, screen);
//            Sleep(250);
//        }
//        if(GetAsyncKeyState(0xBD) & 0x8000) {
//            count_days--;
//            if(count_days < 1) {
//                count_days = 1;
//            }
//
//            for(auto& elemen_vector: weather_vector) {
//                elemen_vector.SetCountDays(count_days);
//            }
//
//            UpdateConsole(weather_vector, index, table_vec, table_c, document, screen);
//            Sleep(250);
//        }
//        if(GetAsyncKeyState(0x1B) & 0x8000) {
//            is_exit = true;
//        }
//        auto end = std::chrono::system_clock::now();
//        if (std::chrono::duration_cast<std::chrono::minutes>(end - start).count() >= frequency) {
//            for(auto& elemen_vector: weather_vector) {
//                elemen_vector.WeatherUpdate();
//            }
//            UpdateConsole(weather_vector, index, table_vec, table_c, document, screen);
//            start = std::chrono::system_clock::now();
//        }
//    }

    return 0;
}
