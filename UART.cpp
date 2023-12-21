#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>
#include "UART.h"
#include <iostream>

UART::UART(const std::string &portPI)
{
    try
    {
        serialPort.Open(portPI);
    }
    catch (const LibSerial::OpenFailed &e)
    {
        throw e;
    }
    serialPort.SetBaudRate(LibSerial::BaudRate::BAUD_9600);
    serialPort.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
    serialPort.SetStopBits(LibSerial::StopBits::STOP_BITS_1);
    serialPort.SetParity(LibSerial::Parity::PARITY_NONE);
    serialPort.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
}
std::string UART::scanLayer()
{
    sendData("scan-level");
    return recieveData(5e5); // subjectToChange
}
bool UART::setPrecision(const int &n)
{
    // sendData("set-sample-size " + std::to_string(n));
    return true;
}
bool UART::drop()
{
    sendData("move-to-bottom");
    return true;
}
bool UART::moveUp()
{
    sendData("initial-move-up 1");

    try
    {
        recieveData(10000);
    }
    catch (std::exception &e)
    {
        std::cerr << "ok moveUp\n";
        return true;
    }
    std::cerr << "DEBUG:height-lim\n";
    return false;
}
bool UART::calibrate()
{
    sendData("calibrate");

    // try{
    //     recieveData(5000);
    // }
    // catch(std::exception &e){
    //     std::cerr << "ok i guess\n";
    //     return true;
    // }
    return false;
}
bool UART::reSendLayer() { return true; }
bool UART::setSampleCount(const int &n)
{
    sendData("set-sample-size " + std::to_string(n));
    return true;
}
void UART::sendData(const std::string &data)
{
    serialPort.Write(data);
    std::cout << "command " << data << " sent\n";
}
std::string UART::recieveData(size_t allowedTimeout)
{
    std::string readData;

    serialPort.ReadLine(readData, '\n', allowedTimeout);

    return readData;
}
UART::~UART()
{
    serialPort.Close();
}