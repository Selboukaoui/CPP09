#pragma once
#include <iostream>
#include <map>
#include <exception>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <algorithm>




void parsingInput(const std::string&);
void loadData(const std::string &FileName, std::map<int, double> &dataCsv);