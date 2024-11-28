#ifndef LOTUSAPP_H
#define LOTUSAPP_H

#include "../lotus.core/defines.h"
#include "../lotus.core/lotustypes.h"

// user-defined function to configure an app
extern b8 ltConfigureApp(LTapplication* app);

b8 ltRunApplication(void);
b8 ltInitApplication(LTapplication* app);

#endif