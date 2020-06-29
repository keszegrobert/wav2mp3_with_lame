#include "mp3writer.h"
#include "wavreader.h"
#include <iostream>
#include <lame/lame.h>

void Mp3Writer::write(std::string fname, WavReader& wav)
{
    lame_t lame = lame_init();
    lame_set_in_samplerate(lame, wav.sample_rate);
    lame_set_VBR(lame, vbr_default);
    lame_set_VBR_q(lame, 5);

    if (lame_init_params(lame) < 0) {
        return;
    }

    std::ofstream mp3;
    mp3.open(fname, std::ios_base::binary);
    while (true) {
        int write = 0;
        if (wav.channels == 1)
            write = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
        else
            write = lame_encode_buffer_interleaved(lame, pcm_buffer, wav.channel_data.size(), mp3_buffer, MP3_SIZE);
        mp3.write(reinterpret_cast<char*>(mp3_buffer), write);
    }

    mp3.close();

    lame_close(lame);
}