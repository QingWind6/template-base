#include "app.h"

#include "hal.h"

namespace template_base {

void App::Run() {
    HAL::GetHAL().init();
}

}  // namespace template_base
