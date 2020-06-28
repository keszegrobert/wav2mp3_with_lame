#include <dirent.h>
#include <iostream>
#include <lame/lame.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "This tool converts a folder of wav files to mp3" << std::endl;
        std::cout << "  Usage: wav2mp3 folder" << std::endl;
        return EXIT_FAILURE;
    }

    DIR* dir = opendir(argv[1]);
    if (dir == nullptr) {
        std::cout << "Folder does not exist:" << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    struct dirent* ent = readdir(dir);
    while (ent != NULL) {
        std::string filename(ent->d_name);
        if (filename.rfind(".wav") == std::string::npos) {
            ent = readdir(dir);
            continue;
        }
        std::cout << ent->d_name << std::endl;
        ent = readdir(dir);
    }

    lame_t lame = lame_init();
    std::cout << "Hello, lame " << get_lame_version() << std::endl;
    lame_close(lame);
    return EXIT_SUCCESS;
}
