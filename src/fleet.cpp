#include <string> 
#include <math.h> 
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include "libclean/robot.hpp"
#include "libclean/fleet.hpp"

Fleet::Fleet(const std::string& filename)
   : fleet({}), availableRobots({}),  busyRobots({})
   {
    this->filename = filename;
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    //if (!file.is_open()) {
        // Handle file opening error
      //  return;
    //}
    file << "Fleet object created" << std::endl; 
   };

void Fleet::updateVectors(Robot* robot){
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Fleet updateVectors() function was called" << std::endl; 
    if (robot->getBusy()) { // will run if robot is busy
      for (int i = 0; i < availableRobots.size(); i++) {
         if (availableRobots[i] == robot) {
            availableRobots.erase(availableRobots.begin() + i);
         }
      }
      busyRobots.push_back(robot);
    }
    else { // if robot is not busy
      for (int i = 0; i < busyRobots.size(); i++) {
         if (busyRobots[i] == robot) {
            busyRobots.erase(busyRobots.begin() + i);
         }
      }
      availableRobots.push_back(robot);
    }
};

void Fleet::addToFleet(Robot* robot) {
   std::ofstream file;
   file.open(filename, std::ofstream::app);
   file << "Fleet addToFleet() function was called" << std::endl; 
   fleet.push_back(robot);
};

std::vector<Robot*> Fleet::getFleet() {
   std::ofstream file;
   file.open(filename, std::ofstream::app);
   file << "Fleet getFleet() function was called" << std::endl; 
   return fleet;
};

std::vector<Robot*> Fleet::getAvailableRobots() {
   std::ofstream file;
   file.open(filename, std::ofstream::app);
   file << "Fleet getAvailableRobots() function was called" << std::endl; 
   return availableRobots;
};

std::vector<Robot*> Fleet::getBusyRobots() {
   std::ofstream file;
   file.open(filename, std::ofstream::app);
   file << "Fleet getBusyRobots() function was called" << std::endl; 
   return busyRobots;
};