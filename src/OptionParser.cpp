/*
 * OptionParser.cpp
 *
 *  Created on: 5 May 2014
 *      Author: ieuan
 */

#include "OptionParser.h"
#include "StringOperations.h"
#include <unistd.h>
#include <getopt.h>
#include <cstring>
#include <iostream>

OptionParser::OptionParser(int argc, char* argv[]) : _argc(argc), argv(argv) {}

int OptionParser::Parse(http_options& options)
{
	int ret;
	opterr = 0;

	memset(&options, '\0', sizeof(options));

	static struct option long_options[] = {
	            {"help",     no_argument, NULL,  'h' },
	            {"version",  no_argument, NULL,  'v' },
	            {"daemon", required_argument, NULL, 'd' },
	            {"confpath", required_argument, NULL, 'c' },
	            {"portnum", required_argument, NULL, 'p'},
	            {"maxcons", required_argument, NULL, 'm'},
	            {"htdocs", required_argument, NULL, 't'},
	            {"errors", required_argument, NULL, 'e'}
	        };

	while ((ret = getopt_long(_argc, argv, "", long_options, NULL)) != -1)
	{
		switch(ret)
		{
			case 'v':
				options.version = 1;
				break;
			case 'h':
				options.help = 1;
				break;
			case 'd':
				strcpy(options.daemon, optarg);
				break;
			case 'c':
				strcpy(options.conf_path, optarg);
				break;
			case 'p':
				strcpy(options.port_num, optarg);
				break;
			case 'm':
				strcpy(options.max_cons, optarg);
				break;
			case 't':
				strcpy(options.ht_docs, optarg);
				break;
			case 'e':
				strcpy(options.errors, optarg);
				break;
			case '?':
				return -1;
			default:
				break;
		}
	}
	return 0;
}

OptionParser::~OptionParser() {
	// TODO Auto-generated destructor stub
}
