#include "wavreader.h"
#include <string>

const size_t MP3_SIZE = 8192;
const size_t PCM_SIZE = 8192;

class Mp3Writer {
private:
    unsigned char mp3_buffer[MP3_SIZE];
    int16_t pcm_buffer[PCM_SIZE * 2];

public:
    void write(std::string fname, PcmModel& pcm);
};