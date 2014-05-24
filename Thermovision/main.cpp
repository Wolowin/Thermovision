#include <QApplication>
#include <View/mainwindow.h>
#include <uEye.h>
#include <boost/thread.hpp>
#include <Model/thermo_camera_model.h>
#include <qpa/qplatformnativeinterface.h>
#include "log.h"
#include "QLabel"

using namespace std;



//static QWindow* windowForWidget(const QWidget* widget)
//{
//	if (QWindow* window = widget->windowHandle())
//	{
//		cout << "1" << endl;
//		return window;
//	}
//	if (const QWidget* nativeParent = widget->nativeParentWidget())
//	{
//		cout << "2" << endl;
//		return nativeParent->windowHandle();
//	}
//	cout << "3" << endl;
//	return 0;
//}

//HWND getHWNDForWidget(const QWidget* widget)
//{
//	if (QWindow* window = ::windowForWidget(widget))
//	{
//		cout << "4" << endl;
//		if (window->handle())
//		{
//		cout <<(QGuiApplication::platformNativeInterface()->nativeResourceForWindow(QByteArrayLiteral("handle"), window)) << endl;
//			return static_cast<HWND>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow(QByteArrayLiteral("handle"), window));
//		}
//	}
//return 0;
//}


void workerFunc(char* ptr)
{
	ptr += 3000;
	while (1)
	{
	cout << (unsigned int) *(ptr) << endl;
	Sleep(1000);
	}
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	thermo_camera_model qwerty;


	qwerty.qwe();

		QImage myImage(reinterpret_cast <uchar*>(qwerty.raw_image_data_pointer), 768, 576, QImage::QImage::Format_RGB32	);
		QLabel myLabel;

//		QPixmap vpix;
//		vpix.convertFromImage(myImage);
//		myLabel.setPixmap(vpix);

		myLabel.setPixmap(QPixmap::fromImage(myImage));
		myLabel.show();

//		for (int i=0 ; i < 4000 ; i ++, qwerty.raw_image_data_pointer++)
//		{
//			cout << (unsigned int) *(qwerty.raw_image_data_pointer) << endl;
//		}
boost::thread workerThread(workerFunc, qwerty.raw_image_data_pointer);
	return a.exec();
}


//#include <windows.h>

//LPCWSTR NazwaKlasy = reinterpret_cast<const wchar_t*>("Klasa Okienka");
//MSG Komunikat;

//LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{

//	// WYPE£NIANIE STRUKTURY
//	WNDCLASSEX wc;

//	wc.cbSize = sizeof(WNDCLASSEX);
//	wc.style = 0;
//	wc.lpfnWndProc = WndProc;
//	wc.cbClsExtra = 0;
//	wc.cbWndExtra = 0;
//	wc.hInstance = hInstance;
//	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//	wc.lpszMenuName = NULL;
//	wc.lpszClassName = NazwaKlasy;
//	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

//	// REJESTROWANIE KLASY OKNA
//	if (!RegisterClassEx(&wc))
//	{
//		MessageBoxA(NULL, "Wysoka Komisja odmawia rejestracji tego okna!", "Niestety...",
//			MB_ICONEXCLAMATION | MB_OK);
//		return 1;
//	}

//	// TWORZENIE OKNA
//	HWND hwnd;

//	hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, NazwaKlasy, reinterpret_cast<const wchar_t*>("Oto okienko"), WS_OVERLAPPEDWINDOW,
//	CW_USEDEFAULT, CW_USEDEFAULT, 1000, 1000, NULL, NULL, hInstance, NULL );

//	if (hwnd == NULL)
//	{
//		MessageBoxA(NULL, "Okno odmówi³o przyj¶cia na ¶wiat!", "Ale kicha...", MB_ICONEXCLAMATION);
//		return 1;
//	}

//	ShowWindow(hwnd, nCmdShow); // Poka¿ okienko...
//	UpdateWindow(hwnd);

//		thermo_camera_model qwerty;
//		qwerty.test = hwnd;
//		qwerty.qwe();

//	// Pêtla komunikatów
//	while (GetMessage(&Komunikat, NULL, 0, 0))
//	{
//		TranslateMessage(&Komunikat);
//		DispatchMessage(&Komunikat);
//	}
//	return Komunikat.wParam;
//}

//// OBS£UGA ZDARZEÑ
//LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	switch (msg)
//	{
//	case WM_CLOSE:
//		DestroyWindow(hwnd);
//		break;

//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;

//	default:
//		return DefWindowProc(hwnd, msg, wParam, lParam);
//	}

//	return 0;
//}



