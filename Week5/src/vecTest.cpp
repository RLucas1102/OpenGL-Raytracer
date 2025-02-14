#include <iostream>
#include <vec/vec3.h>

int main() {

    vec3 U(2,3,4);
    vec3 V(1,2,3);

    std::cout << U.getX() << ", " << U.getY() << ", " << U.getZ() << std::endl;

    U.setX(4);
    U.setY(2);
    U.setZ(3);

    std::cout << "U: ";
    U.print();

    std::cout << "V: ";
    V.print();

    std::cout << "Length: " << U.length() << std::endl;

    std::cout << "Negation: ";
    U.negate().print();

    std::cout << "Normalize: ";
    U.normalize().print();

    std::cout << "U + V: ";
    U.add(V);
    U.print();

    std::cout << "U - V: ";
    U.subtract(V);
    U.print();

    std::cout << "5 * U: ";
    U.multiply(5);
    U.print();

    std::cout << "U . V: " << U.dot(V) << std::endl;

    std::cout << "U x V: ";
    U.cross(V).print();


    return 0;
}