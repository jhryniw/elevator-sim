#include "packet.h"
#include "pipe.h"
#include "sim/sim.h"

#include <chrono>
#include <iostream>
#include <optional>
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

namespace elevator_sim {
namespace {

State MakeBuilding(int numFloors, int numElevators) {
    std::vector<Floor> floors(numFloors);
    for (size_t i = 0; i < numFloors; i++) {
        floors[i].number = i;
    }

    ElevatorSpec spec = {
        .maxSpeed = 300,
        .maxFloor = numFloors - 1,
    };

    std::vector<Elevator> elevators;
    elevators.reserve(numElevators);
    for (size_t i = 0; i < numElevators; i++) {
        elevators.emplace_back(Elevator(spec));
    }

    return {
        .elevators = std::move(elevators),
        .floors = std::move(floors),
    };
}

void MoveElevator(const MoveElevatorCommand& cmd, State* state) {
    if (cmd.elevatorId >= 0 && cmd.elevatorId < state->elevators.size()) {
        state->elevators[cmd.elevatorId].setTargetFloor(cmd.targetFloor);
    }
}

void AcceptPassenger(const PassengerCommand& cmd, State* state) {
    if (cmd.sourceFloor >= 0 && cmd.sourceFloor <= state->floors.size() &&
        cmd.destinationFloor >= 0 && cmd.destinationFloor <= state->floors.size() &&
        cmd.sourceFloor != cmd.destinationFloor) {
        Passenger p = {
            .startTime = state->currentTime,
            .destination = cmd.destinationFloor,
        };
        state->floors[cmd.sourceFloor].passengers.push_back(p);
    }
};

// helper type for the visitor
template<class... Ts> struct Visitor : Ts... { using Ts::operator()...; };
template<class... Ts> Visitor(Ts...) -> Visitor<Ts...>;

void ProcessPacket(const Packet& packet, State* state) {
    std::visit(Visitor{
        [state](const MoveElevatorCommand& cmd) { MoveElevator(cmd, state); },
        [state](const PassengerCommand& cmd) { AcceptPassenger(cmd, state); },
    }, packet);
}

void ProcessIncoming(Pipe* pipe, State* state) {
    while (true) {
        auto maybeRequest = Poll(pipe);
        if (maybeRequest) {
            std::cout << "request recieved" << std::endl;
            auto maybePacket = DecodePacket(*maybeRequest);
            if (maybePacket) {
                ProcessPacket(*maybePacket, state);
            }
        } else {
            break;
        }
    }
}

}  // namespace
}  // namespace elevator_sim

int main() {
    using namespace elevator_sim;
    Pipe requestPipe(/*src*/0, /*dst*/1);
    State state = MakeBuilding(
        /*numFloors=*/9,
        /*numElevators=*/2
    );

    int t = 0;
    while (true) {
        state.currentTime = t;
        ProcessIncoming(&requestPipe, &state);
        Tick(&state);
        Print(state);
        t++;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}
