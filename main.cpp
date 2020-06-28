#include <iostream>
#include <lame/lame.h>

int main(int, char**)
{
    lame_t lame = lame_init();
    std::cout << "Hello, lame,  " << get_lame_version() << std::endl;
    lame_close(lame);
}
