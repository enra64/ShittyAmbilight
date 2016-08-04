#ifndef LEDCONNECTOR_H
#define LEDCONNECTOR_H

#include "rgbconstructor.h"

/**
 * @brief The LedConnectorException class provides exceptions occurring in LedConnector
 */
class LedConnectorException : public std::runtime_error {
public:
    /**
     * @brief LedConnectorException const char* constructor
     * @param message the error
     */
    explicit LedConnectorException(const char* message) : runtime_error(message) {
    }

    /**
     * @brief LedConnectorException std::string constructor
     * @param message the error
     */
    explicit LedConnectorException(const std::string& message) : runtime_error(message) {
    }
};

/**
 * @brief The LedConnector class is the master class. It interfaces with the RGB leds and obtains the image data.
 */
class LedConnector {
public:
    /**
     * @brief update update screen data
     */
    void update();

    /**
     * @brief draw push current data to leds
     */
    void draw();

    /**
     * @brief connect connect to the arduino; because it waits for established comms, it is blocking
     * @return true if connection establishment successfull
     */
    bool connect(const std::__cxx11::string &port);

    /**
     * @brief getAverageUpdateDuration speed statistics
     * @return average duration of the update function in seconds
     */
    float getAverageUpdateDuration() {
        return mUpdateDuration / mUpdateCount;
    }

    /**
     * @brief getAverageUpdateDuration speed statistics
     * @return average duration of the draw function in seconds
     */
    float getAverageDrawDuration() {
        return mDrawDuration / mDrawCount;
    }

    /**
     * @brief LedConnector construct a LedConnector, responsible for obtaining and updating the led data
     * @param borderProvider will be used to obtain the border images
     * @param horizontalLedCount how many leds are on each horizontal border
     * @param verticalLedCount how many leds are on each vertical border
     */
    LedConnector(std::shared_ptr<BorderProvider> borderProvider, unsigned int horizontalLedCount, unsigned int verticalLedCount);

    ~LedConnector();
private:

    /**
     * @brief mRgbConverter the object that provides the RGB uint8_t led data
     */
    std::unique_ptr<RgbConverter> mRgbConverter;

    /**
     * @brief mRgbBuffer our very own bufferspace for led data
     */
    uint8_t* mRgbBuffer;

    /**
     * @brief mCommBuffer smaller buffer for arduino responses
     */
    char mCommBuffer[128];

    /**
     * @brief mSerialFd serial connection file descriptor
     */
    int mSerialFd = -1;

    /**
     * @brief writeRgbBufferToText Another debug function; can be used to print out the led buffer to a text file
     * @param buffer the buffer to write; uint8_t RGB-triplets are expected
     * @param length the buffer length in bytes (number of uint8_t)
     * @param path where to write to
     */
    void writeRgbBufferToText(std::string path);

    float mDrawDuration = 0, mUpdateDuration = 0;

    unsigned int mDrawCount = 0, mUpdateCount = 0;
};

#endif // LEDCONNECTOR_H
