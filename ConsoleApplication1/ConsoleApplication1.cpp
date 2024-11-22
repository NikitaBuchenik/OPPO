#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <regex>

using namespace std;

struct MeterReading {
    string resourceType;
    string date;
    double value;
};

std::ostream& operator<<(std::ostream& stream, const MeterReading& reading);
static vector<string> extractData(const string& data, const string& regex);
static string getResourceType(string s);
static string getDate(string s);
static double getValue(string s);

int main() {
    ifstream ist("in.txt");

    vector<MeterReading> readings;
    string s;

    while (getline(ist, s)) {
        string resourceType = getResourceType(s);
        string date = getDate(s);
        double value = getValue(s);

        readings.push_back({ resourceType, date, value });
    }

    for (const auto& item : readings) {
        cout << item << "\n";
    }
}

string getResourceType(string s) {
    auto resourceType = extractData(s, R"([a-zA-Z\s]+)"); 
    if (resourceType.empty()) {
        throw runtime_error("Resource type not found in the input line.");
    }
    return resourceType.at(0);
}

string getDate(string s) {
    auto date = extractData(s, R"(\d{4}\.\d{2}\.\d{2})");
    if (date.size() != 1) {
        throw runtime_error("Date not found or invalid in the input line.");
    }
    return date.at(0);
}

double getValue(string s) {
    auto value = extractData(s, R"(\d+,\d{2})");
    if (value.size() != 1) {
        throw runtime_error("Value not found or invalid in the input line.");
    }
    string valueStr = value.at(0);
    replace(valueStr.begin(), valueStr.end(), ',', '.');
    return stod(valueStr);
}

vector<string> extractData(const string& data, const string& regex) {
    std::regex words_regex(regex);
    auto words_begin = sregex_iterator(data.begin(), data.end(), words_regex);
    auto words_end = sregex_iterator();

    vector<string> out;
    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        out.push_back(i->str());
    }
    return out;
}

std::ostream& operator<<(std::ostream& stream, const MeterReading& reading) {
    stream << "Resource Type: " << reading.resourceType;
    stream << ", Date: " << reading.date;
    stream << ", Value: " << reading.value;
    return stream;
}
