#include "libclean/manager.hpp"
#include <fstream>
#include <iostream>


int main() {

    std::ofstream logfile;
    // opens an existing csv file or creates a new file. 
    logfile.open("logfile.csv", std::ofstream::app); 
    // logs the action of creating the file 
    logfile << "Log file created" << std::endl; 
    std::cout << "Log file created" << std::endl;

      // Create a new manager:
    Manager manager("logfile.csv");

  

    //    Testing Room Constructor: passing the csv file to log the actions
    // Create a new Room called the office
    Room officeObject {"Office", 16.0, 12.5, true, true, true, "logfile.csv"};
    Room* office = &officeObject;
    // Create a new Room called the atrium
    Room atriumObject {"Atrium", 160, 120.5, false, true, true, "logfile.csv"};
    Room* atrium = &atriumObject;
    // Create a new Room called the csMajorLab
    Room csMajorLabObject {"CSMajorLab", 20, 10, true, false, false, "logfile.csv"};
    Room* csMajorLab = &csMajorLabObject;
    


    

    // Test: getSize
    // returns the size of the room which is the length * width
    std::cout << "The size of the office is: " << office->getSize() << std::endl;

    // Test: setPercentSwept
    // sets the percent swept of the room 
    std::cout << office->getPercentSwept() << "% of the office has been swept." << std::endl;
    office->setPercentSwept(20); // -- 20 % of the office has been swept
    std::cout << office->getPercentSwept() << "% of the office has been swept." << std::endl;

    // Test: setPercentMopped
    // sets the percent mopped of the room 
    std::cout << office->getPercentMopped() << "% of the office has been mopped." << std::endl;
    office->setPercentMopped(15); // -- 15 % of the office has been mopped
    std::cout << office->getPercentMopped() << "% of the office has been swept." << std::endl;

    // Test: setPercentScrubbed
    // sets the percent scrubbed of the room
    std::cout << office->getPercentScrubbed() << "% of the office has been srubbed." << std::endl;
    office->setPercentScrubbed(100); // -- 100 % of the office has been scrubbed
    std::cout << office->getPercentScrubbed() << "% of the office has been scrubbed." << std::endl;

    // Test: randomlyDirty
    //Check to make sure randomlyDirty changed the percentage:
    office->randomlyDirty();
    std::cout << "After one cycle of randomlyDirty:" << std::endl;
    std::cout << office->getPercentSwept() << "% of the office has been swept." << std::endl;
    std::cout << office->getPercentMopped() << "% of the office has been mopped." << std::endl;
    std::cout << office->getPercentScrubbed() << "% of the office has been srubbed." << std::endl;

    std::cout << "After another cycle of randomlyDirty:" << std::endl;
    office->randomlyDirty(); 
    std::cout << office->getPercentScrubbed() << "% of the office has been scrubbed." << std::endl;

//  Testing Robot Constructor: Scrubber
    // Creating a Scrubber robot called Robot1.
    Scrubber Robot1 {"Robot1", 100, Size::SMALL, office, "logfile.csv"};
    manager.viewRobotStatus(Robot1);

//  Testing Robot Constructor: Sweeper
    // Creating a Sweeper robot called Robot2.
    Sweeper Robot2 {"Robot2", 100, Size::LARGE, csMajorLab, "logfile.csv"};
    manager.viewRobotStatus(Robot2);



    // Making the robot sweep the room it is assigned to/in.
    std::cout << office->getPercentSwept() << "% of the csMajorLab has been swept." << std::endl;
    Robot2.sweep(); // -- Robot2 sweeps the csMajorLab
    std::cout << "After Robot2 swept: " << office->getPercentSwept() << "% of the csMajorLab has been swept." << std::endl;

//  Testing Robot Constructor: Mopper
    // Creating a Mopper robot called Robot3
    Mopper Robot3 {"Robot3", 100, Size::LARGE, atrium, "logfile.csv"};
    manager.viewRobotStatus(Robot3);

    // Making the robot mop the room it is assigned to/in.
    std::cout << atrium->getPercentMopped() << "% of the atrium has been mopped." << std::endl;
    Robot3.mop(); // -- Robot3 mops the atrium
    std::cout << "After Robot3 mopped: " << office->getPercentMopped() << "% of the atrium has been mopped." << std::endl;

    // Test : move
    // Moves the robot to the specified room
    Robot1.move(atrium); // -- moves Robot1 to the atrium from the office
    std::cout << "New location of Robot1: " << Robot1.getLocation()->getName() << std::endl;

    // Making the robot scrub the room it is assigned to/in.
    std::cout << office->getPercentScrubbed() << "% of the office has been scrubbed." << std::endl;
    Robot1.scrub(); // -- Robot1 scrubs the office
    std::cout << "After Robot1 scrubs: " << office->getPercentScrubbed()  << "% of the office has been mopped." << std::endl;

    // Test : charge
    std::cout << "Before Charging: " << Robot1.getBattery() << std::endl;
    Robot1.charge(); // -- Robot1 battery will go up to 100%
    std::cout << "After Charging: " << Robot1.getBattery() << std::endl;

    // Test : setName
    // Changes the name of the robot
    Robot2.setName("Martin"); // -- Robot2 name is now Martin
    std::cout << "Robot2 new name: " << Robot2.getName() << std::endl;

    // Test : hasFailed
    // Should always return false - not implemented yet
    std::cout << "Robot2 fail status: " << Robot2.hasFailed() << std::endl;

    // Test : isRoomClean
    // Returns true if the room is clean according to that robots type
    std::cout << "The csMajorLab status: " << Robot2.isRoomClean() << std::endl;
    std::cout << "The csMajorLab percent swept: " << csMajorLab->getPercentSwept() << std::endl;
    csMajorLab->randomlyDirty();
    std::cout << "After a cycle of randomlyDirty: " << std::endl;
    std::cout << "The csMajorLab percent swept: " << csMajorLab->getPercentSwept() << std::endl;
    std::cout << "The csMajorLab status: " << Robot2.isRoomClean() << std::endl;

//   Testing Manager:

    // Test: viewRobotStatus
    manager.viewRobotStatus(Robot1);

    // Test: viewLocation
    // Prints the name of the room, what can be done to it, and the room status
    manager.viewLocation(office);

    // Test: assignRobot
    // Assigns a robot to a specific room
    manager.assignRobot(Robot1, office); // -- Manager is assigning Robot1 to the office from the atrium

    // Making sure assignRobot changed location of Robot1:
    manager.viewRobotStatus(Robot1);

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "BEGIN: SECOND ROUND OF TESTING!" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "BEGIN: TESTING FLEET" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

// Testing Fleet:
    // Fleet constructor
    std::cout << "Constructing a new fleet called 'fleet': " << std::endl;
    Fleet fleet{"logfile.csv"};

    std::cout << std::endl;
    std::cout << std::endl;

    // std::vector<Robot*> getFleet();
    std::cout << "This is testing displayFleet() from manager class which uses getFleet in fleet class:" << std::endl;
    std::cout << "FLeet should be empty: " << std::endl;
    manager.displayFleet(fleet);

    std::cout << std::endl;
    std::cout << std::endl;

    // addToFleet(Robot* robot)
    std::cout << "Adding a robot to the fleet using addToFleet: " << std::endl;
    Robot testingBot1Object{"Testing Bot1", 90, Size::SMALL, office, "logfile.csv"};
    Robot* testingBot1 = &testingBot1Object;
    fleet.addToFleet(testingBot1);
    std::cout << "Fleet should contain: 'Testing Bot1'" << std::endl;
    manager.displayFleet(fleet);

    std::cout << std::endl;
    std::cout << std::endl;

    // updateVectors(Robot* robot)
    // std::vector<Robot*> getAvailableRobots();
    // std::vector<Robot*> getBusyRobots();
    fleet.updateVectors(testingBot1);
    manager.displayAvailableRobots(fleet);
    manager.displayBusyRobots(fleet);

    std::cout << std::endl;

    std::cout << "testingBot1 is not busy so it should be 0" << std::endl;
    std::cout << "testingBot1 is: " << testingBot1->getBusy() << std::endl;

    std::cout << std::endl;

    std::cout << "Setting testingBot1 status to busy: " << std::endl;
    testingBot1->setBusy(true);
    std::cout << "testingBot1 is: " << testingBot1->getBusy() << std::endl;

    std::cout << std::endl;

    fleet.updateVectors(testingBot1);
    manager.displayAvailableRobots(fleet);
    manager.displayBusyRobots(fleet);

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "BEGIN: TESTING TECHNICIAN" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
// Testing Technician:
    // Technician constructor
    Technician technician1{"logfile.csv"};

    std::cout << "Is the technician busy? " << technician1.isTechBusy() << std::endl;
    std::cout << "This means that there are no broken robots." << std::endl;
    std::cout << std::endl;

    // technicianFixesRobot()
    // addRobotToQueue(Robot* robot)
    // isTechBusy()

    std::cout << "Has testingBot1 failed? " << testingBot1->getFailed() << std::endl;

    std::cout << std::endl;

    testingBot1->setFailed(true);
    testingBot1->setBattery(0);

    std::cout << "Has testingBot1 failed? " << testingBot1->getFailed() << std::endl;

    std::cout << std::endl;

    technician1.addRobotToQueue(testingBot1);
    std::cout << "Is the technician busy? " << technician1.isTechBusy() << std::endl;
    std::cout << "This means that there are broken robots to be fixed." << std::endl;
    std::cout << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "BEGIN: TESTING BUILDING" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

// Testing Building: 
    // Building constructor
    std::cout << "Constructing a building called building." << std::endl;
    Building building{"logfile.csv"};

    std::cout << std::endl;

    // std::vector<Room*> getBuilding();
    std::cout << "Creating a room called Classroom and adding it to the building." << std::endl;
    Room classroomObject{"Classroom", 16, 12, true, false, true, "logfile.csv"};
    Room* classroom = &classroomObject;

    std::cout << std::endl;

    building.addRoom(classroom);
    manager.displayAllRooms(building);

    std::cout << std::endl;

    // std::vector<Room*> getDirtyRooms();
    manager.displayDirtyRooms(building);

    std::cout << std::endl;

    // std::vector<Room*> getCleanRooms();
    manager.displayCleanRooms(building);

    std::cout << std::endl;

    std::cout << "Making Classroom dirty." << std::endl;
    classroom->randomlyDirty();

    manager.displayDirtyRooms(building);

    std::cout << std::endl;

    // std::vector<Room*> getCleanRooms();
    manager.displayCleanRooms(building);
    logfile.close();
};

