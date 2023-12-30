#ifndef LIBCLEAN_TECHNICIAN_HPP
#define LIBCLEAN_TECHNICIAN_HPP
#include <deque>
#include "libclean/robot.hpp" // Include the Robot class header
#include <iostream>

class Technician {
    private:
        std::deque<Robot*> brokenRobotQueue;
        std::string filename = "default.csv";

    public:
        Technician(const std::string& filename);
        std::deque<Robot*> getRobotQueue();
        void technicianFixesRobot();
        bool addRobotToQueue(Robot* robot);
        bool isTechBusy() const;
};

#endif // TECHNICIAN_H
