// ─────────────────────────────────────────────────────────────────────────────
//  CleanBot Fleet Manager  ─  main.cpp
//  Interactive CLI for coordinating autonomous cleaning robots.
//
//  New in v2.0
//    • Rich colour terminal UI (ANSI/Unicode)
//    • dashboard  – full system overview at a glance
//    • auto       – AI-style smart auto-assignment
//    • stats      – session analytics
//    • priority   – mark a room as high-priority for auto-assign
//    • Fixed Room constructor calls (vacuumable parameter)
//    • Fixed uninitialized waitingRobot crash
// ─────────────────────────────────────────────────────────────────────────────
#include "libclean/manager.hpp"
#include "libclean/fleet.hpp"
#include "libclean/robot.hpp"
#include "libclean/room.hpp"
#include "libclean/building.hpp"
#include "libclean/timer.hpp"
#include "libclean/ui.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iomanip>
#include <cmath>

// ─────────────────────────────────────────────────────────────────────────────
//  Session statistics
// ─────────────────────────────────────────────────────────────────────────────
struct Stats {
    int assignments     = 0;
    int completedCleans = 0;
    int failures        = 0;
    int techCalls       = 0;
    int autoRuns        = 0;
};

// ─────────────────────────────────────────────────────────────────────────────
//  String split
// ─────────────────────────────────────────────────────────────────────────────
static std::vector<std::string> split(const std::string& s, const std::string& delim) {
    std::vector<std::string> out;
    std::size_t cur = 0;
    while (cur < s.size()) {
        std::size_t start = s.find_first_not_of(delim, cur);
        if (start == std::string::npos) break;
        std::size_t end = s.find_first_of(delim, start);
        out.push_back(s.substr(start, end - start));
        cur = (end == std::string::npos) ? s.size() : end;
    }
    return out;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Job / Size to string helpers
// ─────────────────────────────────────────────────────────────────────────────
static std::string jobStr(Job j) {
    switch (j) {
        case Job::SWEEPER:  return "Sweeper ";
        case Job::MOPPER:   return "Mopper  ";
        case Job::SCRUBBER: return "Scrubber";
        case Job::VACUUMER: return "Vacuumer";
        default:            return "Unknown ";
    }
}
static std::string sizeStr(Size s) {
    return s == Size::SMALL ? "Small" : "Large";
}

// ─────────────────────────────────────────────────────────────────────────────
//  Dashboard  ─  full system overview
// ─────────────────────────────────────────────────────────────────────────────
static void showDashboard(Fleet& fleet, Building& building, Timer& timer,
                          Room* base,
                          const std::unordered_set<std::string>& priorityRooms) {
    using namespace UI;
    thickLine(72, BCYN);
    std::cout << BCYN << BOLD
              << "  FLEET DASHBOARD"
              << RST << DIM << "                            Time: "
              << RST << BYEL << BOLD << timer.getTime() << RST << "\n";
    thickLine(72, BCYN);

    // ── Robots ──────────────────────────────────────────────────
    section("ROBOTS", BMAG);
    std::cout << DIM
              << "  " << pcol("Name",    12)
              << pcol("Type",    10)
              << pcol("Size",    7)
              << "Battery         "
              << pcol("Location", 14)
              << "Status\n" << RST;
    hline(72, MAGENTA);

    for (Robot* r : fleet.getFleet()) {
        bool atBase   = (r->getLocation() == base);
        bool failed   = r->hasFailed();
        bool busy     = r->getBusy();
        float batt    = r->getBattery();

        std::cout << "  "
                  << BOLD << pcol(r->getName(), 12) << RST
                  << pcol(jobStr(r->getJob()), 10)
                  << pcol(sizeStr(r->getSize()), 7)
                  << battBar(batt, 10)
                  << " " << std::setw(5) << std::fixed << std::setprecision(1) << batt << "%  "
                  << pcol(r->getLocation()->getName(), 14)
                  << statusBadge(busy, failed, atBase)
                  << "\n";
    }

    // ── Rooms ────────────────────────────────────────────────────
    section("BUILDING", BBLU);
    std::cout << DIM
              << "  " << pcol("Room",    14)
              << pcol("Size(m²)", 10)
              << pcol("Swept",   14)
              << pcol("Mopped",  14)
              << pcol("Scrubbed",14)
              << "Vacuumed\n" << RST;
    hline(72, BLUE);

    for (Room* r : building.getBuilding()) {
        if (r == base) continue;
        bool hp = priorityRooms.count(r->getName()) > 0;
        float sz = r->getSize();
        std::cout << "  "
                  << (hp ? BYEL : "") << BOLD << pcol(r->getName(), 14) << RST
                  << std::setw(8) << std::fixed << std::setprecision(1) << sz << "  "
                  << bar(r->getPercentSwept(),    10) << "  "
                  << bar(r->getPercentMopped(),   10) << "  "
                  << bar(r->getPercentScrubbed(), 10) << "  "
                  << bar(r->getPercentVacuumed(), 10)
                  << (hp ? (BYEL + " [!]" + RST) : "")
                  << "\n";
    }

    thickLine(72, BCYN);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Stats
// ─────────────────────────────────────────────────────────────────────────────
static void showStats(const Stats& s, Fleet& fleet, Building& building, Timer& timer) {
    using namespace UI;
    section("SESSION ANALYTICS", BGRN);

    auto& all    = fleet.getFleet();
    int avail    = (int)fleet.getAvailableRobots().size();
    int busy     = (int)fleet.getBusyRobots().size();
    int failed   = 0;
    for (Robot* r : all) if (r->hasFailed()) ++failed;

    int totalRooms = (int)building.getBuilding().size() - 1; // exclude base
    int clean  = (int)building.getCleanRooms().size();
    int dirty  = (int)building.getDirtyRooms().size();

    std::cout << "\n"
              << "  " << BOLD << "Time Elapsed   " << RST << BCYN << timer.getTime() << " units\n" << RST
              << "  " << BOLD << "Assignments    " << RST << s.assignments << " total"
                 << "  (" << s.autoRuns << " auto)\n"
              << "  " << BOLD << "Rooms Cleaned  " << RST << BGRN << s.completedCleans << RST << "\n"
              << "  " << BOLD << "Robot Failures " << RST << (s.failures ? BRED : BGRN)
                 << s.failures << RST << "\n"
              << "  " << BOLD << "Tech Calls     " << RST << s.techCalls << "\n"
              << "\n";

    section("FLEET SUMMARY", BMAG);
    std::cout << "  Total robots : " << BOLD << all.size()   << RST << "\n"
              << "  Available    : " << BGRN << avail  << RST << "\n"
              << "  Working      : " << BYEL << busy   << RST << "\n"
              << "  Failed       : " << (failed ? BRED : BGRN) << failed << RST << "\n\n";

    section("BUILDING SUMMARY", BBLU);
    std::cout << "  Total rooms  : " << BOLD << totalRooms << RST << "\n"
              << "  Clean        : " << BGRN << clean << RST << "\n"
              << "  Dirty        : " << (dirty ? BRED : BGRN) << dirty << RST << "\n\n";
}

// ─────────────────────────────────────────────────────────────────────────────
//  Smart auto-assign
//  Scores every (robot, room) pair and assigns the best available matches.
//  Scoring: battery_ratio * 0.5 + dirtiness * 0.5
// ─────────────────────────────────────────────────────────────────────────────
static int doAutoAssign(Fleet& fleet, Building& building, Manager& manager,
                        Room* base, Stats& stats,
                        const std::unordered_set<std::string>& priorityRooms) {
    using namespace UI;

    struct Match {
        Robot* robot;
        Room*  room;
        float  score;
    };

    std::vector<Match> matches;
    std::unordered_set<std::string> assignedRobots;
    std::unordered_set<std::string> assignedRooms;

    for (Robot* robot : fleet.getAvailableRobots()) {
        if (robot->hasFailed()) continue;

        for (Room* room : building.getDirtyRooms()) {
            bool compatible = false;
            float dirtiness = 0.0f;
            float roomSize  = room->getSize();
            int   capacity  = (robot->getSize() == Size::SMALL) ? 500 : 1200;

            if (robot->getJob() == Job::SWEEPER && room->getSweepable()) {
                compatible = true;
                dirtiness  = (100.0f - room->getPercentSwept()) / 100.0f;
                float need = ((100.0f - room->getPercentSwept()) * roomSize / (float)capacity) + 5.0f;
                if (robot->getBattery() < need * 0.5f) compatible = false;
            } else if (robot->getJob() == Job::MOPPER && room->getMoppable()) {
                compatible = true;
                dirtiness  = (100.0f - room->getPercentMopped()) / 100.0f;
                float need = ((100.0f - room->getPercentMopped()) * roomSize / (float)capacity) + 5.0f;
                if (robot->getBattery() < need * 0.5f) compatible = false;
            } else if (robot->getJob() == Job::SCRUBBER && room->getScrubbable()) {
                compatible = true;
                dirtiness  = (100.0f - room->getPercentScrubbed()) / 100.0f;
                float need = ((100.0f - room->getPercentScrubbed()) * roomSize / (float)capacity) + 5.0f;
                if (robot->getBattery() < need * 0.5f) compatible = false;
            } else if (robot->getJob() == Job::VACUUMER && room->getVacuumable()) {
                compatible = true;
                dirtiness  = (100.0f - room->getPercentVacuumed()) / 100.0f;
                float need = ((100.0f - room->getPercentVacuumed()) * roomSize / (float)capacity) + 5.0f;
                if (robot->getBattery() < need * 0.5f) compatible = false;
            }

            if (!compatible) continue;

            float battRatio = robot->getBattery() / 100.0f;
            float priority  = priorityRooms.count(room->getName()) ? 1.5f : 1.0f;
            float score     = (battRatio * 0.4f + dirtiness * 0.6f) * priority;
            matches.push_back({robot, room, score});
        }
    }

    // Sort best score first
    std::sort(matches.begin(), matches.end(),
              [](const Match& a, const Match& b){ return a.score > b.score; });

    int count = 0;
    for (auto& m : matches) {
        if (assignedRobots.count(m.robot->getName())) continue;
        if (assignedRooms.count(m.room->getName()))   continue;
        // Assign
        manager.assignRobot(m.robot, m.room);
        fleet.updateVectors(m.robot);
        assignedRobots.insert(m.robot->getName());
        assignedRooms.insert(m.room->getName());
        stats.assignments++;
        stats.autoRuns++;
        ++count;
        success("Auto-assigned " + m.robot->getName() + " [" + jobStr(m.robot->getJob()) + "]"
                + " --> " + m.room->getName()
                + "  (score " + std::to_string((int)(m.score * 100)) + ")");
    }
    if (count == 0) warn("No compatible robot/room pairs found for auto-assign.");
    return count;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Help menu
// ─────────────────────────────────────────────────────────────────────────────
static void showHelp() {
    using namespace UI;
    section("COMMANDS", BCYN);

    auto row = [](const std::string& cmd, const std::string& desc) {
        std::cout << "  " << BYEL << BOLD << pcol(cmd, 36) << RST
                  << DIM << desc << RST << "\n";
    };

    row("assign <robot> <room>",              "Assign a robot to clean a room");
    row("auto",                               "Smart-assign best robots to dirty rooms");
    row("display dirty rooms",                "List all dirty rooms");
    row("display clean rooms",                "List all clean rooms");
    row("display all rooms",                  "List every room");
    row("display busy robots",                "List robots currently working");
    row("display available robots",           "List robots ready for assignment");
    row("display all robots",                 "List the entire fleet");
    row("view <robot|room>",                  "Inspect a robot or room in detail");
    row("dashboard",                          "Full system overview (fleet + building)");
    row("stats",                              "Session analytics & performance metrics");
    row("call technician <robot>",            "Dispatch technician to repair a failed robot");
    row("priority <room>",                    "Toggle high-priority flag on a room");
    row("dirty <room>",                       "Mark a room as dirty (simulation)");
    row("time <N>",                           "Advance simulation by N time units");
    row("time until <robot>",                 "Wait until a robot finishes (or fails)");
    row("exit",                               "Quit the program");
    std::cout << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
//  main
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    using namespace UI;

    Timer     timer;
    Building  building{"logfile.csv"};
    Fleet     fleet{"logfile.csv"};
    Manager   manager{"logfile.csv"};
    Technician technician{"logfile.csv"};
    Stats     stats;
    std::unordered_set<std::string> priorityRooms;

    // Base room (charging station)
    Room baseObj{"Base", 0, 0, false, false, false, false, "logfile.csv"};
    Room* base = &baseObj;
    building.addRoom(base);

    // ── Load input.csv ──────────────────────────────────────────
    // Try several relative paths so it works from different build dirs
    std::ifstream infile;
    for (const auto& path : {"input.csv", "../app/input.csv", "../../app/input.csv",
                              "app/input.csv"}) {
        infile.open(path);
        if (infile.is_open()) break;
    }
    if (!infile.is_open()) {
        error("Could not open input.csv  (tried input.csv, app/input.csv, ../../app/input.csv)");
        return 1;
    }

    const std::string WS = " \n\r\t\f\v";
    std::string line;
    bool makeRooms = false;

    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        auto terms = split(line, WS + ",()");
        if (terms.empty()) continue;

        if (terms[0] == "robots:") {
            makeRooms = false;
        } else if (terms[0] == "rooms:") {
            makeRooms = true;
        } else if (!makeRooms && terms.size() >= 3) {
            // Parse robot: name size type
            Size sz  = (terms[1] == "large") ? Size::LARGE : Size::SMALL;
            Job  job = Job::SWEEPER;
            if      (terms[2] == "mopper")   job = Job::MOPPER;
            else if (terms[2] == "scrubber")  job = Job::SCRUBBER;
            else if (terms[2] == "vacuumer")  job = Job::VACUUMER;

            Robot* r = new Robot(terms[0], 100, sz, base, "logfile.csv", job);
            fleet.addToFleet(r);
            fleet.updateVectors(r);
        } else if (makeRooms && terms.size() >= 6) {
            // Parse room: name length width sweepable moppable scrubbable [vacuumable]
            bool vac = (terms.size() >= 7) && (terms[6] == "true");
            Room* r = new Room(
                terms[0],
                std::stof(terms[1]), std::stof(terms[2]),
                terms[3] == "true", terms[4] == "true",
                terms[5] == "true", vac,
                "logfile.csv"
            );
            building.addRoom(r);
        }
    }

    // ── Welcome ─────────────────────────────────────────────────
    banner();
    info("Loaded " + std::to_string(fleet.getFleet().size()) + " robots and "
         + std::to_string((int)building.getBuilding().size() - 1) + " rooms.");

    // ── Simulation state ────────────────────────────────────────
    int    wait          = 0;
    bool   waitingOnRobot = false;
    Robot* waitingRobot   = nullptr;   // null = not waiting

    // ── Main loop ───────────────────────────────────────────────
    while (true) {
        // Resolve time-advance modes
        bool interactive = (wait == 0 && !waitingOnRobot);

        if (interactive) {
            prompt(timer.getTime());
            std::flush(std::cout);
        }

        // ── Command input ────────────────────────────────────────
        while (interactive) {
            std::string first, second, third;
            std::cin >> first;

            if (first == "help") {
                showHelp();

            } else if (first == "dashboard") {
                showDashboard(fleet, building, timer, base, priorityRooms);

            } else if (first == "stats") {
                showStats(stats, fleet, building, timer);

            } else if (first == "auto") {
                section("SMART AUTO-ASSIGN", BGRN);
                int n = doAutoAssign(fleet, building, manager, base, stats, priorityRooms);
                if (n > 0) {
                    info(std::to_string(n) + " assignment(s) made.");
                }

            } else if (first == "priority") {
                std::cin >> second;
                bool found = false;
                for (Room* r : building.getBuilding()) {
                    if (r->getName() == second && r != base) {
                        found = true;
                        if (priorityRooms.count(second)) {
                            priorityRooms.erase(second);
                            warn(second + " priority flag removed.");
                        } else {
                            priorityRooms.insert(second);
                            success(second + " marked as HIGH PRIORITY.");
                        }
                    }
                }
                if (!found) error("Room '" + second + "' not found.");

            } else if (first == "time") {
                std::cin >> second;
                bool isNum = !second.empty() && std::all_of(second.begin(), second.end(), ::isdigit);
                if (isNum) {
                    wait = std::stoi(second);
                    interactive = false;
                } else if (second == "until") {
                    std::cin >> third;
                    bool found = false;
                    for (Robot* r : fleet.getFleet()) {
                        if (r->getName() == third) {
                            waitingRobot  = r;
                            waitingOnRobot = true;
                            interactive   = false;
                            found         = true;
                            info("Waiting for " + third + " to finish...");
                        }
                    }
                    if (!found) error("Robot '" + third + "' not found.");
                } else {
                    error("Usage: time <N>  or  time until <robot>");
                }

            } else if (first == "assign") {
                std::cin >> second >> third;
                bool foundRobot = false, foundRoom = false;
                for (Robot* r : fleet.getFleet()) {
                    if (r->getName() == second) {
                        foundRobot = true;
                        for (Room* room : building.getBuilding()) {
                            if (room->getName() == third) {
                                foundRoom = true;
                                manager.assignRobot(r, room);
                                fleet.updateVectors(r);
                                stats.assignments++;
                            }
                        }
                    }
                }
                if (!foundRobot || !foundRoom)
                    error("Robot '" + second + "' or room '" + third + "' not found.");

            } else if (first == "display") {
                std::cin >> second >> third;
                if      (second == "dirty"     && third == "rooms")  manager.displayDirtyRooms(building);
                else if (second == "clean"     && third == "rooms")  manager.displayCleanRooms(building);
                else if (second == "all"       && third == "rooms")  manager.displayAllRooms(building);
                else if (second == "busy"      && third == "robots") manager.displayBusyRobots(fleet);
                else if (second == "available" && third == "robots") manager.displayAvailableRobots(fleet);
                else if (second == "all"       && third == "robots") manager.displayFleet(fleet);
                else error("Unknown display target. Type 'help' for options.");

            } else if (first == "view") {
                std::cin >> second;
                bool found = false;
                for (Robot* r : fleet.getFleet()) {
                    if (r->getName() == second) { found = true; manager.viewRobotStatus(r); }
                }
                for (Room* r : building.getBuilding()) {
                    if (r->getName() == second) { found = true; manager.viewLocation(r); }
                }
                if (!found) error("'" + second + "' not found.");

            } else if (first == "call") {
                std::cin >> second >> third;
                if (second != "technician") {
                    error("Usage: call technician <robot>");
                } else {
                    bool found = false;
                    for (Robot* r : fleet.getFleet()) {
                        if (r->getName() == third) {
                            found = true;
                            manager.callTech(r, technician);
                            r->setLocation(base);
                            stats.techCalls++;
                            success("Technician dispatched for " + third + ".");
                        }
                    }
                    if (!found) error("Robot '" + third + "' not found.");
                }

            } else if (first == "dirty") {
                std::cin >> second;
                bool found = false;
                for (Room* r : building.getBuilding()) {
                    if (r->getName() == second && r != base) {
                        found = true;
                        r->randomlyDirty();
                        warn(second + " has been dirtied.");
                    }
                }
                if (!found) error("Room '" + second + "' not found.");

            } else if (first == "exit") {
                section("FINAL STATS", BCYN);
                showStats(stats, fleet, building, timer);
                std::cout << BCYN << BOLD << "  Goodbye!\n" << RST;
                return 0;

            } else {
                error("Unknown command '" + first + "'. Type 'help' for the command list.");
            }

            // Check if we should keep asking for commands this tick
            if (wait > 0 || waitingOnRobot) break;

            prompt(timer.getTime());
            std::flush(std::cout);
        }

        // ── Tick: update all robots ──────────────────────────────
        int prevFailed = 0;
        for (Robot* r : fleet.getFleet()) if (r->hasFailed()) ++prevFailed;

        for (Robot* r : fleet.getFleet()) {
            if (r->getLocation() == base) {
                r->charge();
            } else {
                r->setFailed(r->hasFailed());
                r->clean();
                if (r->isRoomClean()) {
                    std::cout << BGRN << "  [+] " << RST << GREEN
                              << r->getName() << " finished cleaning "
                              << r->getLocation()->getName() << "!\n" << RST;
                    r->move(base);
                    r->setBusy(false);
                    fleet.updateVectors(r);
                    stats.completedCleans++;
                }
            }
        }

        // Count new failures this tick
        for (Robot* r : fleet.getFleet()) {
            if (r->hasFailed()) ++stats.failures;
        }
        stats.failures = std::max(0, stats.failures - prevFailed);

        technician.technicianFixesRobot();
        timer.addTime();

        if (wait > 0) --wait;

        // Resolve waitingOnRobot
        if (waitingOnRobot && waitingRobot != nullptr) {
            if (waitingRobot->hasFailed()) {
                warn(waitingRobot->getName() + " has FAILED while working.");
                waitingOnRobot = false;
                waitingRobot   = nullptr;
            } else if (waitingRobot->getLocation() == base) {
                waitingOnRobot = false;
                waitingRobot   = nullptr;
            }
        }
    }
}
