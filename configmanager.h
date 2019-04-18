#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H


#include <map>
#include <string>
#include <fstream>
#include <iostream>
/**
 * @brief 配置文件读取类,单例模式,懒汉式
 */
class ConfigManager {
private:
    ConfigManager(std::string filename);
    std::map<std::string, std::string> data_;
    static ConfigManager *instance_;

public:
    static ConfigManager *GetInstance(std::string init_file_path="config.ini");
    std::string get_date(std::string key);
    ~ConfigManager();
};

#endif // CONFIGMANAGER_H
