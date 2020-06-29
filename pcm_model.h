#include <vector>

struct PcmModel {
    uint16_t pcm;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t bytes_per_sec;
    uint16_t block_align;
    uint16_t bits_per_sample;
    std::vector<std::vector<int32_t>> channel_data;
};