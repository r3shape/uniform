#include <uniform.h>

UFApplication MyApp;

none appInit(none) {}

none appExit(none) {}

none appUpdate(none) {}

none appRender(none) {}

UFApplication* ufExport(none) {
    MyApp = (UFApplication) {
        .ver = 1, .api = 1, .ctx = 1,
        .windowPos=(Vec2){ .data={100, 100} },
        .windowSize=(Vec2){ .data={1280, 720} },
        .init_hook = appInit, .exit_hook = appExit,
        .update_hook = appUpdate, .render_hook = appRender
    };
    return &MyApp;
}
