#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <stdexcept>
#include "libclean/manager.hpp"
#include "libclean/fleet.hpp"
#include "libclean/robot.hpp" 
#include "libclean/room.hpp"
#include "libclean/building.hpp"
#include "libclean/timer.hpp"

TEST_CASE("Timer Tests"){
    Timer timer;

    SECTION("Time") {
    // addTime() and getTime()
        CHECK( timer.getTime() == 0 );
        timer.addTime();
        CHECK( timer.getTime() == 1 );
        timer.addTime();
        CHECK( timer.getTime() == 2 );
    }
}