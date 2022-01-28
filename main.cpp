#include<iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <set>
#include <list>
#include "Person.h"
#include "member.h"
#include "leader.h"

using memberList = std::vector<member>;
using leaderList = std::vector<leader>;
using group = std::pair<leader, memberList>;
using groupList = std::vector<group>;
using solutions = std::list<groupList>;
using dateTime = std::pair<std::string, double>;
using availabilityList = std::vector<dateTime>;


const int MAX_GROUP_SIZE = 4;

uint32_t calcFitness(group g1){

    availabilityList leaderAval = g1.first.getAvailabilityList();
    availabilityList times;

    for (const auto &pair : leaderAval){
        bool goodTime = true;
        for (member &m1 : g1.second){
            if (!m1.canMeet(pair.first, pair.second)){
                goodTime = false;
            }
        }

        if (goodTime){
            times.emplace_back(pair);
        }
    }

    if (times.empty()){
        return 0;
    }

    return 1;
}

void findBestSolutions(uint32_t prevFitness, leaderList& avalLeaders, uint32_t numAvalLeaders,
                       memberList& avalMembers, uint32_t numAvalMemebers,
                       uint32_t& maxFitness, solutions& maxSolutions, groupList& prevSolutions){
    if (numAvalLeaders == 0 && numAvalMemebers == 0){
        //evaluate maximum
        if (prevFitness > maxFitness){
            maxFitness = prevFitness;
            maxSolutions.clear();
            maxSolutions.emplace_back(prevSolutions);
        } else if (prevFitness == maxFitness){
            maxSolutions.emplace_back(prevSolutions);
        }
        return;
    } else if ((numAvalLeaders == 0 && numAvalMemebers != 0) ||
            (numAvalLeaders != 0 && numAvalMemebers == 0)){
        return;
    }

    bool done = false;

    uint32_t numMemInGroup = 1;
    if (numAvalLeaders * 3 < numAvalMemebers){
        numMemInGroup = 4;
    } else if (numAvalLeaders * 2 < numAvalMemebers){
        numMemInGroup = 3;
    } else if (numAvalLeaders < numAvalMemebers){
        numMemInGroup = 2;
    }


    for (leader& l1 : avalLeaders){

        if (!l1.available)
            continue;

        l1.available = false;
        for (auto mit1 = avalMembers.begin(); mit1 != avalMembers.end(); ++mit1){
            if (!mit1->available)
                continue;
            mit1->available = false;

            if (numMemInGroup == 1){
                group groupS1;
                groupS1.first = l1;
                groupS1.second.emplace_back(*mit1);

                uint32_t fit = calcFitness(groupS1);
                if (fit != 0){
                    prevSolutions.emplace_back(groupS1);

                    findBestSolutions(prevFitness + fit, avalLeaders,
                                      numAvalLeaders -1, avalMembers
                            , numAvalMemebers -numMemInGroup, maxFitness
                            , maxSolutions, prevSolutions);

                    prevSolutions.pop_back();
                }

                done = true;
            }

            for (auto mit2 = mit1+1; mit2 != avalMembers.end() && !done; ++mit2) {
                if (!mit2->available)
                    continue;
                mit2->available = false;


                if (numMemInGroup == 2){
                    group groupS2;
                    groupS2.first = l1;
                    groupS2.second.emplace_back(*mit1);
                    groupS2.second.emplace_back(*mit2);

                    uint32_t fit = calcFitness(groupS2);
                    if (fit != 0){
                        prevSolutions.emplace_back(groupS2);

                        findBestSolutions(prevFitness + fit, avalLeaders,
                                          numAvalLeaders -1, avalMembers
                                , numAvalMemebers -numMemInGroup, maxFitness
                                , maxSolutions, prevSolutions);

                        prevSolutions.pop_back();
                    }

                    done = true;
                }


                for (auto mit3 = mit2+1; mit3 != avalMembers.end() && !done; ++mit3) {
                    if (!mit3->available)
                        continue;
                    mit3->available = false;

                    // check for 3 person teams
                    if (numMemInGroup == 3){
                        group groupS3;
                        groupS3.first = l1;
                        groupS3.second.emplace_back(*mit1);
                        groupS3.second.emplace_back(*mit2);
                        groupS3.second.emplace_back(*mit3);


                        uint32_t fit = calcFitness(groupS3);
                        if (fit != 0){
                            prevSolutions.emplace_back(groupS3);

                            findBestSolutions(prevFitness + fit, avalLeaders,
                                              numAvalLeaders -1, avalMembers
                                    , numAvalMemebers -numMemInGroup, maxFitness
                                    , maxSolutions, prevSolutions);

                            prevSolutions.pop_back();
                        }

                        done = true;
                    }

                    for (auto mit4 = mit3+1; mit4 != avalMembers.end() && !done; ++mit4) {
                        if (!mit4->available)
                            continue;
                        mit4->available = false;

                        group g1;

                        g1.first = l1;
                        g1.second.emplace_back(*mit1);
                        g1.second.emplace_back(*mit2);
                        g1.second.emplace_back(*mit3);
                        g1.second.emplace_back(*mit4);

                        uint32_t fit = calcFitness(g1);
                        if (fit != 0){
                            prevSolutions.emplace_back(g1);

                            findBestSolutions(prevFitness + fit, avalLeaders,
                                              numAvalLeaders -1, avalMembers
                                              , numAvalMemebers -4, maxFitness
                                              , maxSolutions, prevSolutions);

                            prevSolutions.pop_back();
                        }

                        mit4->available = true;
                    }
                    mit3->available = true;
                }
                mit2->available = true;
            }
            mit1->available = true;
        }
        l1.available = true;
    }
}

