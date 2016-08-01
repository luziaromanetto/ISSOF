#include <iostream>
#include <sstream>
#include <functional>

#include "of/of.h"
#include "ofISS.h"

using namespace std;
using namespace of;

/* ------------------------------------------------------------------ */
/*                    Programa Principal                              */
/* ------------------------------------------------------------------ */
void readConfigurationFile(char fileName[],
        double *xi, double *xf, int *l, double *dx, double *as, double *al, char *output);

int main(int argc, char* argv[]) {
    ofMesh<ofDefault3D> mesh;
    ofISS3D ISS(&mesh);
    ISS.readConfigurationFile(argv[1]);
    
    return 0;
}
