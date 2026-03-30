#include "libclean/manager.hpp"
#include "libclean/ui.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

// ─────────────────────────────────────────────────────────────────────────────
//  Helpers
// ─────────────────────────────────────────────────────────────────────────────
static std::string jobStr(Job j) {
    switch (j) {
        case Job::SWEEPER:  return "Sweeper";
        case Job::MOPPER:   return "Mopper";
        case Job::SCRUBBER: return "Scrubber";
        case Job::VACUUMER: return "Vacuumer";
        default:            return "Unknown";
    }
}
static std::string sizeStr(Size s) {
    return s == Size::SMALL ? "Small" : "Large";
}

// ─────────────────────────────────────────────────────────────────────────────
Manager::Manager(const std::string& filename) {
    this->filename = filename;
    std::ofstream file(filename, std::ofstream::app);
    file << "Manager object created\n";
}

// ─────────────────────────────────────────────────────────────────────────────
void Manager::viewRobotStatus(Robot* robot) {
    using namespace UI;
    std::ofstream file(filename, std::ofstream::app);
    file << "Manager viewRobotStatus() was called\n";

    section("ROBOT  ─  " + robot->getName(), BMAG);

    bool failed  = robot->hasFailed();
    bool busy    = robot->getBusy();
    float batt   = robot->getBattery();

    std::cout << "  " << BOLD << pcol("Name",     14) << RST << robot->getName()              << "\n"
              << "  " << BOLD << pcol("ID",        14) << RST << robot->getID()                << "\n"
              << "  " << BOLD << pcol("Type",      14) << RST << jobStr(robot->getJob())        << "\n"
              << "  " << BOLD << pcol("Size",      14) << RST << sizeStr(robot->getSize())      << "\n"
              << "  " << BOLD << pcol("Battery",   14) << RST
              << battBar(batt, 14) << "  " << std::fixed << std::setprecision(1) << batt << "%\n"
              << "  " << BOLD << pcol("Location",  14) << RST << robot->getLocation()->getName() << "\n"
              << "  " << BOLD << pcol("Status",    14) << RST << statusBadge(busy, failed, false) << "\n\n";
}

// ─────────────────────────────────────────────────────────────────────────────
void Manager::viewLocation(Room* room) {
    using namespace UI;
    std::ofstream file(filename, std::ofstream::app);
    file << "Manager viewLocation() was called\n";

    section("ROOM  ─  " + room->getName(), BBLU);

    float sz = room->getSize();
    std::cout << "  " << BOLD << pcol("Name",     14) << RST << room->getName()                 << "\n"
              << "  " << BOLD << pcol("Size",     14) << RST
              << std::fixed << std::setprecision(1) << sz << " m²\n";

    // Capabilities + cleanliness bars
    auto printMetric = [&](const std::string& label, bool capable, float pct) {
        std::cout << "  " << BOLD << pcol(label, 14) << RST;
        if (!capable) {
            std::cout << DIM << "not applicable\n" << RST;
        } else {
            std::cout << bar(pct, 14) << "  " << pctStr(pct) << "\n";
        }
    };

    printMetric("Swept",    room->getSweepable(),  room->getPercentSwept());
    printMetric("Mopped",   room->getMoppable(),   room->getPercentMopped());
    printMetric("Scrubbed", room->getScrubbable(), room->getPercentScrubbed());
    printMetric("Vacuumed", room->getVacuumable(), room->getPercentVacuumed());

    float swept    = room->getSweepable()  ? room->getPercentSwept()    : 100.0f;
    float mopped   = room->getMoppable()   ? room->getPercentMopped()   : 100.0f;
    float scrubbed = room->getScrubbable() ? room->getPercentScrubbed() : 100.0f;
    float vacuumed = room->getVacuumable() ? room->getPercentVacuumed() : 100.0f;
    std::cout << "  " << BOLD << pcol("Status", 14) << RST
              << roomStatus(swept, mopped, scrubbed, vacuumed) << "\n\n";
}

// ─────────────────────────────────────────────────────────────────────────────
void Manager::assignRobot(Robot* robot, Room* room) {
    using namespace UI;
    std::ofstream file(filename, std::ofstream::app);
    file << "Manager assignRobot() was called\n";

    // Battery requirement
    float roomSize = room->getSize();
    int   cap      = (robot->getSize() == Size::SMALL) ? 500 : 1200;
    float pctLeft  = 0.0f;
    bool  capable  = false;

    if (robot->getJob() == Job::SWEEPER) {
        capable = room->getSweepable();
        pctLeft = room->getPercentSwept();
    } else if (robot->getJob() == Job::MOPPER) {
        capable = room->getMoppable();
        pctLeft = room->getPercentMopped();
    } else if (robot->getJob() == Job::SCRUBBER) {
        capable = room->getScrubbable();
        pctLeft = room->getPercentScrubbed();
    } else if (robot->getJob() == Job::VACUUMER) {
        capable = room->getVacuumable();
        pctLeft = room->getPercentVacuumed();
    }

    if (!capable) {
        error(robot->getName() + " cannot clean " + room->getName()
              + " (capability mismatch).");
        return;
    }

    float battNeeded = ((100.0f - pctLeft) * roomSize / (float)cap) + 5.0f;

    if (robot->getBattery() < battNeeded) {
        warn(robot->getName() + " may not have enough battery to fully clean "
             + room->getName() + ".  Proceed? (yes / no)");
        std::cout << "  " << BCYN << "fleet> " << RST;
        std::string ans;
        std::cin >> ans;
        if (ans != "yes") {
            info("Assignment cancelled.");
            return;
        }
    }

    robot->move(room);
    robot->setBusy(true);
    success(robot->getName() + " is heading to " + room->getName() + ".");
}

