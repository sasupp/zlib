#ifndef ZTIMER_H
#define ZTIMER_H

#include <inttypes.h>
#include <chrono>

namespace ZLib {

namespace TimerLib {

class Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
public:
    Timer() {}

    inline void Start() {
        start = std::chrono::high_resolution_clock::now();
    }

    inline int64_t Elapsed() const {
        return std::chrono::duration_cast<std::chrono::microseconds>
                (std::chrono::high_resolution_clock::now() - start).count();
    }
};

}
}
#endif
