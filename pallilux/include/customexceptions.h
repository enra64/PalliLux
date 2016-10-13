#ifndef LEDEXCEPTIONS_H
#define LEDEXCEPTIONS_H

#include <stdexcept>

/**
 * @brief Exceptions occuring in the ArduinoConnector
 */
class ArduinoConnectorException : public std::runtime_error
{
public:
	/**
	 * @brief std::string constructor
	 * @param message the error message
	 */
    ArduinoConnectorException(const std::string& message) : runtime_error(message)
	{
	}
};

/**
 * @brief Exception for serial communication errors
 */
class ArduinoConnectorCommunicationException : public ArduinoConnectorException
{
public:
	/**
	 * @brief std::string constructor
	 * @param message the error message
	 */
    ArduinoConnectorCommunicationException(const std::string& message) : ArduinoConnectorException(message)
	{
	}
};

/**
 * @brief Exception for protocol errors (eg arduino not behaving as expected)
 */
class ArduinoConnectorProtocolException : public ArduinoConnectorException
{
public:
	/**
	 * @brief std::string constructor
	 * @param message the error message
	 */
    ArduinoConnectorProtocolException(const std::string& message) : ArduinoConnectorException(message)
	{
	}
};

#endif // LEDEXCEPTIONS_H
