#include <iostream>
#include <memory>

#include "shared.hpp"



int main()
{
    {
        _shared_pointer<int> sh;

        int* s = new int();

        *s = 100;

        {
            _shared_pointer<int> sh1( s );
            sh = sh1;
        }

        std::cout << "Hello World!" << "\n";
    }

    std::shared_ptr<int> a;


    return 0;
}
