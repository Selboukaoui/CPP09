#include "BitcoinExchange.hpp"

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

void ParseAndExcuteLine(const std::string &line, std::map<int, double> &data, std::map<int, double> &dataCsv)
{
    if (line.empty())
        throw std::runtime_error("Line can't be empty");

    int year;
    int month;
    int maxDay;
    std::string dateVal;

    //parse Date/year should be 4 number and up of 1900 and less than 3000 

    // parse year
    int i = 0;
    for (; i < 4; i++)
    {
        if (!isdigit(line[i]))
            throw std::runtime_error("bad input => " + line);
        dateVal.push_back(line[i]);
    }

    if (line[i] == '-' && atoi(dateVal.c_str()) >= 1900 && atoi(dateVal.c_str()) <= 3000)
        ++i;
    else
        throw std::runtime_error("bad input => " + line);

    year = atoi(dateVal.c_str());

    // parse month
    dateVal.clear();
    int j = 0;
    for (; j < 2; j++)
    {
        if (j == 1 && line[i] == '-')
            break ;
        if (!isdigit(line[i]))
            throw std::runtime_error("bad input => " + line);
        dateVal.push_back(line[i]);
        i++;
    }

    if (line[i] == '-' && atoi(dateVal.c_str()) >= 1 && atoi(dateVal.c_str()) <= 12)
        ++i;
    else
        throw std::runtime_error("bad input => " + line);

    month = atoi(dateVal.c_str());

    // parse Day

    dateVal.clear();

    j = 0;
    for (; j < 2; j++)
    {
        if (j == 1 && !isdigit(line[i]))
            break ;
        if (!isdigit(line[i]))
            throw std::runtime_error("bad input => " + line);

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
    }

    int day = atoi(dateVal.c_str());
    if (day < 1 || day > maxDay)
        throw std::runtime_error("bad input => " + line);
    
    // Parse space before and after pipe 

    if ((line[i] == '\t' || line[i] == ' ') && line[i + 1] == '|' && \
        (line[i + 2] == '\t' || line[i + 2] == ' ') )
        i+=3;
    else
        throw std::runtime_error("bad input => " + line);
    
    // parse value 
    dateVal.clear();
    int OneDot = 0;

    while (line[i])
    {
        if (line[i] == '-' || line[i] == '+')
        {
            dateVal.push_back(line[i]);
            i++;
            continue;
        }
        
        if ((!isdigit(line[i]) && line[i] != '.') || (line[i] == '.' && OneDot != 0))
            throw std::runtime_error("bad input => " + line);
        
        if (line[i] == '.' && OneDot == 0)
            OneDot = 44;
        dateVal.push_back(line[i]);
        i++;
    }
    
    double btcVal = strtod(dateVal.c_str(), NULL);

    if (btcVal < 0 )
        throw std::runtime_error("not a positive number.");
    else if (btcVal > 1000)
        throw std::runtime_error("too large a number");


    // load input data:
    int date = year * 10000 + month * 100 + day;
    data.insert(std::pair<int, double>(date, btcVal));


    
    // search and execute

    std::map<int, double>::iterator it;

    it = dataCsv.find(date);
    if (it == dataCsv.end())
    {
        it = dataCsv.lower_bound(date);
        if (it != dataCsv.begin())
            --it;
    }
    
    if (it != dataCsv.end())
        std::cout << year << "-" << std::setw(2) << std::setfill('0') << month << "-" << std::setw(2) << std::setfill('0') << day << " => " << btcVal << " = " << it->second * btcVal <<std::endl;
    else
        std::cout << date <<"  Not found" << std::endl;

}



void parsingInput(const std::string &FileName, std::map<int, double> &dataCsv)
{
    // open file:
    std::ifstream InputFile(FileName.c_str());

    // check if it's opened:
    if (!InputFile.is_open())
    {
        throw std::runtime_error("Failed to open input file");
    }

    // Parse content:       Bad date, invalid value > 1000 or negative 

    std::string line;
    std::getline(InputFile, line);
    if ("date | value" != trim(line))
        throw std::runtime_error("bad input => " + line);

    std::map<int, double> data;

    while (std::getline(InputFile, line))
    {
            try 
            {
                ParseAndExcuteLine(trim(line), data, dataCsv);
            }
            catch (std::exception &e)
            {
                std::cout << "Error: " << e.what() << std::endl;
            }
    }
}




void loadData(const std::string &FileName, std::map<int, double> &dataCsv)
{
    // open file:
    std::ifstream InputFile(FileName.c_str()); 

    // check if it's opened:
    if (!InputFile.is_open())
        throw std::runtime_error("Failed to open input file");

    std::string line;
    std::getline(InputFile, line);

    while (std::getline(InputFile, line))
    {
        int comma = 0;
        int date = 0;
        double exchange_rate = 0;

        for (size_t i = 0; i < line.length(); i++)
        {
            if (isdigit(line[i]) && comma == 0)
                date = date * 10 + (line[i] - '0'); 
            else if (line[i] == ',')
            {
                comma = 1;
                i++;
                exchange_rate = strtod(&line[i], NULL);
                break ;
            }
        }
        dataCsv.insert(std::pair<int, double>(date, exchange_rate));
    }

}


