
#include <iostream>
#include <new>

int main()
{
    int i = 0;
    while(new(std::nothrow) char[1024 * 1024])
    {
        ++i;
    }
    std::cout << i << " MiB";

}
