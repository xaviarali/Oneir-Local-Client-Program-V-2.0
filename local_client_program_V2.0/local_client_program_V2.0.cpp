// local_client_program_V2.0.cpp : Defines the entry point for the console application.
//

// oneir_local_client.cpp : Defines the entry point for the console application.
//
// header files
#include "stdafx.h"

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <io.h>
#include<iostream>
#include <stdio.h>

#define _CRT_SECURE_NO_WARNINGS
#include<string.h>
#include<time.h>
#include<winsock2.h>
#include<ws2tcpip.h>
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#pragma comment(lib,"Ws2_32.lib")
#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "80"
#define path "C:\\oneirV1.0\\"
#define path_to_ini_file  "C:\\oneirV1.0\\credential.ini"
#define telnet98 "C:\\oneirV1.0\\Dev3-Cenedex.etx"
#define terminal "C:\\oneirV1.0\\terminal.etx"
#define username 0
#define session_file_1 1
#define session_file_2 2
#define session_file_3 3
#define CREDENTIAL_SIZE_LIMIT 4
#define CREDENTIAL_SPACE 100
#define REMOTE_HOST_IP "sleepy-mesa-60780.herokuapp.com"
using namespace std;
// global variable
int choice = 0;
int CREDENTIALS_SIZE = 0;
char* credentials[CREDENTIAL_SIZE_LIMIT];
// Macros
#define _ETNAPI_H
#define ETNBUFSIZ       0x2000
HWND erik98 = NULL;

// aliases for function pointers
typedef VOID(CALLBACK* ETNDATAPROC)(HWND, LPBYTE, DWORD);
typedef BOOL(__stdcall *etnEnableInput)(HWND hSession, BOOL bEnable);
typedef BOOL(__stdcall *etnIsSession)(HWND hSession);
typedef HWND(__stdcall *etnObtainHandle)(LPCTSTR lpSessionName, DWORD dwMilliseconds);
typedef BOOL(__stdcall *etnReceiveData)(HWND hSession, LPBYTE lpData, LPDWORD lpcbData, DWORD dwMilliseconds);
typedef BOOL(__stdcall *etnRegisterCallback)(HWND hSession, ETNDATAPROC lpDataProc);
typedef BOOL(__stdcall *etnReleaseHandle)(HWND hSession);
typedef BOOL(__stdcall *etnSendData)(HWND hSession, LPBYTE lpData, DWORD cbData);
typedef BOOL(__stdcall *etnSetDelay)(DWORD dwMilliseconds);

// loads the .DLL provided by Erik's Telnet98
static HANDLE GetEtnApi(void)
{
	static HANDLE hDLL = NULL;
#if _WIN64
	if (!hDLL)
		hDLL = LoadLibrary(L"ETNAPI64.DLL");
#else
	if (!hDLL)
		hDLL = LoadLibrary(L"ETNAPI32.DLL");
#endif
	if (!hDLL) {
		MessageBox(NULL, L"Failed to load ETNAPI", (LPCWSTR)__FILE__, MB_ICONSTOP);
		exit(1);
	} // if
	return hDLL;
} // GetEtnApi


  // wrapper function for etnEnableInput which is loaded from the .DLL file
BOOL WINAPI xetnEnableInput(HWND hSession, BOOL bEnable)
{
	//static BOOL(STDAPICALLTYPE *etnEnableInput)(HWND, BOOL) = NULL;
	static etnEnableInput x = NULL;
	if (!x)
		x = (etnEnableInput)GetProcAddress((HMODULE)GetEtnApi(), "etnEnableInput");
	if (x)
		return x(hSession, bEnable);
	return FALSE;
} // etnEnableInput


  // wrapper function for etnIsSession which is loaded from the .DLL file
BOOL WINAPI xetnIsSession(HWND hSession)
{
	//static BOOL(STDAPICALLTYPE *etnIsSession)(HWND) = NULL;
	static etnIsSession x = NULL;
	if (!x)
		x = (etnIsSession)GetProcAddress((HMODULE)GetEtnApi(), "etnIsSession");
	if (x)
		return x(hSession);
	return FALSE;
} // etnIsSession


  // wrapper function for etnObtainHandle which is loaded from the .DLL file
HWND WINAPI xetnObtainHandle(LPCTSTR lpSessionName, DWORD dwMilliseconds)
{
	//static HWND(STDAPICALLTYPE *etnObtainHandle)(LPCTSTR, DWORD) = NULL;
	static etnObtainHandle x = NULL;
	if (!x)
		x = (etnObtainHandle)GetProcAddress((HMODULE)GetEtnApi(), "etnObtainHandle");
	if (x)
		return x(lpSessionName, dwMilliseconds);
	return NULL;
} // etnObtainHandle


  // wrapper function for etnReceiveData which is loaded from the .DLL file
