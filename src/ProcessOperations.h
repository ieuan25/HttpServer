/*
 * ProcessOperations.h
 *
 *  Created on: 5 May 2014
 *      Author: ieuan
 */

#ifndef PROCESSOPERATIONS_H_
#define PROCESSOPERATIONS_H_

#include <signal.h>
extern volatile sig_atomic_t children_terminated;

class ProcessOperations {
public:
	ProcessOperations();
	static void RecoverTerminatedChildren();
	static void Daemonise();
	static int ForkNewProcess();
	virtual ~ProcessOperations();
};

#endif /* PROCESSOPERATIONS_H_ */
