#include <iostream>
#include <lame/lame.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "This tool converts a folder of wav files to mp3" << std::endl;
        std::cout << "  Usage: wav2mp3 folder" << std::endl;
        return EXIT_FAILURE;
    }
    lame_t lame = lame_init();
    std::cout << "Hello, lame " << get_lame_version() << std::endl;
    lame_close(lame);
    return EXIT_SUCCESS;
}
