#include <iostream>
#include <vec/vec3.h>

int main() {

    vec3 U(2,3,4);

    std::cout << U.getX() << ", " << U.getY() << ", " << U.getZ() << std::endl;

    U.setX(4);
    U.setY(2);
    U.setZ(3);

    U.print();

    std::cout << "Length: " << U.length() << std::endl;

    std::cout << "Negation: ";
    U.negate().print();

    std::cout << "Normalize: ";
    U.normalize().print();

    return 0;
}