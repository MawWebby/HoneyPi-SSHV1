// JSON EXAMPLE WITH FILES

#include <map>
#include "json.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <unistd.h>


using json = nlohmann::json;


std::string jsonLocation = "JSONFileTest.json";



//////////////////////////////////
//// STRING TO INT CONVERSION //// 
//////////////////////////////////
long long int stringtoint(std::string values) {
    long long int valuesdetermine = 0;
    bool completedwhile = false;
    int timing9760 = 0;
    int timing9760max = values.length();
    bool validcase = false;
    bool periodcheck = false;
    
    // CHECK FOR NULL
    if (values.length() == 0) {
        std::cout << "NULL CASE" << std::endl;
        return 0;
    }

    // CHECK FOR LONG INT
    if (values.length() > 19) {
        std::cout << "LONG LONG LONG INT" << std::endl;
        return 0;
    }

    // MAIN CHECK LOOP
    std::string substringvalue = "";
    while (completedwhile == false && timing9760 <= timing9760max) {
        if (timing9760 + 1 <= timing9760max) {
            validcase = false;
            if (periodcheck == false) {
                valuesdetermine = valuesdetermine * 10;
                periodcheck = false;
            } else {
                periodcheck = false;;
            }
            substringvalue = values.substr(timing9760, 1);
            if (substringvalue == "0") {
                validcase = true;
            } else if (substringvalue == "1") {
                validcase = true;
                valuesdetermine = valuesdetermine + 1;
            } else if (substringvalue == "2") {
                validcase = true;
                valuesdetermine = valuesdetermine + 2;
            } else if (substringvalue == "3") {
                validcase = true;
                valuesdetermine = valuesdetermine + 3;
            } else if (substringvalue == "4") {
                validcase = true;
                valuesdetermine = valuesdetermine + 4;
            } else if (substringvalue == "5") {
                validcase = true;
                valuesdetermine = valuesdetermine + 5;
            } else if (substringvalue == "6") {
                validcase = true;
                valuesdetermine = valuesdetermine + 6;
            } else if (substringvalue == "7") {
                validcase = true;
                valuesdetermine = valuesdetermine + 7;
            } else if (substringvalue == "8") {
                validcase = true;
                valuesdetermine = valuesdetermine + 8;
            } else if (substringvalue == "9") {
                validcase = true;
                valuesdetermine = valuesdetermine + 9;
            }


            // DECIMALS (IGNORE THIS CASE)
            // FIX THIS
            else if (substringvalue == ".") {
                validcase = true;
                periodcheck = true;
            }


            // INVALID STRING
            if (validcase != true) {
                std::cout << "INVALID CHARACTER" << std::endl;
                return 0;
            }   
        } else {
            completedwhile = true;
        }
        timing9760 = timing9760 + 1;
    }

    return valuesdetermine;
}



//////////////////////////////////
//// INT TO STRING CONVERSION //// 
//////////////////////////////////
std::string inttostring(long long int value) {
    std::string returnvalue = std::to_string(value);
    return returnvalue;
}


int main() {
    while(true) {
        // OPEN FILE AND PARSE JSON
        std::cout << "OPENING INPUT" << std::endl;
        std::ifstream inputFile;
        inputFile.open(jsonLocation.c_str());
        std::cout << "PROCESSING INPUT" << std::endl;
        json dataarray = json::parse(inputFile);
        inputFile.close();



        // MODIFY TESTING
        std::cout << "MODIFYING NUMBERS" << std::endl;
        if (dataarray["TESTING"]["HELLO"] == json::value_t::string) {
            std::string hello = inttostring(stringtoint(dataarray["TESTING"]["HELLO"]) + 1);
            dataarray["TESTING"]["HELLO"] = hello;
        } else {
            std::cout << "CONDITION NOT MET FOR HELLO!" << std::endl;
   
        }


        // MODIFY TESTING
        if (dataarray["TESTING"]["123"] == json::value_t::null) {
            dataarray["TESTING"]["123"] = "115";
        } else {
            std::string ping = inttostring(stringtoint(dataarray["TESTING"]["123"]) + 105);
            dataarray["TESTING"]["123"] = ping;
        }



        // PRINT VALUE
        std::cout << "Current Value of Hello is: " << dataarray["TESTING"]["HELLO"] << std::endl;



        // WRITE DATA
        std::cout << "OPENING OUTPUT FILE" << std::endl;
        std::ofstream outputFile;
        outputFile.open(jsonLocation.c_str());
        std::cout << "WRITING OUTPUT DATA!" << std::endl;
        outputFile << std::setw(4) << dataarray << std::endl;
        outputFile.close();

        sleep(5);        
    }
}
