#include "Scanner.h"
#include "UART.h"
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <thread>
#include <tuple>
#include <iostream>
#include <sstream>
#include <algorithm>
void delayo(int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

Scanner::Scanner() : uart("/dev/ttyAMA0")
{
}
void Scanner::calibrate()
{
    uart.calibrate();
}
void Scanner::setPrecision(int precision)
{
    uart.setPrecision(precision);
    delayo(2000);
}
void Scanner::setSampleCount(int precision)
{
    uart.setSampleCount(precision);
    delayo(2000);
}
void Scanner::reset()
{
    uart.drop();

    delayo(10000);
    uart.moveUp();
}
void Scanner::startScan(int layersCount)
{

    std::string response;
    do
    {
        //
        response = uart.scanLayer();
        if (response.size() > 1 && response[response.size() - 1] == '\n')
        {
            response.pop_back();
            response.pop_back();
        }

        if (response == ">object_limit" || response == "height_limit")
        {
            break;
        }
        std::cout << "DEBUG + {" << response << '}' << response.length() << "\n";
        raw.push_back(response);
        delayo(10000);
    } while (response != "height_limit" && layersCount-- > 0);
}
std::vector<std::string> Scanner::getRawData()
{
    return raw;
}
void Scanner::parseData()
{

    for (std::string &line : raw)
    {
        // Удаляем символы '>'
        line.erase(std::remove(line.begin(), line.end(), '>'), line.end());

        // Извлекаем число перед двоеточием
        size_t colonPos = line.find(':');
        pointsPerLayer = std::stoi(line.substr(0, colonPos));

        // Удаляем число и двоеточие из строки
        line = line.substr(colonPos + 1);

        // Разбиваем строку на тройки
        std::vector<std::tuple<double, double, double>> triples;
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, ';'))
        {
            std::stringstream tripleStream(token);
            std::string number;
            double a, b, c;

            std::getline(tripleStream, number, ',');
            a = std::stod(number);
            std::getline(tripleStream, number, ',');
            b = std::stod(number);
            std::getline(tripleStream, number, ',');
            c = std::stod(number);

            triples.emplace_back(a, b, c);
        }

        if (pointsPerLayer != triples.size())
        {
            std::cerr << "Warning: Number of triples doesn't match the specified count n." << std::endl;
        }

        pointMesh.push_back(triples);
    }
}
std::vector<std::vector<std::tuple<double, double, double>>> Scanner::getParsedData()
{
    return pointMesh;
}
void Scanner::convertData()
{
    std::ofstream objFile;
    objFile.open("output.object", std::ios::trunc);
    if (!objFile.is_open())
    {
        std::cerr << "Unable to open file for writing." << std::endl;
    }

    for (const auto &i : pointMesh)
    {
        for (const auto &line : i)
        {
            objFile << "v " << std::get<0>(line) << ' ' << std::get<2>(line) << ' ' << std::get<1>(line) << ' ' << std::endl;
        }
    }
    objFile << "s off" << std::endl;

    // запись граней (faces) надо имплиментировать алгоритм триангуляции наверное
    for (int layer = 0; layer < pointMesh.size() - 1; layer++)
    {
        for (int point = 0; point < pointsPerLayer; point++)
        {
            int currentPoint = layer * pointsPerLayer + point;
            int nextPoint = currentPoint + 1;
            int correspondingPointNextLayer = currentPoint + pointsPerLayer;
            int nextCorrespondingPointNextLayer = correspondingPointNextLayer + 1;

            // Если это последняя точка слоя, соединяем её с первой точкой следующего слоя
            if (point == pointsPerLayer - 1)
            {
                nextPoint = layer * pointsPerLayer;
                nextCorrespondingPointNextLayer = (layer + 1) * pointsPerLayer;
            }

            // Формирование двух треугольников
            objFile << "f " << currentPoint << ' ' << nextPoint << ' ' << correspondingPointNextLayer << std::endl;
            objFile << "f " << nextPoint << ' ' << nextCorrespondingPointNextLayer << ' ' << correspondingPointNextLayer << std::endl;
        }
    }

    objFile.close();

    std::cout << "OBJ file created." << std::endl;
}