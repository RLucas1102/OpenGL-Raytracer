#include <iostream>
#include <vec/vec3.h>
#include <vec/ray.h>

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
    normalize(U).print();

    std::cout << "U + V: ";
    add(U, V).print();

    std::cout << "U - V: ";
    subtract(U, V).print();

    std::cout << "5 * U: ";
    multiply(U, 5).print();

    std::cout << "U . V: " << dot(U, V) << std::endl;

    std::cout << "U x V: ";
    cross(U, V).print();

    std::cout << "U: ";
    U.print();

    std::cout << "V: ";
    V.print();

    float t = 2;
    vec3 origin(4,2,0);
    vec3 direction(-1, 2, 0);

    ray F(origin, direction);

    F.getOrigin().print();
    F.getDirection().print();
    F.at(t).print();

    return 0;
}