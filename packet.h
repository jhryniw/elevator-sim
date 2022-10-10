#pragma once

#include <optional>
#include <string>
#include <variant>

enum PacketType : int {
    MOVE_ELEVATOR_COMMAND = 0,
    PASSENGER_COMMAND = 1,
};

struct MoveElevatorCommand {
    int elevatorId;
    int targetFloor;
};

struct PassengerCommand {
    int sourceFloor;
    int destinationFloor;
};

std::string EncodePacket(const MoveElevatorCommand& cmd);
std::string EncodePacket(const PassengerCommand& cmd);

using Packet = std::variant<MoveElevatorCommand, PassengerCommand>;

std::optional<Packet> DecodePacket(const std::string& msg);
