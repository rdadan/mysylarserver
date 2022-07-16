#include "util.h"

namespace rdadan {
pid_t GetThreadID() {
    return syscall(SYS_gettid);
}

uint32_t GetFiberID() {
    return 666;
}

} // namespace rdadan
