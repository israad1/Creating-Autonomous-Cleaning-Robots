#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <stdexcept>
#include "libclean/manager.hpp"
#include "libclean/fleet.hpp"
#include "libclean/robot.hpp" 
#include "libclean/room.hpp"
#include "libclean/building.hpp"
#include "libclean/timer.hpp"

TEST_CASE("Robot Tests"){
    Timer timer;

    Fleet fleet {"logfile.csv"};

    Building building{"logfile.csv"};

    Room baseObject {"Base", 0, 0, false, false, false, "logfile.csv"};
    Room* base = &baseObject;
    building.addRoom(base);

    Robot scrubberObject {"Scrubber", 100, Size::SMALL, base, "logfile.csv", Job::SCRUBBER};
    Robot* scrubber = &scrubberObject;

    Robot mopperObject {"Mopper", 100, Size::LARGE, base, "logfile.csv", Job::MOPPER};
    Robot* mopper = &mopperObject;

    Robot sweeperObject {"Sweeper", 100, Size::SMALL, base, "logfile.csv", Job::SWEEPER};
    Robot* sweeper = &sweeperObject;

    Room officeObject {"Office", 16.0, 12.5, true, true, true, "logfile.csv"};
    Room* office = &officeObject;
    building.addRoom(office);

    Manager manager("logfile.csv");

    Technician technician{"logfile.csv"};

    SECTION("Adding robots to fleet and getters and setters"){
        fleet.addToFleet(scrubber);
        fleet.updateVectors(scrubber);

        CHECK( !fleet.getFleet().empty() );

        fleet.addToFleet(mopper);
        fleet.updateVectors(mopper);
        fleet.addToFleet(sweeper);
        fleet.updateVectors(sweeper);

        CHECK( fleet.getFleet().size() == 3);

        manager.viewRobotStatus(scrubber);
        manager.displayFleet(fleet);
        CHECK( scrubber->getID() == 0 );
        CHECK( mopper->getID() == 1 );
        CHECK( sweeper->getID() == 2 );

    // Name : getName() and setName(string)
        CHECK( mopper->getName() == "Mopper" );
        mopper->setName("Mr.Mops");
        CHECK( mopper->getName() == "Mr.Mops" );

    // Battery : getBattery() and setBattery(float)
        CHECK( mopper->getBattery() == 100 );
        mopper->setBattery(50);
        CHECK( mopper->getBattery() == 50 );

    // Size : getSize()
        CHECK( mopper->getSize() == Size::LARGE );
        CHECK( scrubber->getSize() == Size::SMALL );

    // Location : getLocation() and setLocation()
        CHECK( mopper->getLocation() == base );
        mopper->setLocation(office);
        CHECK( mopper->getLocation() == office );

    // Busy : getBusy() and setBusy(bool) 
        CHECK( mopper->getBusy() == false );
        mopper->setBusy(true);
        CHECK( mopper->getBusy() == true );

    // Failed : getFailed() and setFailed() and hasFailed()
        CHECK( mopper->getFailed() == false );
        CHECK( mopper->hasFailed() == false );
        mopper->setFailed(true);
        CHECK( mopper->getFailed() == true );
        CHECK( mopper->hasFailed() == true );

    // Job : getJob()
        CHECK( mopper->getJob() == Job::MOPPER );
        CHECK( scrubber->getJob() == Job::SCRUBBER );
        CHECK( sweeper->getJob() == Job::SWEEPER );
    }

    SECTION("Moving a robot"){
    // Battery lowers by 5 after moving and the location of the robot changes correctly
        CHECK( scrubber->getBattery() == 100 );
        CHECK( scrubber->getLocation() == base );
        scrubber->move(office);
        CHECK( scrubber->getBattery() == 95 );
        CHECK( scrubber->getLocation() == office );
    // Checking if you cannot move a robot if it's battery is too low
        scrubber->setBattery(3);
        scrubber->move(base);    
        CHECK( scrubber->getLocation() == office );
        CHECK( scrubber->getBattery() == 3 );

        scrubber->setBattery(5);
        scrubber->move(base);    
        CHECK( scrubber->getLocation() == office );
        CHECK( scrubber->getBattery() != 0 );
    }

    SECTION("Charging a robot"){
    // Charge : charge()
        scrubber->setBattery(10);
        scrubber->charge();
        CHECK( scrubber->getBattery() == 15 );
        scrubber->charge();
        CHECK( scrubber->getBattery() == 20 );
    // If battery is going to go over 100 if it corrects to 100
        scrubber->setBattery(98);
        scrubber->charge();
        CHECK( scrubber->getBattery() == 100 );
    }

    SECTION("Cleaning a room"){
    // Clean : isRoomClean() and clean()
        // Scrubber
        scrubber->move(office);
        CHECK( scrubber->isRoomClean() == true );
        CHECK( office->getPercentScrubbed() == 100 );
        office->randomlyDirty();
        CHECK( scrubber->isRoomClean() == false );
        int perscrub = office->getPercentScrubbed();
        CHECK( office->getPercentScrubbed() != 100 );
        scrubber->clean();
        CHECK( office->getPercentScrubbed() != perscrub );

        office->setPercentMopped(100);
        // Mopper
        mopper->move(office);
        CHECK( mopper->isRoomClean() == true );
        CHECK( office->getPercentMopped() == 100 );
        office->randomlyDirty();
        CHECK( mopper->isRoomClean() == false );
        int permop = office->getPercentMopped();
        CHECK( office->getPercentMopped() != 100 );
        mopper->clean();
        CHECK( office->getPercentMopped() != permop );

        office->setPercentSwept(100);
        // Sweeper
        sweeper->move(office);
        CHECK( sweeper->isRoomClean() == true );
        CHECK( office->getPercentSwept() == 100 );
        office->randomlyDirty();
        CHECK( sweeper->isRoomClean() == false );
        int persweep = office->getPercentSwept();
        CHECK( office->getPercentSwept() != 100 );
        sweeper->clean();
        CHECK( office->getPercentSwept() != persweep );
    }
}