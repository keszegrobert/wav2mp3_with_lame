#include <fstream>

class WavFile {
private:
    uint16_t pcm;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t bytes_per_sec;
    uint16_t block_align;
    uint16_t bits_per_sample;

private:
    void parseHeader(std::ifstream& infile, std::string& name, uint32_t& size);
    void parseRiff(std::ifstream& infile, uint32_t& size);
    void parseFmt(std::ifstream& infile);
    void parseData(std::ifstream& infile);
    void ignoreUnknown(std::ifstream& infile, uint32_t size);

public:
    void parse(std::string fname);
};
