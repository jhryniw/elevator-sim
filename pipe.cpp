#include "pipe.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <filesystem>
#include <unistd.h>

namespace elevator_sim {

Pipe::Pipe(int src, int dst)
{
    r_fifo_ = createFifo(src, dst, 'r');
    w_fifo_ = createFifo(src, dst, 'w');
}

Pipe::~Pipe() {
    close(rfd());
    close(wfd());
}

int Pipe::rfd() const {
    return r_fifo_.fd;
}

int Pipe::wfd() const {
    return w_fifo_.fd;
}

pollfd Pipe::pollfd() const {
    return { .fd = rfd() };
}

int Pipe::dst() const {
    return r_fifo_.dst;
}

int Pipe::src() const {
    return r_fifo_.src;
}

std::string Pipe::read() {
    char msg_buf[128];
    ::read(rfd(), msg_buf, 128);
    return std::string(msg_buf);
}

void Pipe::write(const std::string& payload) {
    // Try opening write connection if it isn't open yet
    if (wfd() == -1) {
        w_fifo_ = createFifo(w_fifo_.src, w_fifo_.dst, 'w');
    }

    if (wfd() != -1) {
        ::write(wfd(), payload.c_str(), payload.size());
    }
}

Pipe::fifo_t Pipe::createFifo(int src, int dst, char rw) {
    fifo_t fifo;
    char fifoPath[64];

    fifo.src = src;
    fifo.dst = dst;

    if (src == -1 || dst == -1) {
        return fifo;
    } 

    if (!std::filesystem::exists("/tmp/elevator-sim")) {
        std::filesystem::create_directories("/tmp/elevator-sim");
    }

    if (rw == 'r') {
        sprintf(fifoPath, "/tmp/elevator-sim/fifo-%d-%d", dst, src);

        if ((fifo.fd = open(fifoPath, O_RDONLY | O_NONBLOCK)) == -1) {
            mkfifo(fifoPath, S_IRUSR | S_IWUSR);
            fifo.fd = open(fifoPath, O_RDONLY | O_NONBLOCK);
        }
    } else if (rw == 'w') {
        sprintf(fifoPath, "/tmp/elevator-sim/fifo-%d-%d", src, dst);

        if ((fifo.fd = open(fifoPath, O_WRONLY | O_NONBLOCK)) == -1) {
            mkfifo(fifoPath, S_IRUSR | S_IWUSR);
            fifo.fd = open(fifoPath, O_WRONLY | O_NONBLOCK);
        }
    } else {
        // Error
        printf("Cannot create fifo with rw flag \'%c\'", rw);
        return fifo;
    }

    return fifo;
}

std::optional<std::string> Poll(Pipe* pipe) {
    pollfd pfd = pipe->pollfd();
    if (poll(&pfd, /*nfds=*/1, /*timeout=*/0)) {
        if (pfd.revents & POLLIN) {
            return pipe->read();
        }
    }

    return std::nullopt;
}

}  // namespace elevator_sim
