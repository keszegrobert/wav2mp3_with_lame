#include "wavfile.h"
#include <iostream>

void WavFile::parseHeader(std::ifstream& infile, std::string& name, uint32_t& size)
{
    char chunk_name[4];
    infile.read(chunk_name, 4);
    if (infile.tellg() < 0)
        return;
    name = std::string(chunk_name, 4);
    infile.read(reinterpret_cast<char*>(&size), 4);
    std::cout << "chunk size of " << name << " is " << size << std::endl;
}

void WavFile::parseRiff(std::ifstream& infile, uint32_t& size)
{
    char wave[4];
    infile.read(wave, 4);
    if (std::string(wave) == "WAVE") {
        throw std::runtime_error("No WAVE in the header");
    }
}

void WavFile::parseFmt(std::ifstream& infile)
{
    infile.read(reinterpret_cast<char*>(&pcm), 2);
    std::cout << "pcm: " << pcm << std::endl;
    infile.read(reinterpret_cast<char*>(&channels), 2);
    std::cout << "channels: " << channels << std::endl;
    infile.read(reinterpret_cast<char*>(&sample_rate), 4);
    std::cout << "sample_rate: " << sample_rate << std::endl;
    infile.read(reinterpret_cast<char*>(&bytes_per_sec), 4);
    std::cout << "bytes_per_sec: " << bytes_per_sec << std::endl;
    infile.read(reinterpret_cast<char*>(&block_align), 2);
    std::cout << "block_align: " << block_align << std::endl;
    infile.read(reinterpret_cast<char*>(&bits_per_sample), 2);
    std::cout << "bits_per_sample: " << bits_per_sample << std::endl
              << std::endl;
}

void WavFile::ignoreUnknown(std::ifstream& infile, uint32_t size)
{
    if (infile.tellg() < 0)
        return;
    std::cout << "    ignoring: " << size << " bytes" << std::endl;
    infile.ignore(size);
}

void WavFile::parseData(std::ifstream& infile) {}

void WavFile::parse(std::string fname)
{
    std::ifstream infile(fname, std::ios::binary);
    if (!infile.is_open()) {
        throw std::runtime_error("Could not open file: " + fname);
    }

    std::string chunk_name;

    while (infile.tellg() >= 0) {
        uint32_t size;
        parseHeader(infile, chunk_name, size);
        if (chunk_name == "RIFF") {
            parseRiff(infile, size);
            continue;
        }

        if (chunk_name == "fmt ") {
            parseFmt(infile);
            continue;
        }

        if (chunk_name == "data") {
            parseData(infile);
            continue;
        }

        ignoreUnknown(infile, size);
    }
}
