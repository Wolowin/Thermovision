#include <iostream>
#include <windows.h>

using namespace std;

int main()
{

	HANDLE hSerial;
	hSerial = CreateFile(
		"COM3",
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);
	if(hSerial==INVALID_HANDLE_VALUE)
	{
		cout << "1" << endl;
		if(GetLastError()==ERROR_FILE_NOT_FOUND)
		{
					cout << "2" << endl;
			//serial port does not exist. Inform user.
		}
		//some other error occurred. Inform user.
	}


	DCB dcbSerialParams = {0};
	dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
	if (!GetCommState(hSerial, &dcbSerialParams))
	{
		//error getting state
	}
	dcbSerialParams.BaudRate=CBR_19200;
	dcbSerialParams.ByteSize=8;
	dcbSerialParams.StopBits=ONESTOPBIT;
	dcbSerialParams.Parity=NOPARITY;
	if(!SetCommState(hSerial, &dcbSerialParams))
	{
		//error setting serial port state
	}
	return 0;
}

