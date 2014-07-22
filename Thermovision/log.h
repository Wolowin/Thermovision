#ifndef LOG_H
#define LOG_H

#define DEBUG_BUILD   //comment if release

#include <iostream>

using namespace std;

#ifdef DEBUG_BUILD
#define log_debug(msg) cout << __PRETTY_FUNCTION__ << endl << __FILE__ << ":" << __LINE__ << ":" << msg << endl << endl
#else
#define LogDebug(msg)
#endif

#define log_error(msg) cout << "ERROR" << endl << __PRETTY_FUNCTION__ << endl << __FILE__ << ":" << __LINE__ << ":" << msg << endl ; assert(false)

#endif // LOG_H
