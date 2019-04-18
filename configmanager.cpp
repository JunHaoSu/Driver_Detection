#include "configmanager.h"

using namespace std;

ConfigManager::ConfigManager(std::string filename)
{
    data_.clear();
    ifstream fin(filename.c_str());
    if(!fin)
    {
       cerr << "Can not open config file" << filename <<endl;
       return;
    }
    while(!fin.eof())
    {
        string str;
        getline(fin,str);
        if(str[0] == '#')
            continue;

        int pos = str.find("=");
        if(pos == -1)
            continue;
        string key = str.substr(0,pos);
        string value = str.substr(pos+1, str.length());
        data_[key] = value;
        if(!fin.good())
            break;
    }
    fin.close();
}

ConfigManager::~ConfigManager()
{
    if(instance_ != NULL)
        delete instance_;
}

string ConfigManager::get_date(std::string key)
{
    map<string, string>::const_iterator iter = data_.find(key);
    if(iter == data_.end())
    {
        cerr << "Parameter name " <<key<<" not found!"<<endl;
        return string("NOT_FOUND");
    }
    return iter->second;
}

ConfigManager *ConfigManager::instance_ = NULL;
ConfigManager *ConfigManager::GetInstance(std::string init_file_path)
{
    if(instance_ == NULL)
    {
        instance_ = new ConfigManager(init_file_path);
    }
    return instance_;
}
