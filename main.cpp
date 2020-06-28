#include <dirent.h>
#include <iostream>
#include <lame/lame.h>
#include <sstream>
#include <stdexcept>
#include <vector>

const std::vector<std::string> get_wav_files(std::string folder)
{
    DIR* dir = opendir(folder.c_str());
    if (dir == nullptr) {
        std::stringstream ss;
        ss << "Folder does not exist:" << folder << std::endl;
        throw std::runtime_error(ss.str());
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
        wavs.push_back(filename);
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
    try {
        std::vector<std::string> paths = get_wav_files(argv[1]);
    } catch (const std::exception& e) {
        std::cout << e.what();
        return EXIT_FAILURE;
    }

    lame_t lame = lame_init();
    std::cout << "Hello, lame " << get_lame_version() << std::endl;
    lame_close(lame);
    return EXIT_SUCCESS;
}
