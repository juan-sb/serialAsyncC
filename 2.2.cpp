#include <Windows.h>
#include <stdio.h>


HANDLE hPort;
DCB PortDCB;


char *lpszPortName = "COM5";
char lpMsgBuf[1000];


int main(){
	
	// Open the serial port.
	hPort = CreateFile(lpszPortName, // Pointer to the name of the port
						GENERIC_READ | GENERIC_WRITE, // Access (read/write) mode
						0, // Share mode
						NULL, // Pointer to the security attribute
						OPEN_EXISTING, // How to open the serial port
						0, // Port attributes
						NULL); // Handle to port with attribute to copy
						
	// If it fails to open the port, return error.
	if ( hPort == INVALID_HANDLE_VALUE )
	{
		CloseHandle(hPort); // Could not open the port.
		LocalFree( lpMsgBuf ); // Free the buffer.
		return EC_FOPEN;
	}
	
	PortDCB.DCBlength = sizeof(DCB);
	GetCommState(hPort, &PortDCB); // Get the default port setting information.
	
	// Change the DCB structure settings.
	PortDCB.BaudRate = 9600; // Current baud rate
	PortDCB.fBinary = TRUE; // Binary mode; no EOF check
	PortDCB.fParity = TRUE; // Enable parity checking.
	PortDCB.fOutxCtsFlow = FALSE; // No CTS output flow control
	PortDCB.fOutxDsrFlow = FALSE; // No DSR output flow control
	PortDCB.fDtrControl = DTR_CONTROL_ENABLE; // DTR flow control type
	PortDCB.fDsrSensitivity = FALSE; // DSR sensitivity
	PortDCB.fTXContinueOnXoff = TRUE; // XOFF continues Tx
	PortDCB.fOutX = FALSE; // No XON/XOFF out flow control
	PortDCB.fInX = FALSE; // No XON/XOFF in flow control
	PortDCB.fErrorChar = FALSE; // Disable error replacement.
	PortDCB.fNull = FALSE; // Disable null stripping.
	PortDCB.fRtsControl = RTS_CONTROL_ENABLE; // RTS flow control
	PortDCB.fAbortOnError = FALSE; // Do not abort reads/writes on error
	PortDCB.ByteSize = 8; // Number of bits/bytes, 4-8
	PortDCB.Parity = NOPARITY; // 0-4=no,odd,even,mark,space
	PortDCB.StopBits = ONESTOPBIT; // 0,1,2 = 1, 1.5, 2
	
	// Configure the port according to the specifications of the DCB structure.
	if (!SetCommState (hPort, &PortDCB))
	{
		 printf("Unable to configure the port & Err Info. = %s\n", (LPCTSTR)lpMsgBuf);
		 // Free the buffer.
		 LocalFree( lpMsgBuf );
		 return EC_INVAL_CONFIG;
	}
	// Retrieve the time-out parameters for all read and write operations on the port.
	GetCommTimeouts(hPort, &CommTimeouts);
	// Change the COMMTIMEOUTS structure settings.
	CommTimeouts.ReadIntervalTimeout = MAXDWORD;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 10;
	CommTimeouts.WriteTotalTimeoutConstant = 1000;
	
	// Set the time-out parameters for all read and write operations on the port.
	if (!SetCommTimeouts (hPort, &CommTimeouts))
	{
	// Could not create the read thread.
		dwError = GetLastError();
		printf("Unable to set the time-out parameters & Err code = %ld\n", dwError);
		return EC_TIMEOUT_SET;
	}
	
	EscapeCommFunction(hPort, SETDTR);
	EscapeCommFunction(hPort, SETRTS);
	
	pCreate -> h_Port = hPort; // reserve the port handle
}
