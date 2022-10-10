#include "packet.h"

#include <sstream>

std::string EncodePacket(const MoveElevatorCommand& cmd) {
    std::ostringstream ss;
    ss << PacketType::MOVE_ELEVATOR_COMMAND << ' ';
    ss << cmd.elevatorId << ' ';
    ss << cmd.targetFloor;
    return ss.str();
}

std::optional<Packet> DecodePacket(const std::string& msg) {
    std::istringstream iss(msg);

    int type;
    iss >> type;

    switch (type) {
        case PacketType::MOVE_ELEVATOR_COMMAND:
            MoveElevatorCommand cmd;
            iss >> cmd.elevatorId;
            iss >> cmd.targetFloor;
            return cmd;
    }

    return std::nullopt;
}
