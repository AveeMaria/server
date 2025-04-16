#pragma once

constexpr int SCREEN_WIDTH = 1152;
constexpr int SCREEN_HEIGHT = 640;

constexpr int MAP_COLS = 9;
constexpr int MAP_ROWS = 5;

constexpr int TILESIZE = SCREEN_HEIGHT / MAP_ROWS;

constexpr int FPS = 100;

constexpr int DEBOUNCE_DELAY = 80;
constexpr int DOUBLE_CLICK_DELAY = 170;

constexpr int TEXTURE_SIZE = 128;

constexpr uint32_t max_ping_time = 5000;