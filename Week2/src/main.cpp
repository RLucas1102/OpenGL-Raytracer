#include <iostream>
#include <fstream>

int main() {

    // File
    std::ofstream myFile;
    myFile.open("image.ppm");

    // Image

    int img_width  = 256;
    int img_height = 256;

    // Render

    myFile << "P3\n" << img_width << ' ' << img_height << "\n255\n";

    for (int i = 0; i < img_height; i++) {
        for (int j = 0; j < img_width; j++) {
            double r = double(j) / (img_width-1);
            double g = double(i) / (img_height-1);
            double b = double(i) / (img_height-1);

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            myFile << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    myFile.close();

    return 0;
}