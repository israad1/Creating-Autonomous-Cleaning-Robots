#ifndef LIBCLEAN_BUILDING_HPP
#define LIBCLEAN_BUILDING_HPP

#include "libclean/room.hpp"
#include <string> 
#include <math.h> 
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

class Building{
    private:
      std::vector<Room*> building;
      std::string filename = "default.csv";

    public:
      Building(const std::string& filename);

      void addRoom(Room* room);
      std::vector<Room*> getBuilding();
      std::vector<Room*> getDirtyRooms();
      std::vector<Room*> getCleanRooms();
};

#endif