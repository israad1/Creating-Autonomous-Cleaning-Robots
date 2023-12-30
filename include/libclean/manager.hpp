#ifndef LIBCLEAN_MANAGER_HPP
#define LIBCLEAN_MANAGER_HPP

#include "libclean/room.hpp"
#include "libclean/robot.hpp"
#include "libclean/fleet.hpp"
#include "libclean/building.hpp"
#include "libclean/technician.hpp"

class Manager{
    private:
        std::string filename = "default.csv";

    public:
        Manager(const std::string& filename);
        void viewRobotStatus(Robot* robot);
        void viewLocation(Room* room);

        void assignRobot(Robot* robot, Room *room);

        bool callTech(Robot* robot, Technician& tech);

        void displayDirtyRooms(Building building);
        void displayCleanRooms(Building building);
        void displayAllRooms(Building building);

        void displayBusyRobots(Fleet fleet);
        void displayAvailableRobots(Fleet fleet);
        void displayFleet(Fleet fleet);
};

#endif