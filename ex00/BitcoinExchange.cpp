#include "BitcoinExchange.hpp"
#include <fstream>
#include <ctype.h>

std::string trim(const std::string &str)
{
    size_t start = 0;
    while (start < str.size() && std::isspace(str[start]))
        start++;

    size_t end = str.size();
    while (end > start && std::isspace(str[end - 1]))
        end--;

    return str.substr(start, end - start);
}

bool parseline(const std::string &line)
{
    if (line.empty())
    {
        throw std::runtime_error("Line can't be empty");
        return false;
    }

    int year;
    int month;
    int maxDay;
    std::string dateVal;

    //parse Date/year should be 4 number and up of 1900 and less than 3000
    int i = 0;
    for (; i < 4; i++)
    {
        dateVal.push_back(line[i]);
        if (!isdigit(line[i]))
        {
            throw std::runtime_error("bad input => " + line);
            return false;
        }
    }

    if (line[i] == '-' && atoi(dateVal.c_str()) >= 1900 && atoi(dateVal.c_str()) <= 3000)
        ++i;
    else{
        throw std::runtime_error("bad input => " + line);
        return false;
    }

    year = atoi(dateVal.c_str());
    // parse month

    dateVal.clear();

    int j = 0;
    for (; j < 2; j++)
    {
        if (j == 1 && line[i] == '-')
            break ;
        if (!isdigit(line[i]))
        {
            throw std::runtime_error("bad input => " + line);
            return false;
        }
        dateVal.push_back(line[i]);
        i++;
    }

    if (line[i] == '-' && atoi(dateVal.c_str()) >= 1 && atoi(dateVal.c_str()) <= 12)
        ++i;
    else {
        throw std::runtime_error("bad input => " + line);
        return false;
    }

    month = atoi(dateVal.c_str());

    // parse Day

    dateVal.clear();

    j = 0;
    for (; j < 2; j++)
    {
        if (j == 1 && !isdigit(line[i]))
            break ;
        if (!isdigit(line[i]))
        {
            throw std::runtime_error("bad input => " + line);
            return false;
        }
        dateVal.push_back(line[i]);
        i++;
    }

    // check max day in the month
    switch(month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            maxDay = 31; break;
        case 4: case 6: case 9: case 11:
            maxDay = 30; break;
        case 2:
            maxDay = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
            break;
        default:
            month = -1;
    }

    if (atoi(dateVal.c_str()) < 1 && atoi(dateVal.c_str()) > maxDay)
    {
        throw std::runtime_error("bad input => " + line);
        return false;        
    }
    
    // Parse space before and after pipe 

    if ((line[i] == '\t' || line[i] == ' ') && line[i + 1] == '|' && \
        (line[i + 2] == '\t' || line[i + 2] == ' ') )
        i+=3;
    else {
        throw std::runtime_error("bad input => " + line);
        return false;        
    }
    
    // parse value

    // std::cout << line[i] << std::endl;

    dateVal.clear();
    int Onepoint = 0;
    j = 0;
    while (line[i])
    {
        if (line[i] == '-' || line[i] == '+')
        {
            dateVal.push_back(line[i]);
            i++;
            continue;
        }
        if (line[i] == '.' && j == 0)
            Onepoint = 44;

        if ((!isdigit(line[i]) && line[i] != '.') || (line[i] == '.' && Onepoint != 0))
        {
            // std::cout << line[i] << std::endl;
            throw std::runtime_error("bad input => " + line);
            return false; 
        }
        dateVal.push_back(line[i]);
        i++;
        j++;
    }


    double btcVal = strtod(dateVal.c_str(), NULL);
    // std::cout << btcVal << std::endl;

    if (btcVal < 0){
        throw std::runtime_error("not a positive number.");
        return false;        
    }
    else if (btcVal > 1000)
    {
        throw std::runtime_error("too large a number");
        return false;
    }

    return true;
}



void parsingInput(const std::string &InputFileName)
{
    // open file:
    std::ifstream InputFile(InputFileName.c_str());

    // check if it's opened:
    if (!InputFile.is_open())
    {
        throw std::runtime_error("Fialed to open input file");
        return ;
    }

    // Parse content:       Bad date, invalid value > 1000 or negative 

    std::string line;
    std::getline(InputFile, line);
    if ("date | value" != trim(line))
    {
        throw std::runtime_error("bad input => " + line);
        return ;
    }

    // std::map<std::string, int> data;

    while (std::getline(InputFile, line))
    {

            try {
                parseline(trim(line));
                std::cout << line << std::endl;
            }
            catch (std::exception &e)
            {
                std::cout << "Error: " << e.what() << std::endl;
            }
    }
    

}