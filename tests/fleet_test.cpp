#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <stdexcept>
#include "libclean/manager.hpp"
#include "libclean/fleet.hpp"
#include "libclean/robot.hpp" 
#include "libclean/room.hpp"
#include "libclean/building.hpp"
#include "libclean/timer.hpp"

TEST_CASE("Fleet Tests"){
    Fleet fleet {"logfile.csv"};

    Manager manager("logfile.csv");

    Room baseObject {"Base", 0, 0, false, false, false, "logfile.csv"};
    Room* base = &baseObject;

    Room officeObject {"Office", 16.0, 12.5, true, true, true, "logfile.csv"};
    Room* office = &officeObject;

    Robot scrubberObject {"Scrubber", 100, Size::SMALL, base, "logfile.csv", Job::SCRUBBER};
    Robot* scrubber = &scrubberObject;

    SECTION("Updating Vectors") {
    // addToFleet() and updateVectors() and getFleet() and getAvailableRobots()
        CHECK( fleet.getFleet().empty() );
        CHECK( fleet.getBusyRobots().empty() );
        CHECK( fleet.getAvailableRobots().empty() );
        fleet.addToFleet(scrubber);
        fleet.updateVectors(scrubber);
        manager.displayFleet(fleet);
        CHECK( !fleet.getFleet().empty() );
        CHECK( fleet.getBusyRobots().empty() );
        CHECK( !fleet.getAvailableRobots().empty() );
    }
    SECTION("Getters") {
    // getBusyRobots() and getAvailableRobots()
        CHECK( fleet.getFleet().empty() );

        fleet.addToFleet(scrubber);
        fleet.updateVectors(scrubber);

        manager.assignRobot(scrubber, office);
        fleet.updateVectors(scrubber);
        CHECK( !fleet.getFleet().empty() );
        CHECK( !fleet.getBusyRobots().empty() );
        CHECK( fleet.getAvailableRobots().empty() );
    }
}