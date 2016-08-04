#ifndef LEDEXCEPTIONS_H
#define LEDEXCEPTIONS_H

/**
 * @brief The LedConnectorException class provides exceptions occurring in LedConnector
 */
class AmbiConnectorException : public std::runtime_error {
public:
    /**
     * @brief AmbiConnectorException const char* constructor
     * @param message the error
     */
    AmbiConnectorException(const char* message) : runtime_error(message) {
    }

    /**
     * @brief AmbiConnectorException std::string constructor
     * @param message the error
     */
    AmbiConnectorException(const std::string& message) : runtime_error(message) {
    }
};

/**
 * @brief The AmbiConnectorCommunicationException class is thrown when a operation on the serial port fails
 */
class AmbiConnectorCommunicationException : public AmbiConnectorException {
public:
    /**
     * @brief AmbiConnectorCommunicationException const char* constructor
     * @param message the error
     */
    AmbiConnectorCommunicationException(const char* message) : AmbiConnectorException(message) {
    }

    /**
     * @brief AmbiConnectorCommunicationException std::string constructor
     * @param message the error
     */
    AmbiConnectorCommunicationException(const std::string& message) : AmbiConnectorException(message) {
    }
};

/**
 * @brief The AmbiConnectorProtocolException class is thrown when the communication partner does not follow the protocol
 */
class AmbiConnectorProtocolException : public AmbiConnectorException {
public:
    /**
     * @brief AmbiConnectorProtocolException const char* constructor
     * @param message the error
     */
    AmbiConnectorProtocolException(const char* message) : AmbiConnectorException(message) {
    }

    /**
     * @brief AmbiConnectorProtocolException std::string constructor
     * @param message the error
     */
    AmbiConnectorProtocolException(const std::string& message) : AmbiConnectorException(message) {
    }
};

#endif // LEDEXCEPTIONS_H
