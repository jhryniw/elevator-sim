#pragma once

#include <poll.h>

#include <optional>
#include <string>

namespace elevator_sim {

/**
 * Creates bidirectional communication between two nodes
 * src: The node id creating the fifo
 * dst: The node id to connect to
 */
class Pipe {
public:

    Pipe() = default;
    Pipe(int src, int dst);
    ~Pipe();

    // the source node id
    int src() const;

    // the destination node id
    int dst() const;

    pollfd pollfd() const;

    /**
     * Read a packet incoming from the destination
     */
    std::string read();
    void write(const std::string& payload);

private:
    // the read file descriptor
    int rfd() const;

    // the write file descriptor
    int wfd() const;

    struct fifo_t {
        int src;
        int dst;
        int fd = -1;
    };

    fifo_t r_fifo_;
    fifo_t w_fifo_;

    /**
     * Creates one FIFO for reading or writing
     * The src and dst should not be different between calls
     * @return a FIFO struct containing an opened file descriptor
     */
    fifo_t createFifo(int src, int dst, char rw);
};

std::optional<std::string> Poll(Pipe* pipe);

}  // namespace elevator_sim
