#include <iostream>
#include "base.h"
#include <windows.h>


using namespace std;

typedef base* (*cout_factory)();

int main()
{
	base *p_base;

	HMODULE h_mod = ::LoadLibrary ("C:\\Users\\Kornik\\Documents\\PRACA MAGISTERSKA\\Program\\Thermovision\\Thermovision\\build-dll1-Desktop_Qt_5_2_1_MinGW_32bit-Release\\release\\dll1.dll");

	if (h_mod != NULL)
	{
		cout << "jest dobrze" << endl;

		cout_factory p_factory_function =
	   (cout_factory) ::GetProcAddress (h_mod, "Create_cout");

		p_base = p_factory_function();

		p_base->coutuj();
	}

	system("pause");
	return 0;
}

