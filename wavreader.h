#pragma once
#include <fstream>
#include <vector>

class WavReader {
public:
    uint16_t pcm;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t bytes_per_sec;
    uint16_t block_align;
    uint16_t bits_per_sample;
    std::vector<std::vector<int32_t>> channel_data;

private:
    void parseHeader(std::ifstream& infile, std::string& name, uint32_t& size);
    void parseRiff(std::ifstream& infile, uint32_t& size);
    void parseFmt(std::ifstream& infile);
    void parseData(std::ifstream& infile, uint32_t size);
    void ignoreUnknown(std::ifstream& infile, uint32_t size);

public:
    void load(std::string fname);
};
