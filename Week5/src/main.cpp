#include <iostream>
#include <vec/vec3.h>
#include <fstream>

// Prototypes
// ----------
void writeColor(std::ofstream& outputFile, const vec3& pixel_color);

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

            // Create a vector that holds the pixel color
            vec3 pixel_color(double(j) / (img_width-1),
                             double(i) / (img_height-1),
                             double(i) / (img_height-1));

            // Send pixel color to file for output
            writeColor(myFile, pixel_color);

        }
    }

    myFile.close();

    return 0;
}

// Function definitions
// --------------------

// Takes in pixels with three color components, converts them to [0,255], writes to file
void writeColor(std::ofstream& outputFile, const vec3& pixel_color) {
    float r = pixel_color.getX();
    float g = pixel_color.getY();
    float b = pixel_color.getZ();

    // Translate all the [0,1] values to RGB [0,255]
    int rByte = int(255.999 * r);
    int gByte = int(255.999 * g);
    int bByte = int(255.999 * b);

    outputFile << rByte << ' ' << gByte << ' ' << bByte << "\n";

}
