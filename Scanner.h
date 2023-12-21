#ifndef SCANNER_H
#define SCANNER_H
#include "UART.h"
#include <vector>
#include <tuple>

class Scanner
{
public:
    Scanner();
    void startScan(int layersCount = 1e9);
    void setPrecision(int precision);
    void setSampleCount(int precision);
    void parseData();
    void convertData();
    void reset();
    void calibrate();
    std::vector<std::vector<std::tuple<double, double, double>>> getParsedData();
    std::vector<std::string> getRawData();

private:
    std::vector<std::vector<std::tuple<double, double, double>>> pointMesh;
    UART uart;
    std::vector<std::string> raw;
    int pointsPerLayer;
};

#endif // SCANNER_H
