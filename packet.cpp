#include "packet.h"

#include <sstream>

std::string EncodePacket(const MoveElevatorCommand& cmd) {
    std::ostringstream ss;
    ss << PacketType::MOVE_ELEVATOR_COMMAND << ' ';
    ss << cmd.elevatorId << ' ';
    ss << cmd.targetFloor;
    return ss.str();
}

std::string EncodePacket(const PassengerCommand& cmd) {
    std::ostringstream ss;
    ss << PacketType::PASSENGER_COMMAND << ' ';
    ss << cmd.sourceFloor << ' ';
    ss << cmd.destinationFloor;
    return ss.str();
}

std::optional<Packet> DecodePacket(const std::string& msg) {
    std::istringstream iss(msg);

    int type;
    iss >> type;

    switch (type) {
        case PacketType::MOVE_ELEVATOR_COMMAND:
            MoveElevatorCommand ecmd;
            iss >> ecmd.elevatorId;
            iss >> ecmd.targetFloor;
            return ecmd;
        case PacketType::PASSENGER_COMMAND:
            PassengerCommand pcmd;
            iss >> pcmd.sourceFloor;
            iss >> pcmd.destinationFloor;
            return pcmd;
    }

    return std::nullopt;
}
