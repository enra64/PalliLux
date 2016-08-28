#ifndef LEDEXCEPTIONS_H
#define LEDEXCEPTIONS_H

#include <stdexcept>

/**
 * @brief Exceptions occuring in the AmbiConnector
 */
class AmbiConnectorException : public std::runtime_error
{
public:
	/**
	 * @brief std::string constructor
	 * @param message the error message
	 */
	AmbiConnectorException(const std::string& message) : runtime_error(message)
	{
	}
};

/**
 * @brief Exception for serial communication errors
 */
class AmbiConnectorCommunicationException : public AmbiConnectorException
{
public:
	/**
	 * @brief std::string constructor
	 * @param message the error message
	 */
	AmbiConnectorCommunicationException(const std::string& message) : AmbiConnectorException(message)
	{
	}
};

/**
 * @brief Exception for protocol errors (eg arduino not behaving as expected)
 */
class AmbiConnectorProtocolException : public AmbiConnectorException
{
public:
	/**
	 * @brief std::string constructor
	 * @param message the error message
	 */
	AmbiConnectorProtocolException(const std::string& message) : AmbiConnectorException(message)
	{
	}
};

#endif // LEDEXCEPTIONS_H
