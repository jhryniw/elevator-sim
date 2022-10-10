#pragma once

#include <optional>
#include <string>
#include <variant>

enum PacketType : int {
    MOVE_ELEVATOR_COMMAND = 0,
};

struct MoveElevatorCommand {
    int elevatorId;
    int targetFloor;
};

using Packet = std::variant<MoveElevatorCommand>;

std::optional<Packet> DecodePacket(const std::string& msg);
