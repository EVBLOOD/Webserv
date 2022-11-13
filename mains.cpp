#include "text.hpp"

typedef void (*x)(void);


int main()
{
    x names[] = {&saad<0>,&saad<1>, &saad<2>};
    for (int i = 0; i < 3; i++)
    {
        names[i]();
    }
}