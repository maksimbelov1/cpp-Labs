#include <lib/number.h>
#include <iostream>

int main() {
    uint239_t value;
    //std::cout << value << std::endl;
    uint239_t a = FromInt(10, 10);
    a += FromInt(50, 140);
    //uint239_t b =  FromString("128",171) / FromString("64",75) ;
    // bool b = (FromInt(30, 3) != FromInt(3,10));
     std::cout <<  a << '\n';



    return 0;
}