BOOL WINAPI xetnReceiveData(HWND hSession, LPBYTE lpData, LPDWORD lpcbData, DWORD dwMilliseconds)
{
	//static BOOL(STDAPICALLTYPE *etnReceiveData)(HWND, LPBYTE, LPDWORD, DWORD) = NULL;
	static etnReceiveData x = NULL;
	if (!x)
		x = (etnReceiveData)GetProcAddress((HMODULE)GetEtnApi(), "etnReceiveData");
	if (x)
		return x(hSession, lpData, lpcbData, dwMilliseconds);
	return FALSE;
} // etnReceiveData


  // wrapper function for etnRegisterCallback which is loaded from the .DLL file
BOOL WINAPI xetnRegisterCallback(HWND hSession, ETNDATAPROC lpDataProc)
{
	//static BOOL(STDAPICALLTYPE *etnRegisterCallback)(HWND, ETNDATAPROC) = NULL;
	static etnRegisterCallback x = NULL;
	if (!x)
		x = (etnRegisterCallback)GetProcAddress((HMODULE)GetEtnApi(), "etnRegisterCallback");
	if (x)
		return x(hSession, lpDataProc);
	return FALSE;
} // etnRegisterCallback


  // wrapper function for etnReleaseHandle which is loaded from the .DLL file
BOOL WINAPI xetnReleaseHandle(HWND hSession)
{
	//static BOOL(STDAPICALLTYPE *etnReleaseHandle)(HWND) = NULL;
	static etnReleaseHandle x = NULL;
	if (!x)
		x = (etnReleaseHandle)GetProcAddress((HMODULE)GetEtnApi(), "etnSetDelay");
	if (x)
		return x(hSession);
	return FALSE;
} // etnReleaseHandle


  // wrapper function for etnSendData which is loaded from the .DLL file
BOOL WINAPI xetnSendData(HWND hSession, LPBYTE lpData, DWORD cbData)
{
	//static BOOL(STDAPICALLTYPE *etnSendData)(HWND, LPBYTE, DWORD) = NULL;
	static etnSendData x = NULL;
	if (!x)
		x = (etnSendData)GetProcAddress((HMODULE)GetEtnApi(), "etnSendData");
	if (x)
		return x(hSession, lpData, cbData);
	return FALSE;
} // etnSendData


  // wrapper function for etnSetDelay which is loaded from the .DLL file
VOID WINAPI xetnSetDelay(DWORD dwMilliseconds)
{
	//static VOID(STDAPICALLTYPE *etnSetDelay)(DWORD) = NULL;
	static etnSetDelay x = NULL;
	if (!x)
		x = (etnSetDelay)GetProcAddress((HMODULE)GetEtnApi(), "etnSetDelay");
	if (x)
		x(dwMilliseconds);
} // etnSetDelay

  /*
  *  temporary function which extracts a character from a char* and store it into global variable 'choice'
  */
void check(char* s)
{
	
	if (s[96] >= '1' && s[96] <= '9')
	{
		printf("Command Recieved from the Cloud.\n");
		choice = (int)(s[96] - '0');
	}
}
/*
* client which sends a session Id to the server and recives a JSON data in responce
*/
int client()
{
	// username hasnt set
	if (credentials[username] == NULL)
	{
		printf("Username couldnt be read.\n");
		return -23;
	}
	const int request_line = 150;
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char message[request_line], recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}
	
	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	// 54.235.98.223:443
	server.sin_addr.s_addr = inet_addr("54.235.98.223");
	server.sin_family = AF_INET;
	server.sin_port = htons(80);

	//Connect to remote server
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}
	//Send some data
	sprintf_s(message,"GET /oneir?q=%s HTTP/1.1\r\nHost: %s\r\n\r\n",credentials[username],REMOTE_HOST_IP);
	if (send(s, message, strlen(message), 0) < 0)
	{
		puts("Send failed");
		return 1;
	}

	//Receive a reply from the server
	int iResult = 0;
	// Receive until the peer closes the connection
	int i = 0;
	while((iResult = recv(s, recvbuf, recvbuflen, 0)) > 0)
	{
		recvbuf[iResult - 1] = '\0'; break;
	}
	check(recvbuf);
}

