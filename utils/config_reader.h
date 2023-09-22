#ifndef LAB1_CONFIG_READER_H
#define LAB1_CONFIG_READER_H

#include <string>
#include <stdexcept>
#include <fstream>
#include <unordered_map>

class ConfigReader {
public:
    explicit ConfigReader(const std::string &configFileName) : configFile(configFileName) {
        try {
            std::ifstream configFileStream(configFileName);
            if (!configFileStream.is_open()) {
                throw std::runtime_error("Failed to open config file: " + configFileName);
            }

            std::string line;
            while (std::getline(configFileStream, line)) {
                size_t delimiterPos = line.find('=');
                if (delimiterPos != std::string::npos) {
                    std::string key = line.substr(0, delimiterPos);
                    std::string value = line.substr(delimiterPos + 1);
                    configData[key] = value;
                }
            }
        } catch (const std::exception &ex) {
            throw std::runtime_error("Error reading config file: " + std::string(ex.what()));
        }
    }

    std::string get(const std::string &key) const {
        if (configData.find(key) != configData.end()) {
            return configData.at(key);
        } else {
            throw std::runtime_error("Config key not found: " + key);
        }
    }

private:
    std::unordered_map<std::string, std::string> configData;
    const std::string configFile;
};


#endif //LAB1_CONFIG_READER_H
