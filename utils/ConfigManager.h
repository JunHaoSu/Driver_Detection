//
// Created by sujunhao on 19-3-24.
//

#ifndef DRIVER_DETECTION_CONFIGMANAGER_H
#define DRIVER_DETECTION_CONFIGMANAGER_H

#include <map>
#include <string>
#include <fstream>
#include <iostream>

class ConfigManager {
private:
    ConfigManager(std::string filename);
    std::map<std::string, std::string> data_;
    static ConfigManager *instance_;

public:
    static ConfigManager *GetInstance(std::string init_file_path="../res/config.ini");
    std::string get_date(std::string key);
    ~ConfigManager();

};


#endif //DRIVER_DETECTION_CONFIGMANAGER_H
