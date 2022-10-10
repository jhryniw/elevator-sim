#include "packet.h"
#include "pipe.h"

#include <iostream>
#include <sstream>

MoveElevatorCommand ParseMoveElevatorCommand(std::istream& iss) {
    MoveElevatorCommand cmd;
    iss >> cmd.elevatorId;
    iss >> cmd.targetFloor;
    return cmd;
} 

int main() {
    elevator_sim::Pipe commandPipe(/*src=*/1, /*dst=*/0);

    std::string input;
    std::cout << "> ";
    while (std::getline(std::cin, input)) {
        std::string firstToken;
        std::istringstream iss(input);
        iss >> firstToken;
        if (firstToken.empty()) {
            std::cout << "> ";
            continue;
        } else if (firstToken == "exit") {
            break;
        } else if (firstToken == "e" || firstToken == "elevator") {
            auto cmd = ParseMoveElevatorCommand(iss);
            commandPipe.write(EncodePacket(cmd));
        } else {
            std::cout << "unrecogonized command" << std::endl;
        }

        std::cout << "> ";
    }

    return 0;
}
