#include "app/app.h"

extern "C" void app_main(void) {
    template_base::App app;
    app.Run();
}
