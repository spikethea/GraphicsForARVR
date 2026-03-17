#include "BasisMatrices.h"

namespace BasisMatrices {
    const float hermite[16] = {
        2, -2, 1, 1,
        -3, 3, -2, -1,
        0, 0, 1, 0,
        1, 0, 0, 0
    };

    const float bezier[16] = {
        -1, 3, -3, 1,
        3, -6, 3, 0,
        -3, 3, 0, 0,
        1, 0, 0, 0
    };

    const float bspline[16] = {
        -1.0f / 6, 3.0f / 6, -3.0f / 6, 1.0f / 6,
        3.0f / 6, -6.0f / 6, 3.0f / 6, 0.0f,
        -3.0f / 6, 0.0f, 3.0f / 6, 0.0f,
        1.0f / 6, 4.0f / 6, 1.0f / 6, 0.0f
    };
}