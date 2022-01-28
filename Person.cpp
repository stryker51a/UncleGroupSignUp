//
// Created by ethan on 1/25/2022.
//

#include <set>
#include "Person.h"
Person::Person() : available(true), name(""), availability(){
    // do nothing
}

Person::Person(std::string& name2, std::string data)
    : available(true),
      name(name2),
      availability()
{
    std::string curDate = "sat";

    std::string delimiter = DATA_DELIMITER;
    size_t pos = data.find(delimiter);
    std::string token;
    bool done = false;
    while (!done) {
        if (pos == std::string::npos){
            done = true;
        }

        token = data.substr(0, pos);

        if (isWeekday(token)){
            curDate = token;
        } else {
            if (!std::all_of(token.begin(), token.end(),
                             [](char c) { return std::isdigit(c) || c == '.'; })){
                throw std::invalid_argument("time data included a non-digit character");
            }
            std::stringstream num(token);
            double x;
            num >> x;

            std::pair<std::string, double> elem;

            elem.first = curDate;
            elem.second = x;

            availability.emplace_back(elem);
        }


        data.erase(0, pos + delimiter.length());
        pos = data.find(delimiter);
    }

    std::vector<std::pair<std::string, double>> test2;

    for (const std::pair<std::string, double>& p1 : availability){
        if (isAvailable(p1.first, p1.second)){
            test2.emplace_back(p1);
        }
    }

    std::swap(availability, test2);
}

Person::Person(const Person& src): available(src.available), name(src.name), availability(src.availability)
{
    //nothing to do
}

Person& Person::operator=(const Person& rhs){
    if (this != &rhs) { // make sure they are not the same Person
        Person copy(rhs); // create a copy
        std::swap(available, copy.available);
        std::swap(name, copy.name);
        std::swap(availability, copy.availability);
    }
    return *this;
}

Person::~Person(){
    availability.clear();
}

bool Person::isAvailable(std::pair<std::string, double> pair){
    return isAvailable(pair.first, pair.second);
}

std::string Person::getName(){
    return name;
}

bool Person::isAvailable(const std::string& date, double time){

    if (!isWeekday(date)){
        throw std::invalid_argument("date is wrong in isAvailable");
    }

    std::vector<double> times;
    for (double i = time; i < time + LENGTH_OF_MEETING; i += INTERVAL){
        times.emplace_back(i);
    }

    for (const auto& pair : availability){

        times.erase(std::remove_if(times.begin(), times.end(), [pair, date](double doub) {
            return pair.first == date && pair.second == doub;
        }), times.end());

    }

    if (times.empty()){
        return true;
    }
    return false;
}

bool Person::canMeet(const std::string& date, double time){
    for (auto pair : availability){
        if (pair.first == date && pair.second == time) {
            return true;
        }
    }
    return false;
}

const std::vector<std::pair<std::string, double>>& Person::getAvailabilityList() {
    return availability;
}

void Person::printAval(){

    for (const auto& pair : availability){
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}


bool Person::isWeekday(const std::string& day){

    if (day == "sat" || day == "sun" || day == "m" || day == "t"
        || day == "w" || day == "r" || day == "f"){
        return true;
    }

    return false;
}
