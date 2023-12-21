#ifndef UART_H
#define UART_H
#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>
#include <string>

class UART
{
public:
    UART(const std::string &portPI);
    std::string scanLayer();
    bool setPrecision(const int &n);
    bool drop();
    bool calibrate();
    bool reSendLayer();
    bool setSampleCount(const int &n);
    bool moveUp();
    ~UART();

private:
    LibSerial::SerialPort serialPort;
    void sendData(const std::string &data);
    std::string recieveData(size_t allowedTimeout);
};

#endif // UART_H
