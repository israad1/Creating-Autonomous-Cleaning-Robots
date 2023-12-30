#ifndef LIBCLEAN_ROBOT_HPP
#define LIBCLEAN_ROBOT_HPP

#include "libclean/room.hpp"

#include <string> 
#include <math.h> 
#include <iostream>
#include <chrono>
#include <thread>

enum class Size{SMALL, LARGE};
enum class Job{SWEEPER, MOPPER, SCRUBBER, VACUUMER};

class Robot {
    private:
        std::string name;
        std::string filename = "default.csv";
        int id;
        float battery_;
        Size size;
        Room* location;
        bool busy;
        bool failed;
        Job job;
    public:
        static int numberOfRobots;

        Robot(std::string name, float battery, Size size, Room* location, const std::string& filename, Job job);

        bool operator==(const Robot& robot);

        std::string getName();
        int getID();
        float getBattery();
        Size getSize();
        Room* getLocation();
        bool getBusy();
        bool getFailed();
        Job getJob();

        void move(Room* room);
        void charge();
        void setLocation(Room* room);
        void setName(std::string name);
        void setBattery(float percent);
        void setBusy(bool status);
        void setFailed(bool status);
        bool hasFailed();
        bool isRoomClean();
        void clean();
};

#endif