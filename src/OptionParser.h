/*
 * OptionParser.h
 *
 *  Created on: 5 May 2014
 *      Author: ieuan
 */

#ifndef OPTIONPARSER_H_
#define OPTIONPARSER_H_

typedef struct {
	int version;
	int help;
	char daemon[32];
	char portnum[32];
	char conf_path[128];
	char port_num[32];
	char max_cons[32];
	char ht_docs[32];
	char errors[32];
} http_options;

class OptionParser {
public:
	OptionParser(int argc, char* argv[]);
	int _argc;
	char **argv;
	int Parse(http_options& opts);
	virtual ~OptionParser();
};

#endif /* OPTIONPARSER_H_ */
