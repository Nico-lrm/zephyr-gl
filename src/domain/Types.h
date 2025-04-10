#pragma once
#include <string_view>

namespace zephyr {
    static constexpr std::string_view APP_NAME = "ZephyrEngine";

    struct Extent2D
    {
        int width_ = 0;
        int height_ = 0;
    };

};
