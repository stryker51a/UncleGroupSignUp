//
// Created by ethan on 1/25/2022.
//

#ifndef UNCLEGROUPSIGNUP_PERSON_H
#define UNCLEGROUPSIGNUP_PERSON_H

#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <sstream>
#include <functional>
#include <numeric>
#include <algorithm>
#include <stdexcept>

class Person {

public:

    using availabilityList = std::vector<std::pair<std::string, double>>;

    bool available;

    const double LENGTH_OF_MEETING = 2; // in hours
    const std::string DATA_DELIMITER = ", ";
    const double INTERVAL = 1; // in hours

    Person();

    Person(std::string& name2, std::string data);

    Person(const Person& src);

    Person& operator=(const Person& rhs);

    std::string getName();

    ~Person();

    bool canMeet(const std::string& date, double time);

    bool isAvailable(const std::string& date, double time);

    bool isAvailable(std::pair<std::string, double> pair);

    void printAval();

    const availabilityList& getAvailabilityList();


private:

    bool isWeekday(const std::string& day);

    std::string name;

    availabilityList availability;
};


#endif //UNCLEGROUPSIGNUP_PERSON_H
