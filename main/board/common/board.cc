#include "board.h"

#include "esp_random.h"
#include "esp_log.h"
#include "esp_system.h"

#include <cstdio>

namespace {
constexpr const char* kTag = "Board";
}

Board::Board() : uuid_(GenerateUuid()) {}

const char* Board::GetBoardType() const {
    return GetName();
}

const char* Board::GetScreenType() const {
    return "unknown";
}

const char* Board::GetScreenResolution() const {
    return "unknown";
}

const char* Board::GetUuid() const {
    return uuid_.c_str();
}

void Board::PowerOn() {}

void Board::PowerOff() {}

void Board::Reboot() {
    ESP_LOGI(kTag, "Reboot requested");
    esp_restart();
}

void Board::PrepareForDeepSleep() {}

std::string Board::GenerateUuid() const {
    uint8_t uuid[16] = {};
    esp_fill_random(uuid, sizeof(uuid));

    uuid[6] = (uuid[6] & 0x0F) | 0x40;
    uuid[8] = (uuid[8] & 0x3F) | 0x80;

    char uuid_str[37] = {};
    snprintf(uuid_str,
             sizeof(uuid_str),
             "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
             uuid[0],
             uuid[1],
             uuid[2],
             uuid[3],
             uuid[4],
             uuid[5],
             uuid[6],
             uuid[7],
             uuid[8],
             uuid[9],
             uuid[10],
             uuid[11],
             uuid[12],
             uuid[13],
             uuid[14],
             uuid[15]);
    return uuid_str;
}
