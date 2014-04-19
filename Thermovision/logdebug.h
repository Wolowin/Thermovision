#ifndef LOGDEBUG_H
#define LOGDEBUG_H

#define DEBUG_BUILD   //comment if release

#include <iostream>

using namespace std;

#ifdef DEBUG_BUILD
#define LogDebug(msg) cout << __PRETTY_FUNCTION__ << endl << __FILE__ << ":" << __LINE__ << ":" << msg << endl
#else
#define LogDebug(msg)
#endif

#endif // LOGDEBUG_H