// ─────────────────────────────────────────────────────────────────────────────
bool Manager::callTech(Robot* robot, Technician& tech) {
    std::ofstream file(filename, std::ofstream::app);
    file << "Manager callTech() was called\n";
    return tech.addRobotToQueue(robot);
}

// ─────────────────────────────────────────────────────────────────────────────
void Manager::displayDirtyRooms(Building building) {
    using namespace UI;
    std::ofstream file(filename, std::ofstream::app);
    file << "Manager displayDirtyRooms() was called\n";
    section("DIRTY ROOMS", BRED);
    auto dirty = building.getDirtyRooms();
    if (dirty.empty()) {
        success("All rooms are clean!");
    } else {
        for (Room* r : dirty)
            std::cout << "  " << RED << "\u2022 " << RST << r->getName() << "\n";
    }
    std::cout << "\n";
}

void Manager::displayCleanRooms(Building building) {
    using namespace UI;
    std::ofstream file(filename, std::ofstream::app);
    file << "Manager displayCleanRooms() was called\n";
    section("CLEAN ROOMS", BGRN);
    auto clean = building.getCleanRooms();
    if (clean.empty()) {
        warn("No clean rooms yet.");
    } else {
        for (Room* r : clean)
            std::cout << "  " << GREEN << "\u2022 " << RST << r->getName() << "\n";
    }
    std::cout << "\n";
}

void Manager::displayAllRooms(Building building) {
    using namespace UI;
    std::ofstream file(filename, std::ofstream::app);
    file << "Manager displayAllRooms() was called\n";
    section("ALL ROOMS", BBLU);
    for (Room* r : building.getBuilding())
        std::cout << "  " << CYAN << "\u2022 " << RST << r->getName() << "\n";
    std::cout << "\n";
}

void Manager::displayBusyRobots(Fleet fleet) {
    using namespace UI;
    std::ofstream file(filename, std::ofstream::app);
    file << "Manager displayBusyRobots() was called\n";
    section("WORKING ROBOTS", BYEL);
    auto busy = fleet.getBusyRobots();
    if (busy.empty()) {
        info("No robots are currently working.");
    } else {
        for (Robot* r : busy)
            std::cout << "  " << YELLOW << "\u25b6 " << RST
                      << pcol(r->getName(), 14) << jobStr(r->getJob())
                      << "  in " << r->getLocation()->getName() << "\n";
    }
    std::cout << "\n";
}

void Manager::displayAvailableRobots(Fleet fleet) {
    using namespace UI;
    std::ofstream file(filename, std::ofstream::app);
    file << "Manager displayAvailableRobots() was called\n";
    section("AVAILABLE ROBOTS", BGRN);
    auto avail = fleet.getAvailableRobots();
    if (avail.empty()) {
        warn("No robots are currently available.");
    } else {
        for (Robot* r : avail)
            std::cout << "  " << GREEN << "\u2713 " << RST
                      << pcol(r->getName(), 14) << jobStr(r->getJob())
                      << "  bat:" << std::fixed << std::setprecision(0) << r->getBattery() << "%\n";
    }
    std::cout << "\n";
}

void Manager::displayFleet(Fleet fleet) {
    using namespace UI;
    std::ofstream file(filename, std::ofstream::app);
    file << "Manager displayFleet() was called\n";
    section("FULL FLEET", BMAG);
    std::cout << DIM << "  " << pcol("Name", 14) << pcol("Type", 10)
              << pcol("Size", 8) << "Battery    Status\n" << RST;
    hline(60, MAGENTA);
    for (Robot* r : fleet.getFleet()) {
        std::cout << "  " << BOLD << pcol(r->getName(), 14) << RST
                  << pcol(jobStr(r->getJob()), 10)
                  << pcol(sizeStr(r->getSize()), 8)
                  << battBar(r->getBattery(), 8)
                  << " " << std::setw(4) << std::fixed << std::setprecision(0) << r->getBattery() << "%  "
                  << statusBadge(r->getBusy(), r->hasFailed(), false) << "\n";
    }
    std::cout << "\n";
}
