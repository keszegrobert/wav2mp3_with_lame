#include "wavreader.h"
#include <iostream>

void WavReader::parseHeader(std::ifstream& infile, std::string& name, uint32_t& size)
{
    char chunk_name[4];
    infile.read(chunk_name, 4);
    if (infile.tellg() < 0)
        return;
    name = std::string(chunk_name, 4);
    infile.read(reinterpret_cast<char*>(&size), 4);
    std::cout << "chunk size of " << name << " is " << size << std::endl;
}

void WavReader::parseRiff(std::ifstream& infile, uint32_t& size)
{
    char wave[4];
    infile.read(wave, 4);
    if (std::string(wave) == "WAVE") {
        throw std::runtime_error("No WAVE in the header");
    }
}

void WavReader::parseFmt(std::ifstream& infile)
{
    infile.read(reinterpret_cast<char*>(&pcm), 2);
    std::cout << "pcm: " << pcm << std::endl;
    if (pcm != 1)
        throw std::runtime_error("File not in PCM format");

    infile.read(reinterpret_cast<char*>(&channels), 2);
    std::cout << "channels: " << channels << std::endl;
    if (channels != 1 && channels != 2)
        throw std::runtime_error("Number of channels is not normal");

    infile.read(reinterpret_cast<char*>(&sample_rate), 4);
    std::cout << "sample_rate: " << sample_rate << std::endl;

    infile.read(reinterpret_cast<char*>(&bytes_per_sec), 4);
    std::cout << "bytes_per_sec: " << bytes_per_sec << std::endl;

    infile.read(reinterpret_cast<char*>(&block_align), 2);
    std::cout << "block_align: " << block_align << std::endl;

    infile.read(reinterpret_cast<char*>(&bits_per_sample), 2);
    std::cout << "bits_per_sample: " << bits_per_sample << std::endl;
    if (bits_per_sample != 8 && bits_per_sample != 16 && bits_per_sample != 24)
        throw std::runtime_error("Number of bits per sample is not normal");

    int32_t num_bytes_per_second = (int32_t)((channels * sample_rate * bits_per_sample) / 8);
    std::cout << "num_bytes_per_second: " << num_bytes_per_second << std::endl;

    int16_t num_bytes_per_block = channels * (bits_per_sample / 8);
    std::cout << "num_bytes_per_block: " << num_bytes_per_block << std::endl;

    int num_bytes_per_sample = bits_per_sample / 8;
    std::cout << "num_bytes_per_sample: " << num_bytes_per_sample << std::endl;

    if (num_bytes_per_second != (channels * sample_rate * bits_per_sample) / 8)
        throw std::runtime_error("Header is corrupted");
    if (num_bytes_per_block != (channels * num_bytes_per_sample))
        throw std::runtime_error("Header is corrupted");
}

void WavReader::ignoreUnknown(std::ifstream& infile, uint32_t size)
{
    if (infile.tellg() < 0)
        return;
    std::cout << "    ignoring: " << size << " bytes" << std::endl;
    infile.ignore(size);
}

void WavReader::parseData(std::ifstream& infile, uint32_t size)
{
    int16_t num_bytes_per_block = channels * (bits_per_sample / 8);
    int samples = size / (channels * num_bytes_per_block);
    channel_data.clear();
    channel_data.resize(channels);
    for (int channel = 0; channel < channels; ++channel) {
        channel_data[channel].resize(samples);
    }
    std::cout << "samples: " << samples << std::endl;
    std::cout << "channels: " << channels << std::endl;
    for (int sample = 0; sample < samples; ++sample) {
        for (int channel = 0; channel < channels; ++channel) {
            int32_t data = 0;
            infile.read(reinterpret_cast<char*>(&data), bits_per_sample / 8);
            channel_data[channel][sample] = data;
        }
    }

    for (size_t i = 0; i < channels; ++i) {
        std::cout << "channel[" << i << "].size = " << channel_data[i].size() << std::endl;
    }
}

void WavReader::load(std::string fname)
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
            parseData(infile, size);
            continue;
        }

        ignoreUnknown(infile, size);
    }
}
