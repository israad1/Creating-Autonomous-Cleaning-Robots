#include "libclean/building.hpp"


Building::Building(const std::string& filename)
    : building({})
    {this->filename = filename;
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Building object created" << std::endl; 
};

void Building::addRoom(Room* room){
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Building addRoom() function was called" << std::endl; 
    building.push_back(room);
};


std::vector<Room*> Building::getBuilding(){
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Building getBuilding() function was called" << std::endl; 
    return building;
};


std::vector<Room*> Building::getDirtyRooms(){
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Building getDirtyRooms() function was called" << std::endl; 
    std::vector<Room*> dirtyRooms;

    for (Room* r : building){
        bool clean = true;
        if(r->getSweepable()){
            if(r->getPercentSwept() != 100){
                clean = false;
            }
        }
        if(r->getMoppable()){
            if(r->getPercentMopped() != 100){
                clean = false;
            }
        }
        if(r->getScrubbable()){
            if(r->getPercentScrubbed() != 100){
                clean = false;
            }
        }
        if(!clean){
            dirtyRooms.push_back(r);
        }
    }
    return dirtyRooms;
};


std::vector<Room*> Building::getCleanRooms(){
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Building getCleanRooms() function was called" << std::endl; 
    
    std::vector<Room*> cleanRooms;
    for (Room* r : building){
        bool clean = true;
        if(r->getSweepable()){
            if(r->getPercentSwept() != 100){
                clean = false;
            }
        }
        if(r->getMoppable()){
            if(r->getPercentMopped() != 100){
                clean = false;
            }
        }
        if(r->getScrubbable()){
            if(r->getPercentScrubbed() != 100){
                clean = false;
            }
        }
        if(clean){
            cleanRooms.push_back(r);
        }
    }
    return cleanRooms;
};