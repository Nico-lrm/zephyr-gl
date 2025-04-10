#define BOOST_TEST_MODULE ZephyrTests
#include "../../src/domain/Window.h"

#include <boost/test/unit_test.hpp>

using namespace zephyr;

struct FakeWindow
{
    bool process_called = false;
    bool close = false;

    void process_events() { process_called = true; }
    [[nodiscard]] bool should_close() const { return close; }
    [[nodiscard]] const void* get_handle() const { return reinterpret_cast<void*>(0x1234); }
    [[nodiscard]] Extent2D get_window_extent() const { return { 1280, 720 }; }
};

BOOST_AUTO_TEST_CASE(WindowDelegatesCorrectly)
{
    const auto fake = std::make_shared<FakeWindow>();
    Window window(fake);

    window.process_events();
    BOOST_CHECK(fake->process_called);
    BOOST_CHECK_EQUAL(window.should_close(), false);
    BOOST_CHECK_EQUAL(window.get_handle(), reinterpret_cast<void*>(0x1234));
    BOOST_CHECK_EQUAL(window.get_window_extent().width_, 1280);
    BOOST_CHECK_EQUAL(window.get_window_extent().height_, 720);
}
