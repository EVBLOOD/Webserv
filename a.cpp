#include "test.hpp"
a::a()
{
    std::cout << "nothing!\n";
}
template<>
   void a::anounce<0>()
{
     std::cout << "looooooool0!\n";
}
template<>
   void a::anounce<1>()
{
    std::cout << "looooooool!\n";
    this->anounce<0>();
}