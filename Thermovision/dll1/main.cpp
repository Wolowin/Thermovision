#include <iostream>
#include "../DLLApi/base.h"

using namespace std;

class coutuje2 : public base
{
	virtual void coutuj();
};

void coutuje2::coutuj()
{
	cout << "2" << endl;
}

extern "C"
{
	   // Plugin factory function
	   __declspec(dllexport) base* Create_cout ()
	   {
			  //allocate a new object and return it
			  return new coutuje2();
	   }

//       // Plugin cleanup function
//       __declspec(dllexport) void Release_Plugin (IPlugin* p_plugin)
//       {
//              //we allocated in the factory with new, delete the passed object
//              delete p_plugin;
//       }

}


