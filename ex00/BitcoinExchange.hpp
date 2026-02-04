#pragma once
#include <iostream>
#include <map>
#include <exception>
#include <cstdlib>
#include <iomanip>
#include <fstream>


void parsingInput(const std::string&, std::map<int, double> &dataCsv);
void loadData(const std::string &FileName, std::map<int, double> &dataCsv);