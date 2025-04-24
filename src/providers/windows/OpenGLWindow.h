#pragma once

#include "../../domain/Window.h"

#include <concepts>
#include <memory>

namespace zephyr
{
    template<typename T>
    concept OpenGLWindowConcept = WindowConcept<T> && requires(T t) {
        { t.swap_buffers() } -> std::same_as<void>;
    };

    class OpenGLWindow final : public Window
    {
      public:
        ~OpenGLWindow() override = default;

        template<typename T>
            requires OpenGLWindowConcept<T>
        explicit OpenGLWindow(std::shared_ptr<T> obj_ptr)
            : Window(obj_ptr), concept_(std::make_shared<ConcreteOpenGLWindow<T>>(obj_ptr))
        {}

        void swap_buffers() const { concept_->swap_buffers(); }

      private:
        struct IOpenGLWindow
        {
            virtual ~IOpenGLWindow() = default;
            virtual void swap_buffers() const = 0;
        };

        template<typename T>
        struct ConcreteOpenGLWindow final : IOpenGLWindow
        {
            explicit ConcreteOpenGLWindow(std::shared_ptr<T> obj_ptr) noexcept : object_ptr_(obj_ptr) {}
            void swap_buffers() const override { object_ptr_->swap_buffers(); }

            std::shared_ptr<T> object_ptr_;
        };

        std::shared_ptr<IOpenGLWindow> concept_;
    };

} // namespace zephyr
