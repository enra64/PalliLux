#include "windowsserial.h"

#include <string>

using namespace std;

void WindowsSerial::waitForData() const
{
	COMSTAT status;
	do {
		DWORD errors;

		// Use the ClearCommError function to get status info on the Serial port
		ClearCommError(mSerialHandle, nullptr, &status);
	}
	// Check if there is something to read
	while (status.cbInQue <= 0);
	// do busy-waiting as long as i cant fix WaitCommEvent never returning :/
	return;
	DWORD dwEventMask;
	
	// set the comm mask to wait for data-set-ready state change
	if (!SetCommMask(mSerialHandle, EV_RXCHAR))
		throw SerialException("SetCommMask failed: error code " + to_string(GetLastError()));

	// wait for any character
	if (!WaitCommEvent(mSerialHandle, &dwEventMask, nullptr)) {
		DWORD lastError = GetLastError();
		throw SerialException("error " + to_string(lastError) + " while waiting for serial event");
	}
		
}

void WindowsSerial::send(const uint8_t* buf, size_t len) const
{
	DWORD bytesSent;

	// Try to write the buffer on the Serial port
	if (!WriteFile(mSerialHandle, buf, len, &bytesSent, nullptr))
	{
		// In case it don't work get comm error
		//ClearCommError(mSerialHandle, &errors, &status);
		throw SerialException("could not write");
	}
}

size_t WindowsSerial::receive(uint8_t* buf, size_t len) const
{
	COMSTAT status;
	DWORD errors;

	// Number of bytes we'll have read
	DWORD bytesRead;

	// Use the ClearCommError function to get status info on the Serial port
	ClearCommError(mSerialHandle, &errors, &status);

	// Check if there is something to read
	if (status.cbInQue <= 0)
		return status.cbInQue;

	// read only as much as available, and dont read more than our buffer can handle
	unsigned int bytesToRead = status.cbInQue < len ? status.cbInQue : len;

	// Try to read the require number of chars, and return the number of read bytes on success
	if (ReadFile(mSerialHandle, buf, bytesToRead, &bytesRead, nullptr) == 0)
		throw SerialException("no data read");

	return bytesRead;
}

void WindowsSerial::open(const std::string& port)
{
	// Try to connect to the given port throuh CreateFile
	mSerialHandle = CreateFile(port.c_str(),
	                           GENERIC_READ | GENERIC_WRITE,
	                           0,
	                           nullptr,
	                           OPEN_EXISTING,
	                           FILE_ATTRIBUTE_NORMAL,
	                           nullptr);

	// Check if the connection was successfull
	if (mSerialHandle == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			throw SerialException(std::string(port + std::string(" is not available")));
		else
			throw SerialException("unknown error " + to_string(GetLastError()) + " occured during serial open");
	}

	// If connected we try to set the comm parameters
	DCB dcbSerialParams = {0};

	// Try to get the current
	if (!GetCommState(mSerialHandle, &dcbSerialParams))
		throw SerialException("could not get current connection parameters");

	// Define serial connection parameters for the arduino board
	dcbSerialParams.BaudRate = CBR_115200;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	// Setting the DTR to Control_Enable ensures that the Arduino is properly
	// reset upon establishing a connection
	dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

	// Set the parameters and check for their proper application
	if (!SetCommState(mSerialHandle, &dcbSerialParams))
		throw SerialException("could not set new connection parameters");

	// Flush any remaining characters in the buffers
	PurgeComm(mSerialHandle, PURGE_RXCLEAR | PURGE_TXCLEAR);

	// We wait 2s as the arduino board will be resetting
	Sleep(2000);
}

void WindowsSerial::close()
{
    // Close the serial handler
	CloseHandle(mSerialHandle);
    mSerialHandle = nullptr;
}

bool WindowsSerial::deviceExists(const std::string& port) const
{
	// Try to connect to the given port throuh CreateFile
	HANDLE serialHandle = CreateFile(port.c_str(),
	                                 GENERIC_READ | GENERIC_WRITE,
	                                 0,
	                                 nullptr,
	                                 OPEN_EXISTING,
	                                 FILE_ATTRIBUTE_NORMAL,
	                                 nullptr);

	// close the handle again
	CloseHandle(serialHandle);

	// check for success
	return serialHandle != INVALID_HANDLE_VALUE;
}

bool WindowsSerial::good() const
{
	return true;
	throw std::runtime_error("not implemented");
}
