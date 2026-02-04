#include "BitcoinExchange.hpp"




int main(int ac, char **av)
{
    (void)ac;
    if (!av[1]){
        std::cout << "Error: Add input file path < ./btc input.txt >" << std::endl;
        return 0;
    }

    std::string InputFile = av[1];





    // Load CSV data
    std::map<int, double> dataCsv;
    loadData("data.csv", dataCsv);


    // std::map<int, double>::iterator it;

    // int i = 0;
    // for (it = dataCsv.begin(); it != dataCsv.end(); ++it)
    // {
    //     std::cout << it->first << "   " << it->second << std::endl; 
    //     if (i == 250)
    //         break;
    //     i++;
    // }



    // open input file + parse the content :
    try{
        parsingInput(InputFile);
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 0;
    }






}