#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <stdexcept>
#include "libclean/manager.hpp"
#include "libclean/fleet.hpp"
#include "libclean/robot.hpp" 
#include "libclean/room.hpp"
#include "libclean/building.hpp"
#include "libclean/timer.hpp"

TEST_CASE("Manager Tests"){
    Fleet fleet {"logfile.csv"};

    Manager manager("logfile.csv");

    Building building("logfile.csv");

    Technician technician("logfile.csv");

    Room baseObject {"Base", 0, 0, false, false, false, "logfile.csv"};
    Room* base = &baseObject;
    building.addRoom(base);

    Room officeObject {"Office", 16.0, 12.5, true, true, true, "logfile.csv"};
    Room* office = &officeObject;
    building.addRoom(office);

    Robot scrubberObject {"Scrubber", 100, Size::SMALL, base, "logfile.csv", Job::SCRUBBER};
    Robot* scrubber = &scrubberObject;
    fleet.addToFleet(scrubber);
    fleet.updateVectors(scrubber);

    SECTION("Viewing") {
        manager.viewRobotStatus(scrubber);
        manager.viewLocation(office);
    }
    SECTION("Displaying") {
        manager.displayAllRooms(building);
        manager.displayDirtyRooms(building);
        manager.displayCleanRooms(building);

        manager.displayFleet(fleet);
        manager.displayAvailableRobots(fleet);
        manager.displayBusyRobots(fleet);
    }
    SECTION("Calling technician") {
        CHECK( technician.getRobotQueue().empty() );
        manager.callTech(scrubber, technician);
        scrubber->setFailed(true);
        manager.callTech(scrubber, technician);
        CHECK( !technician.getRobotQueue().empty() );
    }
    SECTION("Assign Robot") {
        CHECK( scrubber->getLocation() == base );
        CHECK( scrubber->getBattery() == 100 );
        CHECK( scrubber->getBusy() == false );
        manager.assignRobot(scrubber, office);
        CHECK( scrubber->getLocation() == office );
        CHECK( scrubber->getBattery() == 95 );
        CHECK( scrubber->getBusy() == true );
    }
    SECTION("Trying to assign a robot that will failed while cleaning") {
        Robot mopperObject {"Mopper", 60, Size::SMALL, base, "logfile.csv", Job::MOPPER};
        Robot* mopper = &mopperObject;
        fleet.addToFleet(mopper);
        fleet.updateVectors(mopper); 

        Room classroomObject {"Classroom", 160.0, 120.5, true, true, true, "logfile.csv"};
        Room* classroom = &classroomObject;
        building.addRoom(classroom);
        classroom->randomlyDirty();
        manager.assignRobot(mopper, classroom);
        while (classroom->getPercentMopped() != 100) {
            if (!mopper->hasFailed()) {
                mopper->clean();
            } else {
                break;
            }
        }
        CHECK( mopper->hasFailed() == true );
    }
}