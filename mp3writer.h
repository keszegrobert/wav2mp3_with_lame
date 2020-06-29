#include "wavreader.h"
#include <string>

const size_t MP3_SIZE = 8192;

class Mp3Writer {
public:
    void write(std::string fname, PcmModel& pcm);
};