#include "mp3writer.h"
#include "wavreader.h"
#include <iostream>
#include <lame/lame.h>

void Mp3Writer::write(std::string fname, PcmModel& pcm)
{
    lame_t lame = lame_init();
    int num_samples = pcm.channel_data[0].size();
    lame_set_num_samples(lame, num_samples);
    lame_set_in_samplerate(lame, pcm.sample_rate);
    lame_set_num_channels(lame, pcm.channels);

    //lame_set_VBR(lame, vbr_default);
    //lame_set_VBR_q(lame, 5);

    if (lame_init_params(lame) < 0) {
        return;
    }

    std::ofstream mp3;
    mp3.open(fname, std::ios_base::binary);
    int start = 0;
    while (true) {
        int written = 0;
        unsigned char mp3_buffer[MP3_SIZE];
        int bufsize = std::min(8192, num_samples - start);
        written = lame_encode_buffer_ieee_double(lame,
            &pcm.channel_data[0][start],
            &pcm.channel_data[1][start],
            bufsize,
            mp3_buffer,
            MP3_SIZE);
        mp3.write(reinterpret_cast<char*>(mp3_buffer), written);
        start += bufsize;
        std::cout << "Processed " << start << " bytes, written: " << written << std::endl;
        if (written == 0 || num_samples <= start)
            break;
    }

    mp3.close();

    lame_close(lame);
}