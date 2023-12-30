#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <stdexcept>
#include "libclean/manager.hpp"
#include "libclean/fleet.hpp"
#include "libclean/robot.hpp" 
#include "libclean/room.hpp"
#include "libclean/building.hpp"
#include "libclean/timer.hpp"

TEST_CASE("Technician Tests"){
    Manager manager("logfile.csv");

    Building building("logfile.csv");

    Fleet fleet("logfile.csv");

    Technician technician("logfile.csv");

    Room baseObject {"Base", 0, 0, false, false, false, "logfile.csv"};
    Room* base = &baseObject;

    Room officeObject {"Office", 16.0, 12.5, true, true, true, "logfile.csv"};
    Room* office = &officeObject;

    Robot scrubberObject {"Scrubber", 100, Size::SMALL, base, "logfile.csv", Job::SCRUBBER};
    Robot* scrubber = &scrubberObject;
    fleet.addToFleet(scrubber);
    fleet.updateVectors(scrubber);

    Robot mopperObject {"Mopper", 100, Size::LARGE, base, "logfile.csv", Job::MOPPER};
    Robot* mopper = &mopperObject;
    fleet.addToFleet(mopper);
    fleet.updateVectors(mopper);

    Robot sweeperObject {"Sweeper", 100, Size::SMALL, base, "logfile.csv", Job::SWEEPER};
    Robot* sweeper = &sweeperObject;
    fleet.addToFleet(sweeper);
    fleet.updateVectors(sweeper);

    SECTION("Technician fixes robot") {
    // technicianFixesRobot() and addRobotToQueue() and isTechBusy()
        CHECK( technician.getRobotQueue().empty() );
        CHECK( technician.isTechBusy() == false );
        mopper->setBattery(0);
        CHECK( mopper->hasFailed() == true );
        manager.callTech(mopper, technician);
        CHECK( !technician.getRobotQueue().empty() );
        CHECK( technician.isTechBusy() == true );

        technician.technicianFixesRobot();
        CHECK( technician.getRobotQueue().empty() );
    }
}