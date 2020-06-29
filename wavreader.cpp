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

void WavReader::parseFmt(std::ifstream& infile, PcmModel& pcm)
{
    infile.read(reinterpret_cast<char*>(&pcm.pcm), 2);
    std::cout << "pcm: " << pcm.pcm << std::endl;
    if (pcm.pcm != 1)
        throw std::runtime_error("File not in PCM format");

    infile.read(reinterpret_cast<char*>(&pcm.channels), 2);
    std::cout << "channels: " << pcm.channels << std::endl;
    if (pcm.channels != 1 && pcm.channels != 2)
        throw std::runtime_error("Number of channels is not normal");

    infile.read(reinterpret_cast<char*>(&pcm.sample_rate), 4);
    std::cout << "sample_rate: " << pcm.sample_rate << std::endl;

    infile.read(reinterpret_cast<char*>(&pcm.bytes_per_sec), 4);
    std::cout << "bytes_per_sec: " << pcm.bytes_per_sec << std::endl;

    infile.read(reinterpret_cast<char*>(&pcm.block_align), 2);
    std::cout << "block_align: " << pcm.block_align << std::endl;

    infile.read(reinterpret_cast<char*>(&pcm.bits_per_sample), 2);
    std::cout << "bits_per_sample: " << pcm.bits_per_sample << std::endl;
    if (pcm.bits_per_sample != 8 && pcm.bits_per_sample != 16 && pcm.bits_per_sample != 24)
        throw std::runtime_error("Number of bits per sample is not normal");

    int32_t num_bytes_per_second = (int32_t)((pcm.channels * pcm.sample_rate * pcm.bits_per_sample) / 8);
    std::cout << "num_bytes_per_second: " << num_bytes_per_second << std::endl;

    int16_t num_bytes_per_block = pcm.channels * (pcm.bits_per_sample / 8);
    std::cout << "num_bytes_per_block: " << num_bytes_per_block << std::endl;

    int num_bytes_per_sample = pcm.bits_per_sample / 8;
    std::cout << "num_bytes_per_sample: " << num_bytes_per_sample << std::endl;

    if (num_bytes_per_second != (pcm.channels * pcm.sample_rate * pcm.bits_per_sample) / 8)
        throw std::runtime_error("Header is corrupted");
    if (num_bytes_per_block != (pcm.channels * num_bytes_per_sample))
        throw std::runtime_error("Header is corrupted");
}

void WavReader::ignoreUnknown(std::ifstream& infile, uint32_t size)
{
    if (infile.tellg() < 0)
        return;
    std::cout << "    ignoring: " << size << " bytes" << std::endl;
    infile.ignore(size);
}

void WavReader::parseData(std::ifstream& infile, uint32_t size, PcmModel& pcm)
{
    int16_t num_bytes_per_block = pcm.channels * (pcm.bits_per_sample / 8);
    int samples = size / (pcm.channels * num_bytes_per_block);
    for (int channel = 0; channel < pcm.channels; ++channel) {
        pcm.channel_data[channel].resize(samples);
    }
    std::cout << "samples: " << samples << std::endl;
    std::cout << "channels: " << pcm.channels << std::endl;
    double dmax = double(1 << (pcm.bits_per_sample));
    for (int sample = 0; sample < samples; ++sample) {
        for (int channel = 0; channel < pcm.channels; ++channel) {
            if (pcm.bits_per_sample == 8) {
                int32_t data = 0;
                infile.read(reinterpret_cast<char*>(&data), 1);
                pcm.channel_data[channel][sample] = (double)data / dmax;
            } else if (pcm.bits_per_sample == 16) {
                int16_t data;
                infile.read(reinterpret_cast<char*>(&data), 2);
                pcm.channel_data[channel][sample] = (double)data / dmax;
            } else if (pcm.bits_per_sample == 24) {
                uint8_t data0 = 0;
                infile.read(reinterpret_cast<char*>(&data0), 1);
                uint8_t data1 = 0;
                infile.read(reinterpret_cast<char*>(&data1), 1);
                uint8_t data2 = 0;
                infile.read(reinterpret_cast<char*>(&data2), 1);
                int32_t data = (data2 << 16) | (data1 << 8) | data0;
                if (data & 0x800000)
                    data = data | ~0xFFFFFF;

                pcm.channel_data[channel][sample] = (double)data / dmax;
            }
        }
    }

    for (size_t i = 0; i < pcm.channels; ++i) {
        std::cout << "channel[" << i << "].size = " << pcm.channel_data[i].size() << std::endl;
    }
}

void WavReader::load(std::string fname, PcmModel& model)
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
            parseFmt(infile, model);
            continue;
        }

        if (chunk_name == "data") {
            parseData(infile, size, model);
            continue;
        }

        ignoreUnknown(infile, size);
    }
}
