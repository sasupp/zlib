#ifndef ZIO_H
#define ZIO_H

namespace ZLib {
namespace IO {

#ifdef _WIN32
#define getc _getchar_nolock
#else
#define getc getchar_unlocked
#endif

class IO {
public:
static int ReadInt() {
    char c = getc();
    while (c < '0' || c > '9') c = getc();

    int ret = 0;
    while (c >= '0' && c <= '9') {
        ret = 10 * ret + c - 48;
        c = getc();
    }
    return ret;
}

static int ReadChar() {
    char c = getc();
    while (c == '\r' || c == '\n') c = getc();
    return (int)c;
}
};

}
}

#endif // ZIO_H
