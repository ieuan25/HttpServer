/*
 * Config.h
 *
 *  Created on: 19 Oct 2013
 *      Author: ieuan
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <vector>
#include <map>

class Config {
public:
	Config(const char*);
	virtual ~Config();
	virtual std::map<std::string, std::string> ReadConfig();
	bool IsValid() { return !config_items.empty(); };
	std::map<std::string, std::string> GetConfigItems() {return config_items;};
	void SetConfigItem(std::string key, std::string val);
private:
protected:
	std::vector<std::string> GetConfigLines(const std::string& raw_config);
	static bool CanIgnoreLine(const std::string& line);
	char path[64];
	std::map<std::string, std::string> config_items;

};

#endif /* CONFIG_H_ */
