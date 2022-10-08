#include "sim/Simulator.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

// Simulation
// - time based simulation (1s decision points)
// - 

// Metrics
// - avg wait time
// - avg stops per trip
// - avg trip time?
// - throughput (passengers / hr)
// - resilience (to downed elevators)
// - distance travelled (floors)

// constexpr int SECONDS_PER_DAY = 60 * 60 * 24;

State MakeBuilding(int numFloors, int numElevators) {
    std::vector<Floor> floors(numFloors);
    for (size_t i = 0; i < numFloors; i++) {
        floors[i].number = i;
    }

    std::vector<Elevator> elevators(numElevators);
    return {
        .elevators = std::move(elevators),
        .floors = std::move(floors),
    };
}



int main() {
    State state = MakeBuilding(
        /*numFloors=*/9,
        /*numElevators=*/2
    );

    for (int t = 0; t < 300; t++) {
        Tick(&state);
        Print(state);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}
