#include <iostream>
#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>
#include <chrono>
#include <thread>
#include <pigpio.h>
#include "UART.h"
#include "Scanner.h"

void delay(int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void controlLED(int gpio, bool state)
{
    gpioWrite(gpio, state ? PI_HIGH : PI_LOW);
}

void flashLED(int gpio, int duration)
{
    controlLED(gpio, true);
    delay(duration);
    controlLED(gpio, false);
}

using namespace std::chrono;
steady_clock::time_point lastPressTime;

bool buttonPressedFlag = false;
bool isScanning = false;

void buttonPressed(int gpio, int level, uint32_t tick)
{
    auto now = steady_clock::now();
    if (duration_cast<milliseconds>(now - lastPressTime).count() > 200)
    { // 200 мс дребезгозащита
        if (!isScanning)
        {
            buttonPressedFlag = true;
            std::cout << "Кнопка на GPIO " << gpio << " была нажата." << std::endl;
        }
        lastPressTime = now;
    }
}

int main()
{
    if (gpioInitialise() < 0)
    {
        std::cerr << "Не удалось инициализировать pigpio." << std::endl;
        return 1;
    }
    gpioSetMode(2, PI_INPUT);
    gpioSetPullUpDown(2, PI_PUD_UP);
    gpioSetISRFunc(2, FALLING_EDGE, 0, buttonPressed);

    gpioSetMode(17, PI_OUTPUT); // Светодиод для калибровки
    gpioSetMode(27, PI_OUTPUT); // Светодиод для сканирования

    while (true)
    {
        if (!buttonPressedFlag)
        {
            flashLED(17, 300); // Мигание светодиода при калибровке
        }
        else if (!isScanning)
        {

            try
            {
                isScanning = true;
                controlLED(27, true); // Включить светодиод сканирования
                Scanner myScanner;
                myScanner.setPrecision(4);
                myScanner.calibrate();
                delay(5000);
                myScanner.startScan(10);
                myScanner.parseData();
                myScanner.convertData();
                myScanner.reset();
                controlLED(27, false); // Выключить светодиод сканирования
            }
            catch (std::exception &e)
            {
                std::cerr << e.what();
                return 1;
            }
            isScanning = false;
            buttonPressedFlag = false;
        }
        time_sleep(1);
    }

    gpioTerminate();
    return 0;
}
