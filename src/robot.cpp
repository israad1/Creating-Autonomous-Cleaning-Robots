#include <string> 
#include <math.h> 
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>
#include <random>

#include "libclean/robot.hpp"
#include "libclean/room.hpp"
#include "libclean/timer.hpp"

// need to change robot names to always be unique - constructor and setName
int Robot::numberOfRobots = 0;

Robot::Robot(std::string name, float battery_, Size size, Room *location, const std::string& filename, Job job) 
    :   name(name), id(numberOfRobots), battery_(battery_), size(size), location(location), busy(false), failed(false), job(job)
    {numberOfRobots++;
    this->filename = filename;
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot object created" << std::endl; 
};

bool Robot::operator==(const Robot& robot){
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Checking Robot id" << std::endl;
    if(id == robot.id){
        return true;
    }else{
        return false;
    }
};

std::string Robot::getName() {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot getName() function was  called" << std::endl;
    return name;
};

int Robot::getID(){
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot getID() function was called" << std::endl;
    return id;
};

float Robot::getBattery() {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot getBattery() function was called" << std::endl;
    return battery_;
};

Size Robot::getSize() {
   std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot getSize() function was called" << std::endl;
    return size;
};

Room* Robot::getLocation() {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot getLocation() function was called" << std::endl;
    return location;
};

bool Robot::getBusy() {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot getBusy() function was called" << std::endl;
    return busy;
};

bool Robot::getFailed() {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot getFailed() function was called" << std::endl;
    return failed;
};

Job Robot::getJob() {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot getJob() function was called" << std::endl;
    return job;
};

//assign a new condition for the battery_, so that the function would check if the battery_ is enough for a movemrnt 
//so if the battery_ is lower then like 20% then go to charge (call the charge function ) 
//if the batttry is greater then that then uou can move and decrement the charhe of the batttatry 

//add a decrement function for the battery_ here (maybe not necessry a function but a variable ot just call (battery_--))
void Robot::move(Room *room) {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot move() function was called" << std::endl;
    if (this->battery_ <= 5) {
        std::cout << "Battery is low, need to charge." << std::endl;
        return;
    }
    this->setLocation(room);
    std::cout << name << " with battery: " << battery_ << " is moving to: " << location->getName() << std::endl;
    this->battery_ -= 5;
};

void Robot::charge() {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot charge() function was called" << std::endl;
    if (battery_ >= 0.0 && battery_ < 100.0) {
        // Loop until the battery_ is fully charged
        float newBattery_ = this->getBattery() + 5;
        if (newBattery_ < 100) {
            this->setBattery(newBattery_);
        } else {
            this->setBattery(100);
        }
    }
};

void Robot::setLocation(Room* room) {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot setLocation() function was called" << std::endl;
    this->location = room;
};

void Robot::setName(std::string newname) {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot setName() function was called" << std::endl;
    this->name = newname;
};

void Robot::setBattery(float percent){
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot setBattery() function was called" << std::endl;
    if(percent <= 100 && percent >= 0){
        this->battery_ = percent;
    } else if(percent > 100) {
        this->battery_ = 100;
    } else if(percent < 0) {
        this->battery_ = 0;
    }
};

bool Robot::hasFailed() {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot hasFailed() function was called" << std::endl;
    return battery_ == 0 || this->getFailed();
};

void Robot::setBusy(bool status) {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot setBusy() function was called" << std::endl;
    this->busy = status;
};

void Robot::setFailed(bool status) {
    std::ofstream file;
    file.open(filename, std::ofstream::app);
    file << "Robot setFailed() function was called" << std::endl;
    this->failed = status;
};

