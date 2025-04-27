#pragma once

#include "Types.h"

#include <concepts>
#include <memory>
#include <optional>

namespace zephyr
{
    class Window;

    template<typename T>
    concept GraphicsDeviceConcept =
        std::constructible_from<T, const Window*> && requires(T t, std::optional<Extent2D> extent) {
            { t.begin_frame() } -> std::same_as<void>;
            { t.end_frame() } -> std::same_as<void>;
            { t.submit() } -> std::same_as<void>;
            { t.present() } -> std::same_as<void>;
            { t.wait_idle() } -> std::same_as<void>;
            { t.resize_swapchain(extent) } -> std::same_as<void>;
            { t.get_VRAM_usage() } -> std::convertible_to<double>;
            { t.get_frame_state() } -> std::convertible_to<FrameState>;
        };

    class GraphicsDevice
    {
      public:
        template<typename T>
            requires GraphicsDeviceConcept<T>
        explicit GraphicsDevice(std::shared_ptr<T> obj_ptr)
            : concept_(std::make_shared<ConcreteGraphicsDevice<T>>(obj_ptr))
        {}

        void begin_frame() const { concept_->begin_frame(); }
        void end_frame() const { concept_->end_frame(); }
        void submit() const { concept_->submit(); }
        void present() const { concept_->present(); }
        void wait_idle() const { concept_->wait_idle(); }
        void resize_swapchain(const std::optional<Extent2D> extent) const { concept_->resize_swapchain(extent); }
        [[nodiscard]] double get_VRAM_usage() const noexcept { return concept_->get_VRAM_usage(); };
        [[nodiscard]] FrameState get_frame_state() const noexcept { return concept_->get_frame_state(); };

      private:
        struct IGraphicsDevice
        {
            virtual ~IGraphicsDevice() = default;

            // Drawing
            virtual void begin_frame() = 0;
            virtual void end_frame() = 0;
            virtual void submit() = 0;
            virtual void present() = 0;
            virtual void wait_idle() const = 0;
            virtual void resize_swapchain(std::optional<Extent2D> extent) = 0;

            // Commands

            // Utility
            [[nodiscard]] virtual double get_VRAM_usage() const noexcept = 0;
            [[nodiscard]] virtual FrameState get_frame_state() const noexcept = 0;

            // Creator
            // Des trucs pour créer Pipeline, Shader, Buffer, Texture, CommandList...
        };

        template<typename T>
        struct ConcreteGraphicsDevice final : IGraphicsDevice
        {
            explicit ConcreteGraphicsDevice(std::shared_ptr<T> obj_ptr) noexcept : object_ptr_(obj_ptr) {}
            void begin_frame() override { object_ptr_->begin_frame(); }
            void end_frame() override { object_ptr_->end_frame(); }
            void submit() override { object_ptr_->submit(); }
            void present() override { object_ptr_->present(); }
            void wait_idle() const override { object_ptr_->wait_idle(); }
            void resize_swapchain(const std::optional<Extent2D> extent) override
            {
                object_ptr_->resize_swapchain(extent);
            }
            [[nodiscard]] double get_VRAM_usage() const noexcept override { return object_ptr_->get_VRAM_usage(); };
            [[nodiscard]] FrameState get_frame_state() const noexcept override
            {
                return object_ptr_->get_frame_state();
            };

            std::shared_ptr<T> object_ptr_;
        };

        std::shared_ptr<IGraphicsDevice> concept_;
    };

} // namespace zephyr
