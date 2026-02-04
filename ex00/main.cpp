#include "BitcoinExchange.hpp"

int main(int ac, char **av)
{
    (void)ac;
    if (!av[1])
    {
        std::cout << "Error: Add input file path < ./btc input.txt >" << std::endl;
        return 0;
    }
    std::string InputFile = av[1];

    // Load CSV data
    std::map<int, double> dataCsv;

    try{
        loadData("data.csv", dataCsv);
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 0;
    }

    // parse input and excute the result
    try{
        parsingInput(InputFile, dataCsv);
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 0;
    }
}