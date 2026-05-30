#include "app.h"

#include "board.h"

namespace template_base {

void App::Run() {
    auto& board = Board::GetInstance();
    board.Initialize();
}

}  // namespace template_base
