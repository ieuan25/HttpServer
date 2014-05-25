/*
 * MimeTypes.h
 *
 *  Created on: 20 Oct 2013
 *      Author: ieuan
 */

#ifndef MIMETYPES_H_
#define MIMETYPES_H_

#include <string>
#include <vector>
#include "Config.h"


class MimeTypeConf : public Config {
public:
	MimeTypeConf(const char * path, http_options& opts);
	~MimeTypeConf();
	std::map<std::string, std::string> ReadConfig();
protected:
private:
	void MapFileExtensionsToMimeTypes(const std::vector<std::string> &file_extensions,const std::string& mt);
};

#endif /* MIMETYPES_H_ */
