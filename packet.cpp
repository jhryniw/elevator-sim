#include "packet.h"

#include <sstream>

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
