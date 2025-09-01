#include <uniform.h>

UFApplication MyApp;

none appInit(none) {}

none appExit(none) {}

none appUpdate(none) {}

none appRender(none) {}

UFApplication* ufExport(none) {
    MyApp = (UFApplication) {
        .ver = 1, .api = 1, .ctx = 1,
        .init_hook = appInit, .exit_hook = appExit,
        .update_hook = appUpdate, .render_hook = appRender
    };
    return &MyApp;
}
