#pragma once
#include "pcm_model.h"
#include <fstream>
#include <vector>

class WavReader {

private:
    void parseHeader(std::ifstream& infile, std::string& name, uint32_t& size);
    void parseRiff(std::ifstream& infile, uint32_t& size);
    void parseFmt(std::ifstream& infile, PcmModel& pcm);
    void parseData(std::ifstream& infile, uint32_t size, PcmModel& pcm);
    void ignoreUnknown(std::ifstream& infile, uint32_t size);

public:
    void load(std::string fname, PcmModel& model);
};
