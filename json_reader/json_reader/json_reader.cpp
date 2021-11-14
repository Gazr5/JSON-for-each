#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include "single_include\nlohmann\json.hpp"


using json = nlohmann::json;

inline bool isInteger(const std::string& s)
{
    char* p;
    std::strtol(s.c_str(), &p, 10);

    return (*p == 0);
}

std::string makeJSONElement(std::string s) {
    std::string result;
    
    if (isInteger(s)) {
        result += "[" + s + "]";
    }
    else {
        result += "." + s;
    }

    return result;
}

void getKeyAndValue(json js, std::string path, std::string outputFile) {

    for (const auto& i : js.items()) {

        std::string temp = path + makeJSONElement(i.key());

        if (i.value().is_structured()) {

            getKeyAndValue(i.value(), temp, outputFile);

        }
        else {

            std::string value = i.value().dump();

            value.erase(remove(value.begin(), value.end(), '"'), value.end());

            if (outputFile.empty()) {
                std::cout << std::string(temp.begin() + 1, temp.end()) << "\t" << value << std::endl;
            }
            else {
                std::fstream output_fstream;
                output_fstream.open(outputFile, std::ios::app);
                output_fstream << std::string(temp.begin() + 1, temp.end()) << "\t" << value << std::endl;
                output_fstream.close();
            }
        }
    }
}

int main()
{
    
    std::string inputFile, outputFile;

    std::cout << "Write the input name" << std::endl;
    std::cin >> inputFile;
    std::cout << "Write the output name or write 0 if this file is created automatically" << std::endl;
    std::cin >> outputFile;

    json js = json::parse(std::ifstream(inputFile));
    std::string temp = "";

    //if (outputFile == "0") outputFile = "output.txt";

    std::ofstream output_fstream;
    output_fstream.open(outputFile, std::ofstream::out | std::ofstream::trunc);
    output_fstream.close();

    getKeyAndValue(js, temp, outputFile);
}