#include <list>
#include <string> 
#include <iostream>
#include <chrono>
#include <thread>
#include "libclean/robot.hpp" 
#include "libclean/technician.hpp"

Technician::Technician(const std::string& filename) 
    : brokenRobotQueue({}){
    
    this->filename = filename;
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Technician object created" << std::endl;
};

std::deque<Robot*> Technician::getRobotQueue(){
    return brokenRobotQueue;
}

// Adds a robot to the busy queue if it's not already in the queue and if the robot has failed
bool Technician::addRobotToQueue(Robot* robot) {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Technician addRobotToQueue() is called" << std::endl;
    if (robot->hasFailed()) {
        for (Robot* r : brokenRobotQueue) {
            if (&r == &robot) {
                std::cout << "Robot already in Technicians queue." << std::endl;
                return false;
            }
        }
        brokenRobotQueue.push_back(robot);
        robot->setBusy(true);
        return true;
    //if robot in queue no need to add it. this actually should not happen - but just in case 
    } else {
        std::cout << "Robot already fixed." << std::endl;
        return false;
    }
};

// Fixes the robot and removes it from the busy queue
void Technician::technicianFixesRobot(){
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Technisian technicianFixesRobot() is called" << std::endl;
    if (!(brokenRobotQueue.empty())) {
        Robot* robot = brokenRobotQueue.front();
        robot->setBattery(100.0); // Set the battery to 100%
        brokenRobotQueue.pop_front();
        robot->setFailed(false);
        robot->setBusy(false);
    }
};

// Checks if the technician is busy
bool Technician::isTechBusy() const {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Technisian isTechBusy() is called" << std::endl;
    Robot* robot = brokenRobotQueue.front();
    return !brokenRobotQueue.empty();
};