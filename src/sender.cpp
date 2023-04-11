#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 1)
    {
        std::cout << "There must be only one argument: binary file name\n";
    }
    std::cout << "Hello world\n";
    return 0;
}