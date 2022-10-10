#include "sim.h"

#include <iostream>

namespace elevator_sim {

void Tick(State* state) {
    for (auto& elevator : state->elevators) {
        elevator.tick();
    }
}

void Print(const State& state) {
    static bool firstPrint = true;

    if (state.floors.size() == 0) {
        return;
    }

    std::stringstream ss;
    for (int f = state.floors.size() - 1; f >= 0; f--) {
        const auto& floor = state.floors[f];
        ss << floor.number << " -- " << floor.passengers.size() << " |";
        for (const auto& elevator : state.elevators) {
            if (elevator.getApproximateFloor() == floor.number) {
                ss << elevator.numPassengers();
            } else {
                ss << ' ';
            }
            ss << '|';
        }
        ss << '\n';
    }

    if (!firstPrint) {
        std::cout << "\033[" << state.floors.size() << 'A' << '\r';
    }
    firstPrint = false;
    std::cout << ss.str();
    std::cout.flush();
    // std::cout << ss.str() << std::endl;
}

}  // namespace elevator_sim