bool isSameTime(dateTime dt1, dateTime dt2){
    if (dt1.first == dt2.first && dt1.second == dt2.second){
        return true;
    }
    return false;
}

availabilityList findCommonTimes(leader& leader1, memberList& memeberList1){
    availabilityList times;
    for (const auto& time : leader1.getAvailabilityList()){

        bool good = true;
        for (member m1 : memeberList1){
            bool good2 = false;
            for (const auto& time2 : m1.getAvailabilityList()){
                if (isSameTime(time, time2)){
                    good2 = true;
                    break;
                }
            }
            if (!good2){
                good = false;
                break;
            }
        }

        if (good){
            times.emplace_back(time);
        }
    }

    return times;
}

void printAvalList(availabilityList al1){
    for (const auto& pair : al1){
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}


int main() {

    std::string example = "sat, 1, 2, 3, 4, sun, 3, 4, 5, m, 2, 7, 8, 9";
    std::string name = "Ethan";

    std::ifstream myReadFile("C:/Users/ethan/OneDrive - Vanderbilt/Documents/"
                             "Extra-Curricular/UncleGroupSignUp/leaders.txt");

    std::string line;
    leader testLeader1;

    leaderList lList1;

    while (myReadFile.good() && std::getline(myReadFile, line)){
        size_t pos = line.find(": ");
        std::string testName = line.substr(0, pos);
        std::string testAval = line.substr(pos + 2);

        leader l1(testName, testAval);

        lList1.emplace_back(l1);
    }

//    for (leader leader123 : lList1){
//        std::cout << leader123.getName() << std::endl;
//        leader123.printAval();
//        std::cout << std::endl;
//    }

    memberList mList1;

    std::ifstream myReadFile2("C:/Users/ethan/OneDrive - Vanderbilt/Documents/"
                             "Extra-Curricular/UncleGroupSignUp/members.txt");

    while (myReadFile2.good() && std::getline(myReadFile2, line)){
        size_t pos = line.find(": ");
        std::string testName = line.substr(0, pos);
        std::string testAval = line.substr(pos + 2);

        member m1(testName, testAval);

        mList1.emplace_back(m1);
    }

//    for (member member123 : mList1){
//        std::cout << member123.getName() << std::endl;
//        member123.printAval();
//        std::cout << std::endl;
//    }


//    member m1(name, example);
//    m1.printAval();
//
//    std::cout << std::endl;


//findBestSolutions(uint32_t prevFitness, leaderList avalLeaders, uint32_t numAvalLeaders,
//                       memberList avalMembers, uint32_t numAvalMemebers,
//                       uint32_t& maxFitness, solutions& maxSolutions, groupList& prevSolutions)


    solutions solList;
    groupList testGroupList;
    uint32_t maxFit1 = 0;
    uint32_t avalLeaders1 = lList1.size();
    uint32_t avalMems1 = mList1.size();

    findBestSolutions( (uint32_t) 0, lList1,avalLeaders1,
                       mList1, avalMems1,maxFit1,
                       solList, testGroupList);


    for (groupList gl1 : solList){
        std::cout << "--------- NEW GROUP ---------" << std::endl;

        for (group g1 : gl1){
            printAvalList(findCommonTimes(g1.first, g1.second));

            std::cout << "Leader: " << g1.first.getName() << std::endl <<"Memebers: ";
            for (member theMember : g1.second){
                std::cout << theMember.getName() << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }



    return 0;
}


