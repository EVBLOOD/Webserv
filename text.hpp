#include <iostream>

template <int>
void saad()
{
    std::cout << "no space\n";
}


template <>
void saad<1>()
{
    std::cout << "well_done\n";
}
template <>
void saad<2>()
{
    std::cout << "normal staf\n";
}