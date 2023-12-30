#include <math.h>
#include <string>
#include <fstream>
#include <iostream>
#include <random>

#include "libclean/room.hpp"

// need to change rooms names to always be unique - constructor

int Room::numberOfRooms = 0;

// Constuctor for Room
Room::Room(std::string name, float width_, float length_, bool sweepable, bool moppable, bool scrubbable, bool vacuumable,
 const std::string& filename) 
 : name(name), id(numberOfRooms), width_(width_), length_(length_), sweepable(sweepable), moppable(moppable), scrubbable(scrubbable), vacuumable(vacuumable)
{
    numberOfRooms++;
    this->filename = filename;
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    //if (!file.is_open()) {
        // Handle file opening error
      //  return;
    //}
    file << "Room object created" << std::endl; 

    if (sweepable) {
        percentSwept_ = 100;
    } else {
        percentSwept_ = NAN;
    }
    if (moppable) {
        percentMopped_  = 100;
    } else {
        percentMopped_ = NAN;
    }

        if (vacuumable) {
        percentVacuumed_  = 100;

    } else {
        percentVacuumed_ = NAN;
    }

    if (scrubbable) {
        percentScrubbed_ = 100;
        
    } else {
        percentScrubbed_ = NAN;
    }
};

std::string Room::getName() {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room getName() function was called" << std::endl; 
    return name;
};

float Room::getWidth() {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room getWidth() function was called" << std::endl; 

    return width_;
};

float Room::getLength() {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room getLength() function was called" << std::endl; 
    return length_;
};

float Room::getSize(){
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room getSize() function was called" << std::endl; 
    return width_ * length_;
};

int Room::getID() {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room getID() function was called" << std::endl; 
    return id;
};

bool Room::operator==(const Room& room){
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Checking Room id" << std::endl;
    if(id == room.id){
        return true;
    }else{
        return false;
    }
};

bool Room::getSweepable() {
     std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room getSweepable() function was called" << std::endl; 
    return sweepable;
};
bool Room::getVacuumable() {
     std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room getSweepable() function was called" << std::endl; 
    return sweepable;
};

bool Room::getMoppable() {
     std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room getMoppable() function was called" << std::endl; 
    return moppable;
};

bool Room::getScrubbable() {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room getScrubbable() function was called" << std::endl; 
    return scrubbable;
};

float Room::getPercentSwept() {
     std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room getPercentSwept() function was called" << std::endl; 
    return percentSwept_;
};

float Room::getPercentMopped() {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room getPercentMopped() function was called" << std::endl; 
    return percentMopped_;
};

float Room::getPercentScrubbed() {
   std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room getPercentScrubbed() function was called" << std::endl; 
    return percentScrubbed_;
};

float Room::getPercentVacuumed() {
   std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room getPercentScrubbed() function was called" << std::endl; 
    return percentScrubbed_;
};

void Room::setPercentSwept(float percent) {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room setPercentSwept() function was called" << std::endl; 
    if(sweepable && percentSwept_ <= 100 && percentSwept_ >= 0){
        percentSwept_ = percent;
    }
};

void Room::setPercentMopped(float percent) {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room setPercentMopped() function was called" << std::endl; 
    if(moppable && percentMopped_ <= 100 && percentMopped_ >= 0){
        percentMopped_ = percent;
    }
};
void Room::setPercentVacuumed(float percent) {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room setPercentMopped() function was called" << std::endl; 
    if(moppable && percentMopped_ <= 100 && percentMopped_ >= 0){
        percentMopped_ = percent;
    }
};

// can't go above 100
void Room::setPercentScrubbed(float percent) {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room setPercentScrubbed() function was called" << std::endl; 
    if(scrubbable && percentScrubbed_ <= 100 && percentScrubbed_ >= 0){
        percentScrubbed_ = percent;
    }
};

void Room::randomlyDirty() { // change later
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Room randomlyDirty() function was called" << std::endl; 
    if (sweepable) {
        std::srand(std::time(0));
        double percentRandDirty = (((double)std::rand()) / RAND_MAX) * 100;
        percentSwept_ = percentRandDirty;
    }
    if (moppable) {
        std::srand(std::time(0));
        double percentRandDirty = (((double)std::rand()) / RAND_MAX) * 100;
        percentMopped_ = percentRandDirty;
    }
    if (vacuumable) {
        std::srand(std::time(0));
        double percentRandDirty = (((double)std::rand()) / RAND_MAX) * 100;
        percentSwept_ = percentRandDirty;
    }
    if (scrubbable) {
        std::srand(std::time(0));
        double percentRandDirty = (((double)std::rand()) / RAND_MAX) * 100;
        percentScrubbed_ = percentRandDirty;
    }
};