#ifndef SERIAL_H
#define SERIAL_H

#include <cstdint>

#include <stdexcept>

class SerialException : public std::runtime_error
{
public:
	/**
	 * @brief std::string constructor
	 * @param message the error message
	 */
	SerialException(const std::string& message) : runtime_error(message)
	{
	}
};

/**
 * @brief An interface for serial communication with arduino
 */
class Serial
{
public:
	virtual ~Serial(){}

	virtual void waitForData() const = 0;
	virtual void send(const uint8_t* buf, size_t len) const = 0;
	virtual size_t receive(uint8_t* buf, size_t len) const = 0;
	virtual void open(const std::string& port) = 0;
	virtual void close() = 0;
	virtual bool deviceExists(const std::string& port) const = 0;
	virtual bool good(const std::string& ttyDevice) const = 0;

public:// send and receive overloads because c++ likes to whine
	virtual size_t receive(char* buf, size_t len) const
	{
		return this->receive(reinterpret_cast<uint8_t*>(buf), len);
	}

	virtual void send(const char* buf, size_t len) const
	{
		this->send(reinterpret_cast<const uint8_t*>(buf), len);
	}
};

#endif // SERIAL_H
