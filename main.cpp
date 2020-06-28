#include "wavfile.h"
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <lame/lame.h>
#include <sstream>
#include <stdexcept>
#include <vector>

const std::vector<std::string> get_wav_files(std::string folder)
{
    DIR* dir = opendir(folder.c_str());
    if (dir == nullptr) {
        throw std::runtime_error("Folder does not exist: " + folder);
    }
    std::vector<std::string> wavs;
    while (true) {
        struct dirent* ent = readdir(dir);
        if (ent == nullptr)
            break;
        std::string filename(ent->d_name);
        if (filename.rfind(".wav") == std::string::npos) {
            continue;
        }
        wavs.push_back(folder + "/" + filename);
    }
    return wavs;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "This tool converts a folder of wav files to mp3" << std::endl;
        std::cout << "  Usage: wav2mp3 folder" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Hello, lame " << get_lame_version() << std::endl;

    lame_t lame = lame_init();

    std::vector<std::string> paths;

    try {
        paths = get_wav_files(argv[1]);
        for (std::string fname : paths) {
            std::cout << fname << std::endl;
            WavFile wave;
            wave.parse(fname);
        }

    } catch (const std::exception& e) {
        std::cout << "RUNTIME ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    lame_close(lame);
    return EXIT_SUCCESS;
}
