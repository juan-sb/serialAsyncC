#include <windows.h>
#include <tchar.h>
#include <stdio.h>

void PrintCommState(DCB dcb)
{
    //  Print some of the DCB structure values
    _tprintf( TEXT("\nBaudRate = %d, ByteSize = %d, Parity = %d, StopBits = %d\n"), 
              dcb.BaudRate, 
              dcb.ByteSize, 
              dcb.Parity,
              dcb.StopBits );
}


int _tmain( int argc, TCHAR *argv[] )
{
   DCB dcb;
   HANDLE comm;
   BOOL success;
   TCHAR *puerto = TEXT("COM6"); //  Most systems have a COM1 port

   //  Open a handle to the specified com port.
   comm = CreateFile( puerto,
                      GENERIC_READ | GENERIC_WRITE,
                      0,      //  must be opened with exclusive-access
                      NULL,   //  default security attributes
                      OPEN_EXISTING, //  must use OPEN_EXISTING
                      0,      //  not overlapped I/O
                      NULL ); //  hTemplate must be NULL for comm devices

   if (comm == INVALID_HANDLE_VALUE) 
   {
       //  Handle the error.
       printf ("CreateFile failed with error %d.\n", GetLastError());
       return (1);
   }

   //  Initialize the DCB structure.
   SecureZeroMemory(&dcb, sizeof(DCB));
   dcb.DCBlength = sizeof(DCB);

   //  Build on the current configuration by first retrieving all current
   //  settings.
   success = GetCommState(comm, &dcb);

   if (!success) 
   {
      //  Handle the error.
      printf ("GetCommState failed with error %d.\n", GetLastError());
      return (2);
   }

   PrintCommState(dcb);       //  Output to console

   //  Fill in some DCB values and set the com state: 
   //  57,600 bps, 8 data bits, no parity, and 1 stop bit.
   dcb.BaudRate = 74880;     //  baud rate
   dcb.ByteSize = 8;             //  data size, xmit and rcv
   dcb.Parity   = NOPARITY;      //  parity bit
   dcb.StopBits = ONESTOPBIT;    //  stop bit

   success = SetCommState(comm, &dcb);

   if (!success) 
   {
      //  Handle the error.
      printf ("SetCommState failed with error %d.\n", GetLastError());
      return (3);
   }

   //  Get the comm config again.
   success = GetCommState(comm, &dcb);

   if (!success) 
   {
      //  Handle the error.
      printf ("GetCommState failed with error %d.\n", GetLastError());
      return (2);
   }

   PrintCommState(dcb);       //  Output to console

   _tprintf (TEXT("Serial port %s successfully reconfigured.\n"), puerto);
   
   SetCommMask(comm, EV_TXEMPTY|EV_RXCHAR);
   DWORD bytesToWrite = 4;
   LPCVOID buf = "HOLA";
   LPDWORD bytesWritten;
   LPOVERLAPPED lpOverlapped = NULL;
   HLOCAL eto;
   
   printf("Hello");
   if (!WriteFile (comm, // Port handle,
   					buf,
					bytesToWrite, // Number of bytes to write
					bytesWritten, // Pointer to the number of bytes written
					lpOverlapped)) // Must be NULL for Windows CE
	{
		return 1;
		// WriteFile failed. Report error.
		printf("Unable to write the port ...& Err Info. = %s", (LPCTSTR)eto);
		// Free the buffer.
		LocalFree( eto );
	}
	else
		printf("\nSe escribieron %d caracteres al puerto", bytesWritten);
   
	return (0);
}
