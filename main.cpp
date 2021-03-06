#include <iostream>
#include <utility>
#include <curl/curl.h>
#include <cstdio>
#include <vector>
#include <fstream>
#include "json.hpp"
#include <thread>
#include <algorithm>


class Currency {
private:
    std::vector<double> values;
    std::string name;
public:
    Currency(std::vector<double> values, std::string name)
            : values(std::move(values)) {
        this->name = std::move(name);
    }

    Currency(std::string name) : name(std::move(name)), values() {}

    const std::vector<double> &getValues() const {
        return values;
    }

    void addValue(double value) {
        values.push_back(value);
    }

    void setValues(const std::vector<double> &values) {
        Currency::values = values;
    }

    const std::string &getName() const {
        return name;
    }

    void setName(const std::string &name) {
        Currency::name = name;
    }
};

class CurrenciesTable {
private:
    std::vector<Currency> elements;
public:
    CurrenciesTable() : elements() {}

    ~CurrenciesTable(){
        std::cout << "Destructor";
    }

    void addElement(const Currency &currency) {
        int index = -1;

        for (int i = 0; i < elements.size(); ++i) {
            if (elements[i].getName() == currency.getName())
                index = i;
        }
        if (index == -1)
            elements.push_back(currency);
        else
            elements[index].addValue(currency.getValues()[0]);
    }

    void print() {
        for (auto &element: elements) {
            std::cout << element.getName() << ": ";
            for (int j = 0; j < element.getValues().size(); ++j) {
                std::cout << element.getValues()[j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void printMean() {
        for (auto &element: elements) {
            double sum = 0;
            for (double j: element.getValues()) {
                sum += j;
            }
            std::cout << element.getName() << " mean = " << sum / element.getValues().size() << std::endl;
        }
    }

    void printMedian() {
        for (auto &element: elements) {
            std::cout << element.getName() << " median = ";
            std::vector<double> values = element.getValues();
            std::sort(values.begin(), values.end());
            if (element.getValues().size() % 2 != 0)
                std::cout << values[element.getValues().size() / 2] << std::endl;
            else
                std::cout << (values[element.getValues().size() / 2 - 1] + values[element.getValues().size() / 2]) / 2
                          << "\n";
        }
    }
};

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}


CurrenciesTable table;


void thr_func() {
    CURL *curl = curl_easy_init();
    FILE *file = fopen("/Users/maksim/CLionProjects/lab2_cpp/file.js", "wb");

    curl_easy_setopt(curl, CURLOPT_URL, "https://www.cbr-xml-daily.ru/daily_json.js");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

    CURLcode response = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(file);

    std::ifstream input("/Users/maksim/CLionProjects/lab2_cpp/file.js");

    nlohmann::json json_file;
    input >> json_file;

    for (auto currency: json_file["Valute"]) {
        std::vector<double> values;
        values.push_back(currency["Value"]);

        Currency curr(values, currency["Name"]);
        table.addElement(curr);
    }
    std::cout << "Debug" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
}


int main() {
    //
    char character;
    while(true) {
        std::thread thr1 = std::thread(thr_func);
        thr1.join();
        std::cout << "Print E to exit or C to continue" << std::endl;
        std::cin >> character;
        if (character == 'E')
            break;
    }
    table.printMean();
    table.printMedian();
}
