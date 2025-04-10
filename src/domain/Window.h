#pragma once

#include "Types.h"

#include <concepts>
#include <memory>

namespace zephyr
{
    template<typename T>
    concept WindowConcept = std::movable<T> && requires(T t) {
        { t.process_events() } -> std::same_as<void>;
        { t.should_close() } -> std::convertible_to<bool>;
        requires requires {
            { t.get_handle() };
        };
        { t.get_window_extent() } -> std::same_as<Extent2D>;
    };

    class Window
    {
      public:
        virtual ~Window() = default;

        template<typename T>
            requires WindowConcept<T>
        explicit Window(std::shared_ptr<T> obj_ptr) : concept_(std::make_shared<ConcreteWindow<T>>(obj_ptr))
        {}

        virtual void process_events() { this->concept_->process_events(); };
        [[nodiscard]] bool should_close() const { return this->concept_->should_close(); };
        [[nodiscard]] const void* get_handle() const { return this->concept_->get_handle(); };
        [[nodiscard]] Extent2D get_window_extent() const { return this->concept_->get_window_extent(); };

      private:
        struct IWindow
        {
            virtual ~IWindow() = default;
            virtual void process_events() = 0;
            [[nodiscard]] virtual bool should_close() const = 0;
            [[nodiscard]] virtual const void* get_handle() const = 0;
            [[nodiscard]] virtual Extent2D get_window_extent() const = 0;
        };

        template<typename T> struct ConcreteWindow final : IWindow
        {
            explicit ConcreteWindow(std::shared_ptr<T> obj_ptr) noexcept : object_ptr_(obj_ptr) {}
            void process_events() override { this->object_ptr_->process_events(); };
            [[nodiscard]] bool should_close() const override { return this->object_ptr_->should_close(); };
            [[nodiscard]] const void* get_handle() const override { return this->object_ptr_->get_handle(); };
            [[nodiscard]] Extent2D get_window_extent() const override
            {
                return this->object_ptr_->get_window_extent();
            };

            std::shared_ptr<T> object_ptr_;
        };

        std::shared_ptr<IWindow> concept_;
    };

} // namespace zephyr
