#include <noise/noise.h>

#include <iostream>

using namespace noise;


int main(int argc, char const *argv[])
{
    module::Perlin my_module;
    double value = my_module.GetValue(1.25, 0.75, 0.5);

    std::cout << value << std::endl;

    return 0;
}
