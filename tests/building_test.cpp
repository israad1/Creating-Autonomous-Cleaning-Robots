#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <stdexcept>
#include "libclean/manager.hpp"
#include "libclean/fleet.hpp"
#include "libclean/robot.hpp" 
#include "libclean/room.hpp"
#include "libclean/building.hpp"
#include "libclean/timer.hpp"

TEST_CASE("Building Tests"){
    Manager manager("logfile.csv");

    Building building("logfile.csv");

    Fleet fleet("logfile.csv");

    Room baseObject {"Base", 0, 0, false, false, false, "logfile.csv"};
    Room* base = &baseObject;

    Room officeObject {"Office", 16.0, 12.5, true, true, true, "logfile.csv"};
    Room* office = &officeObject;

    Robot scrubberObject {"Scrubber", 100, Size::SMALL, base, "logfile.csv", Job::SCRUBBER};
    Robot* scrubber = &scrubberObject;
    fleet.addToFleet(scrubber);

    Robot mopperObject {"Mopper", 100, Size::LARGE, base, "logfile.csv", Job::MOPPER};
    Robot* mopper = &mopperObject;
    fleet.addToFleet(mopper);

    Robot sweeperObject {"Sweeper", 100, Size::SMALL, base, "logfile.csv", Job::SWEEPER};
    Robot* sweeper = &sweeperObject;
    fleet.addToFleet(sweeper);

    SECTION("Adding a room to building") {
    // addRoom() and getBuilding() and getDirtyRooms() and getCleanRooms()
        CHECK( building.getBuilding().empty() );
        CHECK( building.getDirtyRooms().empty() );
        CHECK( building.getCleanRooms().empty() );
        building.addRoom(office);
        CHECK( !building.getBuilding().empty() );
        CHECK( building.getDirtyRooms().empty() );
        CHECK( !building.getCleanRooms().empty() );
    }
    SECTION("Getters") {
    // getBuilding() and getBuilding() and getDirtyRooms() and getCleanRooms()
        CHECK( building.getBuilding().empty() );
        CHECK( building.getDirtyRooms().empty() );
        CHECK( building.getCleanRooms().empty() );

        building.addRoom(office);
        office->randomlyDirty();
        CHECK( !building.getBuilding().empty() );
        CHECK( !building.getDirtyRooms().empty() );
        CHECK( building.getCleanRooms().empty() );

        office->setPercentMopped(90);
        office->setPercentScrubbed(90);
        office->setPercentSwept(90);
        manager.assignRobot(scrubber, office);
        manager.assignRobot(mopper, office);
        manager.assignRobot(sweeper, office);
        while(office->getPercentMopped() != 100) {
            mopper->clean();
        }
        while(office->getPercentScrubbed() != 100) {
            scrubber->clean();
        }
        while(office->getPercentSwept() != 100) {
            sweeper->clean();
        }
        manager.viewLocation(office);
        CHECK( !building.getBuilding().empty() );
        CHECK( building.getDirtyRooms().empty() );
        CHECK( !building.getCleanRooms().empty() );
    }
}