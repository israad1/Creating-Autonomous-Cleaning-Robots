#ifndef LIBCLEAN_FLEET_HPP
#define LIBCLEAN_FLEET_HPP

#include "libclean/robot.hpp"
#include <string> 
#include <math.h> 
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

class Fleet{
    private:
      std::vector<Robot*> fleet;
      std::string filename = "default.csv";
      std::vector<Robot*> availableRobots;
      std::vector<Robot*> busyRobots;

    public:
      Fleet(const std::string& filename);

      void updateVectors(Robot* robot);
      void addToFleet(Robot* robot);
      std::vector<Robot*> getFleet();
      std::vector<Robot*> getAvailableRobots();
      std::vector<Robot*> getBusyRobots();
};

#endif