bool Robot::isRoomClean() {
    std::ofstream file;
    file.open("logfile.csv", std::ofstream::app);
    file << "Robot isRoomClean() function was called" << std::endl;
    if (this->getJob() == Job::SCRUBBER) {
        if ((this->getLocation())->getScrubbable()) {
            if (this->getLocation()->getPercentScrubbed() == 100) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    } else if(this->getJob() == Job::MOPPER) {
        if (this->getLocation()->getMoppable()) {
            if (this->getLocation()->getPercentMopped() == 100) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    } 
    else if(this->getJob() == Job::VACUUMER) {
        if (this->getLocation()->getVacuumable()) {
            if (this->getLocation()->getPercentVacuumed() == 100) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    }
    
    
    else {
        if (this->getLocation()->getSweepable()) {
            if (this->getLocation()->getPercentSwept() == 100) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    }
};

void Robot::clean() {
    std::ofstream file;
    file.open("logfile.csv", std::ofstream::app);
    file << "Robot clean() function was called" << std::endl;
    std::srand(std::time(0));
    double chanceFailed = ((double)std::rand()) / RAND_MAX;
    if (chanceFailed < 0.01) {
        this->setFailed(true);
    }
    if (!this->hasFailed()) {
        if (this->getJob() == Job::SWEEPER) {
            if (this->getLocation()->getSweepable()) {
                if (this->getSize() == Size::SMALL) {
                    float percentOfRoomCleaned_ = ((5 / (this->getLocation())->getSize()) * 100) + (this->getLocation())->getPercentSwept();
                    if (percentOfRoomCleaned_ < 100) {
                        (this->getLocation())->setPercentSwept(percentOfRoomCleaned_);
                    } else {
                        this->getLocation()->setPercentSwept(100);
                    }
                    this->setBattery(this->getBattery() - 1);
                } else if (this->getSize() == Size::LARGE) {
                    float percentOfRoomCleaned_ = ((12 / (this->getLocation())->getSize()) * 100) + (this->getLocation())->getPercentSwept();
                    if (percentOfRoomCleaned_ < 100) {
                        (this->getLocation())->setPercentSwept(percentOfRoomCleaned_);
                    } else {
                        this->getLocation()->setPercentSwept(100);
                    }
                    this->setBattery(this->getBattery() - 1);
                }
            }
        } 
        else if (this->getJob() == Job::MOPPER) {
            if (this->getLocation()->getMoppable()) {
                if (this->getSize() == Size::SMALL) {
                    float percentOfRoomCleaned_ = ((5 / (this->getLocation())->getSize()) * 100) + (this->getLocation())->getPercentMopped();
                    if (percentOfRoomCleaned_ < 100) {
                        (this->getLocation())->setPercentMopped(percentOfRoomCleaned_);
                    } else {
                        this->getLocation()->setPercentMopped(100);
                    }
                    this->setBattery(this->getBattery() - 1);
                } else if (this->getSize() == Size::LARGE) {
                    float percentOfRoomCleaned_ = ((12 / (this->getLocation())->getSize()) * 100) + (this->getLocation())->getPercentMopped();
                    if (percentOfRoomCleaned_ < 100) {
                        (this->getLocation())->setPercentMopped(percentOfRoomCleaned_);
                    } else {
                        this->getLocation()->setPercentMopped(100);
                    }
                    this->setBattery(this->getBattery() - 1);
                }
            }
        }
            else if (this->getJob() == Job::VACUUMER) {
            if (this->getLocation()->getVacuumable()) {
                if (this->getSize() == Size::SMALL) {
                    float percentOfRoomCleaned_ = ((5 / (this->getLocation())->getSize()) * 100) + (this->getLocation())->getPercentVacuumed();
                    if (percentOfRoomCleaned_ < 100) {
                        (this->getLocation())->setPercentVacuumed(percentOfRoomCleaned_);
                    } else {
                        this->getLocation()->setPercentVacuumed(100);
                    }
                    this->setBattery(this->getBattery() - 1);
                } else if (this->getSize() == Size::LARGE) {
                    float percentOfRoomCleaned_ = ((12 / (this->getLocation())->getSize()) * 100) + (this->getLocation())->getPercentVacuumed();
                    if (percentOfRoomCleaned_ < 100) {
                        (this->getLocation())->setPercentVacuumed(percentOfRoomCleaned_);
                    } else {
                        this->getLocation()->setPercentVacuumed(100);
                    }
                    this->setBattery(this->getBattery() - 1);
                }
            }
        }
        
        
         else {
            if (this->getLocation()->getScrubbable()) {
                if (this->getSize() == Size::SMALL) {
                    float percentOfRoomCleaned_ = ((5 / (this->getLocation())->getSize()) * 100) + (this->getLocation())->getPercentScrubbed();
                    if (percentOfRoomCleaned_ < 100) {
                        (this->getLocation())->setPercentScrubbed(percentOfRoomCleaned_);
                    } else {
                        this->getLocation()->setPercentScrubbed(100);
                    }
                    this->setBattery(this->getBattery() - 1);
                } else if (this->getSize() == Size::LARGE) {
                    float percentOfRoomCleaned_ = ((12 / (this->getLocation())->getSize()) * 100) + (this->getLocation())->getPercentScrubbed();
                    if (percentOfRoomCleaned_ < 100) {
                        (this->getLocation())->setPercentScrubbed(percentOfRoomCleaned_);
                    } else {
                        this->getLocation()->setPercentScrubbed(100);
                    }
                    this->setBattery(this->getBattery() - 1);
                }
            }
        }
    } else {
        std::cout << this->getName() << " tried to clean " << (this->getLocation())->getName() << " but it has failed." << std::endl;
    }
};













// //for location in room (x, y) coordinates 
// struct Point {
//     int x;
//     int y;
// //constructor 
//     Point(int x_0 = 0, int y_0 = 0) : 
//     x(x_0), 
//     y(y_0) 
//     {}
    

//     // Method to set the coordinates
//     void set(int X_n, int Y_n) {
//         x = X_n;
//         y = Y_n;
//     }

//     // display coordinates
//     void display() const {
//         cout << "(" << x << ", " << y << ")" << endl;
//     }

// };
