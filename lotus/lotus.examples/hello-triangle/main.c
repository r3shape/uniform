#include "../../include/lotus.h"

void main() {
    i32 init = lotusInit();

    f32 vertices[] = {
        -0.5, -0.5, 0.0,
         0.5, -0.5, 0.0,
         0.0,  0.5, 0.0
    };

    LTvertexData triangle = ltglVertexData(1, 3, 0, vertices, NULL);

    if (init) {
        while (LOTUS_TRUE) {
            ltPumpEvents();
            ltClearColor();

            ltDraw(&triangle);

            ltSwapBuffers();
        }

        lotusExit();
    }

}
