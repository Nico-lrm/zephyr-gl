#pragma once
#include <stdexcept>
#include <string_view>

namespace zephyr
{
    static constexpr std::string_view APP_NAME = "ZephyrEngine";

    struct Extent2D
    {
        int width_ = 0;
        int height_ = 0;
    };

    enum class FrameState
    {
        RECORDING,
        END_RECORD,
        SUBMITTED,
        READY
    };

    inline std::ostream& operator<<(std::ostream& os, const FrameState& state) {
        std::string state_str;
        switch (state) {
        case FrameState::RECORDING:
            state_str = "RECORDING";
            break;
        case FrameState::END_RECORD:
            state_str = "END_RECORD";
            break;
        case FrameState::SUBMITTED:
            state_str = "SUBMITTED";
            break;
        case FrameState::READY:
            state_str = "READY";
            break;
        default:
            throw std::invalid_argument("Invalid FrameState");
        }
        return os << state_str;
    }

}; // namespace zephyr
