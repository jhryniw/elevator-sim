#pragma once

#include <sstream>
#include <cmath>
#include <iostream>
#include <vector>

namespace elevator_sim {

struct TickContext {
    int time;
    int deltaTime;
};

struct Passenger {
    int startTime;
    int destination;
};

struct ElevatorSpec {
    int maxSpeed = 300; // millifloors / sec
    int maxFloor = 0;
};

// struct StateCallbacks {
//     std::function<void onFloorReached(int floor, Elevator* elevator)
// }

class Elevator {
public:

    explicit Elevator(const ElevatorSpec& spec) : _spec(spec) {}
    // void board(Passenger newPassenger);

    void setTargetFloor(int targetFloor) {
        if (targetFloor >= 0 && targetFloor <= _spec.maxFloor) {
            _targetMilliFloor = targetFloor * 1000;
        }
    }

    bool atFloor(int floor) const {
        return _milliFloor == floor * 1000;
    }

    bool atTargetFloor() {
        return _milliFloor == _targetMilliFloor;
    }

    int getApproximateFloor() const {
        return std::round(_milliFloor / 1000);
    }

    int numPassengers() const {
        return _passengers.size();
    }

    void tick() {
        if (_milliFloor < _targetMilliFloor) {
            _milliFloor = std::min(
                _milliFloor + _spec.maxSpeed,
                _targetMilliFloor);
            _milliFloor = std::min(
                _milliFloor,
                _spec.maxFloor * 1000
            );
        } else if (_milliFloor > _targetMilliFloor) {
            _milliFloor = std::max(
                _milliFloor - _spec.maxSpeed,
                _targetMilliFloor);
            _milliFloor = std::max(_milliFloor, 0);
        }
    }

private:
    const ElevatorSpec _spec;
    int _targetMilliFloor = 1000;

    int _milliFloor = 1000;
    std::vector<Passenger> _passengers;
};

struct Floor {
    int number = 0;
    bool upButtonPressed = false;
    bool downButtonPressed = false;
    std::vector<Passenger> passengers;
};

struct State {
    bool ok = true;
    int currentTime = 0;
    std::vector<Elevator> elevators;
    std::vector<Floor> floors;
};

void Tick(State* state);
void Print(const State& state);

}  // namespace elevator_sim
