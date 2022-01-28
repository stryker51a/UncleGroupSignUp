//
// Created by ethan on 1/25/2022.
//

#ifndef UNCLEGROUPSIGNUP_LEADER_H
#define UNCLEGROUPSIGNUP_LEADER_H


#include "Person.h"
#include "member.h"

class leader : public Person{
public:
    leader();

    leader(std::string& name2, std::string& data);

private:
    std::vector<member> members;

};


#endif //UNCLEGROUPSIGNUP_LEADER_H
