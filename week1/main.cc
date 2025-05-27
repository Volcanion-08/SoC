#include <iostream>

int main() {

    // Image

    int image_width = 256;
    int image_height = 256;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    int col[3] = { 255, 255, 0 };

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

        double val= 1-j/255.99;
        for (int i = 0; i < image_width; i++) {
            double sat= i/255.99;
            int ir = int((col[0]*sat + 255*(1-sat))*val);
            int ig = int((col[1]*sat + 255*(1-sat))*val);
            int ib = int((col[2]*sat + 255*(1-sat))*val);
            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    std::clog << "\rDone.                 \n";
}
