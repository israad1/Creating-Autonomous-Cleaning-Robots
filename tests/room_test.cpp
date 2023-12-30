#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <stdexcept>
#include "libclean/manager.hpp"
#include "libclean/fleet.hpp"
#include "libclean/robot.hpp" 
#include "libclean/room.hpp"
#include "libclean/building.hpp"
#include "libclean/timer.hpp"

TEST_CASE("Room Tests"){
    Timer timer;

    Fleet fleet {"logfile.csv"};

    Building building{"logfile.csv"};

    Room baseObject {"Base", 0, 0, false, false, false, "logfile.csv"};
    Room* base = &baseObject;
    building.addRoom(base);

    Room officeObject {"Office", 16.0, 12.5, true, true, true, "logfile.csv"};
    Room* office = &officeObject;
    building.addRoom(office);

    Room officeObject2 {"Office", 16.0, 12.5, true, true, true, "logfile.csv"};
    Room* office2 = &officeObject2;
    building.addRoom(office2);

    Robot scrubberObject {"Scrubber", 100, Size::SMALL, base, "logfile.csv", Job::SCRUBBER};
    Robot* scrubber = &scrubberObject;

    Robot mopperObject {"Mopper", 100, Size::LARGE, base, "logfile.csv", Job::MOPPER};
    Robot* mopper = &mopperObject;

    Robot sweeperObject {"Sweeper", 100, Size::SMALL, base, "logfile.csv", Job::SWEEPER};
    Robot* sweeper = &sweeperObject;

    Manager manager("logfile.csv");

    Technician technician{"logfile.csv"};

    SECTION("Getters and Setters"){
    // Name : getname()
        CHECK( office->getName() == "Office" );
    // Width : getWidth() - floats are weird use other function
        CHECK( office->getWidth() == 16 );
    // Length : getLength() - floats are weird use other function
        CHECK( office->getLength() == 12.5 );
    // Size : getSize() - floats are weird use other function
        CHECK( office->getSize() == 200 );
    // ID : getID()
        CHECK( office->getID() == 1 );
        CHECK( base->getID() == 0 );
        CHECK( office2->getID() == 2 );
    // Sweep : getSweepable() and getPercentSwept() and setPercentSwept()
        CHECK( office->getSweepable() == true );
        CHECK( office->getPercentSwept() == 100 );
        office->setPercentSwept(96);
        CHECK( office->getPercentSwept() == 96 );

    // do I need to check if we are trying to set a negative percentage?

    // Mop : getMoppable() and getPercentMopped() and setPercentMopped()
        CHECK( office->getMoppable() == true );
        CHECK( office->getPercentMopped() == 100 );
        office->setPercentMopped(96);
        CHECK( office->getPercentMopped() == 96 );
    // Scrub : getScrubbable() and getPercentScrubbed() and setPercentScrubbed()
        CHECK( office->getScrubbable() == true );
        CHECK( office->getPercentScrubbed() == 100 );
        office->setPercentScrubbed(96);
        CHECK( office->getPercentScrubbed() == 96 );
    }

    SECTION("Randomly dirty") {
        office->setPercentMopped(100);
        office->setPercentSwept(100);
        office->setPercentScrubbed(100);
        CHECK( office->getPercentMopped() == 100 );
        CHECK( office->getPercentSwept() == 100 );
        CHECK( office->getPercentScrubbed() == 100 );

        float perMop = office->getPercentMopped();
        float perSweep = office->getPercentSwept();
        float perScrub = office->getPercentScrubbed();
        office->randomlyDirty();

        CHECK( office->getPercentMopped() != 100 );
        CHECK( office->getPercentSwept() != 100 );
        CHECK( office->getPercentScrubbed() != 100 );
        CHECK( office->getPercentMopped() != perMop );
        CHECK( office->getPercentSwept() != perSweep );
        CHECK( office->getPercentScrubbed() != perScrub );
    }

    SECTION("Equals Operator") {
        CHECK( (office == office2) == false ); // different IDs
    }
}