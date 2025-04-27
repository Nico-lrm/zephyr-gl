#include "../../src/domain/GraphicsDevice.h"
#include "./../src/domain/Window.h"

#include <boost/test/unit_test.hpp>

using namespace zephyr;

#define TEST_WIDTH 1280u
#define TEST_HEIGHT 720u
#define TEST_VRAM_USAGE 1000.0

struct FakeWindow
{
    bool process_called = false;
    bool close = false;

    void process_events() { process_called = true; }
    [[nodiscard]] bool should_close() const { return close; }
    [[nodiscard]] const void* get_handle() const { return reinterpret_cast<void*>(0x1234); }
    [[nodiscard]] Extent2D get_window_extent() const { return { 1280, 720 }; }
};

struct FakeGraphicsDevice
{
    explicit FakeGraphicsDevice(const Window* window) {};

    FrameState actual_frame_state = FrameState::READY;
    bool process_called = false;
    mutable bool idle = false;
    const double vram_usage{ TEST_VRAM_USAGE };
    Extent2D extent{ TEST_WIDTH, TEST_HEIGHT };

    void begin_frame() { actual_frame_state = FrameState::RECORDING; }
    void end_frame() { actual_frame_state = FrameState::END_RECORD; }
    void submit() { actual_frame_state = FrameState::SUBMITTED; }
    void present() { actual_frame_state = FrameState::READY; }
    void wait_idle() const { idle = true; }
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
    const auto fake_window = Window(std::make_shared<FakeWindow>());
    const auto fake_graphics = std::make_shared<FakeGraphicsDevice>(&fake_window);
    const GraphicsDevice graphics_device(fake_graphics);

    graphics_device.begin_frame();
    BOOST_CHECK_EQUAL(fake_graphics->actual_frame_state, FrameState::RECORDING);
    graphics_device.end_frame();
    BOOST_CHECK_EQUAL(fake_graphics->actual_frame_state, FrameState::END_RECORD);
    graphics_device.submit();
    BOOST_CHECK_EQUAL(fake_graphics->actual_frame_state, FrameState::SUBMITTED);
    graphics_device.present();
    BOOST_CHECK_EQUAL(fake_graphics->actual_frame_state, FrameState::READY);

    graphics_device.wait_idle();
    BOOST_CHECK(fake_graphics->idle);

    graphics_device.resize_swapchain(std::nullopt);
    BOOST_CHECK_EQUAL(fake_graphics->extent.width_, TEST_WIDTH);
    BOOST_CHECK_EQUAL(fake_graphics->extent.height_, TEST_HEIGHT);

    graphics_device.resize_swapchain(Extent2D{ 1920, 1080 });
    BOOST_CHECK_EQUAL(fake_graphics->extent.width_, 1920);
    BOOST_CHECK_EQUAL(fake_graphics->extent.height_, 1080);

    BOOST_CHECK_EQUAL(graphics_device.get_VRAM_usage(), TEST_VRAM_USAGE);
    BOOST_CHECK_EQUAL(graphics_device.get_frame_state(), FrameState::READY);
}
