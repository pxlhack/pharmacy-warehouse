#ifndef LAB1_CONFIG_READER_H
#define LAB1_CONFIG_READER_H

#include <string>
#include <stdexcept>
#include <fstream>
#include <unordered_map>

using namespace std;

class ConfigReader {
public:
    explicit ConfigReader(const string &configFileName) : configFile(configFileName) {
        try {
            ifstream configFileStream(configFileName);
            if (!configFileStream.is_open()) {
                throw runtime_error("Failed to open config file: " + configFileName);
            }

            string line;
            while (getline(configFileStream, line)) {
                size_t delimiterPos = line.find('=');
                if (delimiterPos != string::npos) {
                    string key = line.substr(0, delimiterPos);
                    string value = line.substr(delimiterPos + 1);
                    configData[key] = value;
                }
            }
        } catch (const exception &ex) {
            throw runtime_error("Error reading config file: " + string(ex.what()));
        }
    }

    string get(const string &key) const {
        if (configData.find(key) != configData.end()) {
            return configData.at(key);
        } else {
            throw runtime_error("Config key not found: " + key);
        }
    }

private:
    unordered_map<string, string> configData;
    const string configFile;
};


#endif //LAB1_CONFIG_READER_H
