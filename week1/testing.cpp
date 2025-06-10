#include "vec3.h"
#include <iostream>

using namespace std;

int main() {
    cout << "input A and B(B is also normal)" << endl;
    int x1, y1, z1, x2, y2, z2;
    cin >> x1 >> y1 >> z1>> x2 >> y2 >> z2;
    vec3 A(x1,y1,z1), B(x2,y2,z2);
    cout << A.refract(B, 1.333) << endl;
    return 0;
}