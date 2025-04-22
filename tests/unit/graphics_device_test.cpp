#include "../../src/domain/GraphicsDevice.h"

#include <boost/test/unit_test.hpp>

using namespace zephyr;

#define TEST_WIDTH 1280u
#define TEST_HEIGHT 720u
#define TEST_VRAM_USAGE 1000.0

struct FakeGraphicsDevice
{
    FrameState actual_frame_state = FrameState::READY;
    bool process_called = false;
    mutable bool idle = false;
    const double vram_usage{ TEST_VRAM_USAGE };
    Extent2D extent{ TEST_WIDTH, TEST_HEIGHT };

    void begin_frame()
    {
        actual_frame_state = FrameState::RECORDING;
    }
    void end_frame()
    {
        actual_frame_state = FrameState::END_RECORD;
    }
    void submit()
    {
        actual_frame_state = FrameState::SUBMITTED;
    }
    void present()
    {
        actual_frame_state = FrameState::READY;
    }
    void wait_idle() const
    {
        idle = true;
    }
    void resize_swapchain(const std::optional<Extent2D> extent)
    {
        if(extent.has_value())
        {
            this->extent = *extent;
        }
    }
    [[nodiscard]] double get_VRAM_usage() const noexcept { return vram_usage; };
    [[nodiscard]] FrameState get_frame_state() const noexcept { return actual_frame_state; };
};

BOOST_AUTO_TEST_CASE(GraphicsDeviceDelegatesCorrectly)
{
    const auto fake = std::make_shared<FakeGraphicsDevice>();
    const GraphicsDevice graphics_device(fake);

    graphics_device.begin_frame();
    BOOST_CHECK_EQUAL(fake->actual_frame_state, FrameState::RECORDING);
    graphics_device.end_frame();
    BOOST_CHECK_EQUAL(fake->actual_frame_state, FrameState::END_RECORD);
    graphics_device.submit();
    BOOST_CHECK_EQUAL(fake->actual_frame_state, FrameState::SUBMITTED);
    graphics_device.present();
    BOOST_CHECK_EQUAL(fake->actual_frame_state, FrameState::READY);

    graphics_device.wait_idle();
    BOOST_CHECK(fake->idle);

    graphics_device.resize_swapchain(std::nullopt);
    BOOST_CHECK_EQUAL(fake->extent.width_, TEST_WIDTH);
    BOOST_CHECK_EQUAL(fake->extent.height_, TEST_HEIGHT);

    graphics_device.resize_swapchain(Extent2D{ 1920, 1080 });
    BOOST_CHECK_EQUAL(fake->extent.width_, 1920);
    BOOST_CHECK_EQUAL(fake->extent.height_, 1080);

    BOOST_CHECK_EQUAL(graphics_device.get_VRAM_usage(), TEST_VRAM_USAGE);
    BOOST_CHECK_EQUAL(graphics_device.get_frame_state(), FrameState::READY);
}