bool sendDataToTelnet98()
{
	HWND    hSession;
	DWORD   cbData = 0;
	char    buf[ETNBUFSIZ];

	/*
	* increase this value if your application
	* is too slow to fetch all incoming data
	*/
	etnSetDelay(10);

	/*
	*  Hard Coded Data which is being sent to the telnet98 session
	*/
	if (hSession = xetnObtainHandle(NULL, (DWORD)1000)) {
		xetnEnableInput(hSession, FALSE);
		strncpy_s(buf, sizeof(buf), "1", 1);
		char arsd[] = "./runwin ./vigilant 2>$USER.log COMP=tutorial DATE=021231 PASS=DEMO DV=HO PROG=ARSD\r";
		char insd[] = "./runwin ./vigilant 2>$USER.log COMP=tutorial DATE=021231 PASS=DEMO DV=HO PROG=INSD\r";
		switch (choice)
		{
		case 1:
			xetnSendData(hSession, (LPBYTE)"1", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"D", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"E", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"M", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"O", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"\r", (DWORD)strlen(buf));
			break;
		case 2:
			xetnSendData(hSession, (LPBYTE)"1", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"D", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"E", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"M", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"O", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"\r", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"4", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"4", (DWORD)strlen(buf));
			break;

		case 3:
			xetnSendData(hSession, (LPBYTE)"1", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"D", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"E", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"M", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"O", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"\r", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"5", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"4", (DWORD)strlen(buf));
			break;


		case 4:
			xetnSendData(hSession, (LPBYTE)"1", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"D", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"E", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"M", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"O", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"\r", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"5", (DWORD)strlen(buf));
			break;
		case 5:
			xetnSendData(hSession, (LPBYTE)arsd, (DWORD)strlen(arsd));
			break;

		case 6:
			xetnSendData(hSession, (LPBYTE)insd, (DWORD)strlen(insd));
			break;
		}

		xetnEnableInput(hSession, TRUE);
		return true;
	} // if
	printf("No Telnet98 Running!\n");
	return false;
}

// check whether telnet98 session is running
bool isTelnetReady()
{
	if (xetnObtainHandle(NULL, (DWORD)10000) == NULL) return 0;
	return 1;
}

// wait telnet to load , time may vary with the system
void wait(unsigned timeout)
{
	timeout += clock();
	while (clock() < timeout) continue;
}

// Get the horizontal and vertical screen sizes in pixel
void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

void resizeApps(int v, int h)
{
	//HWND chrome = GetWindow(chromeWindow,GW_HWNDNEXT);
	//tn98FrameClass
	//MozillaWindowClass
	//Chrome_WidgetWin_1
	// try 0x0200 instead of 0x0400  SWP_NOZORDER
	// for google chrome
	HWND Window = NULL;

	Window = FindWindowEx(0, Window, L"tn98FrameClass", 0);
	if (Window != NULL)
		SetWindowPos(Window, HWND_TOPMOST, (h * 25) / 100, 0, (h * 75) / 100, v - 50, 0x0400);

	Window = xetnObtainHandle(NULL, (DWORD)1000);
	if (Window != NULL)
		SetWindowPos(Window, HWND_TOPMOST, (h * 25) / 100, 0, (h * 75) / 100, v - 50, 0x0400);
}

// grab user's credential from credentials .ini
void readCredentials(char** array)
{
   
	FILE* credential_file = fopen(path_to_ini_file, "r");
	if (credential_file == NULL)
	{
		printf("Error reading credentials.Please check credential.ini file.");
		return;
	}
      
	for (int j = 1; j < CREDENTIAL_SIZE_LIMIT; ++j)
		strcpy(array[j], path);

	int i = 0;
	char buffer[1000];
	while (fscanf(credential_file, "%s\n", buffer) != EOF && i < CREDENTIAL_SIZE_LIMIT)
	{
		if (strlen(buffer) + strlen(path) > CREDENTIAL_SPACE - 1 )
		{
			printf("Too long filename size in credential.ini.\n");
			return;
		}

		if (i != 0)
			strcat(array[i], buffer);
		else
			strcpy(array[i], buffer);

		// for debugging print data
		//printf("%s\n\t\t\tLenght=%d\n\n", array[i],strlen(array[i]));
		// increment the counter
		i++;
	}
	CREDENTIALS_SIZE = i;
	fclose(credential_file);
}

// intialize gloabal array for storing Credentials i.e username, session file names etc
void initCredentials(char** array)
{
	for (int i = 0; i < CREDENTIAL_SIZE_LIMIT; i++)
		array[i] = new char[100];

}
int main(void)
{
	int vert, horz, taskbar_sz;
	GetDesktopResolution(horz, vert);
	initCredentials(credentials);
	readCredentials(credentials);
	printf("\n\t\t\tSession Id: %s\n", credentials[username], CREDENTIALS_SIZE);
	while (true)
	{
		resizeApps(vert, horz);
		Sleep(3200);
		//readCredentials(credentials);
		client();
		//end of server
		if (choice != 0)
		{
			//Sleep(25);
			if (choice == 5 || choice == 6) system(terminal);
			else system(telnet98);

			resizeApps(vert, horz);

			printf("Waiting For Telnet To load\n");
			while (!isTelnetReady());
			Sleep(3200);
			system("COLOR 2");
			printf("Time Ended For Telnet To load\n");
			sendDataToTelnet98();
			choice = 0;
			erik98 = NULL;
		}
	}
	return 0;
} // main