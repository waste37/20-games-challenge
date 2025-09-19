#pragma once

#include <functional>
#include <utility>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace glfw
{

    class Error : public std::runtime_error {
    public:
        explicit Error(const char *message) noexcept
            : std::runtime_error{ message }
        {
        }
        const char *what() const noexcept
        {
            return std::runtime_error::what();
        }
    };

    namespace detail
    {
        /**
        * @brief Lightweight owning pointer for implementing move semantics to avoid making T* copies
        */

        template <typename T>
        class OwningPtr {
        public:
            OwningPtr(const OwningPtr &) = delete;
            OwningPtr &operator=(const OwningPtr &) = delete;

            constexpr OwningPtr(std::nullptr_t = nullptr) noexcept
                : m_Ptr{}
            {
            }

            constexpr OwningPtr(T *ptr)
                : m_Ptr{ ptr }
            {
            }

            constexpr OwningPtr(OwningPtr &&other) noexcept
                : m_Ptr{ std::exchange(other.m_Ptr, nullptr) }
            {
            }

            constexpr OwningPtr &operator=(OwningPtr &&other) noexcept
            {
                m_Ptr = std::exchange(other.m_Ptr, nullptr);
                return *this;
            }

            [[nodiscard]] constexpr explicit operator bool() const noexcept
            {
                return static_cast<bool>(m_Ptr);
            }

            [[nodiscard]] constexpr operator T *() const noexcept
            {
                return m_Ptr;
            }

        private:
            T *m_Ptr;
        };
    }

    template <typename ...Args>
    class Event {
    public:
        template <typename HandlerType>
        void SetHandler(HandlerType &&handler)
        {
            m_Handler = std::forward<HandlerType>(handler);
        }

        void operator()(Args... args)
        {
            if (m_Handler) {
                m_Handler(args...);
            }
        }

    private:
        std::function<void(Args...)> m_Handler;
    };


    enum class ClientApi {
        OpenGl = GLFW_OPENGL_API,
        OpenGles = GLFW_OPENGL_ES_API,
        None = GLFW_NO_API
    };

    enum class OpenGlProfile {
        Any = GLFW_OPENGL_ANY_PROFILE,
        Compatibility = GLFW_OPENGL_COMPAT_PROFILE,
        Core = GLFW_OPENGL_CORE_PROFILE
    };

    struct WindowHints {
        OpenGlProfile Profile = OpenGlProfile::Any;
        ClientApi ClientApi = ClientApi::OpenGl;
        int ContextVersionMajor = 1;
        int ContextVersionMinor = 0;

        void Apply() const
        {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ContextVersionMajor);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ContextVersionMinor);
            glfwWindowHint(GLFW_CLIENT_API, static_cast<int>(ClientApi));
            glfwWindowHint(GLFW_OPENGL_PROFILE, static_cast<int>(Profile));
        }
    };

    class Window {
    private:
        class HandleContainer : public detail::OwningPtr<GLFWwindow> {
        public:
            using detail::OwningPtr<GLFWwindow>::OwningPtr;
            HandleContainer() noexcept : OwningPtr(nullptr) {}

            HandleContainer(HandleContainer &&other) noexcept
                : detail::OwningPtr<GLFWwindow>{ std::move(other) }
            {
                if (static_cast<GLFWwindow *>(*this)) {
                    SetPointerFromHandle(static_cast<GLFWwindow *>(*this), reinterpret_cast<Window *>(this));
                }
            }

            HandleContainer &operator=(HandleContainer &&other) noexcept
            {
                glfwDestroyWindow(static_cast<GLFWwindow *>(*this));
                static_cast<detail::OwningPtr<GLFWwindow> &>(*this) = std::move(other);
                if (static_cast<GLFWwindow *>(*this)) {
                    SetPointerFromHandle(static_cast<GLFWwindow *>(*this), reinterpret_cast<Window *>(this));
                }

                return *this;
            }

            ~HandleContainer()
            {
                glfwDestroyWindow(static_cast<GLFWwindow *>(*this));
            }
        } m_Handle;

    public:
        explicit Window() noexcept = default;

        explicit Window(std::nullptr_t) noexcept
            : Window{}
        {
        }
        explicit Window(GLFWwindow *handle)
            : m_Handle(handle)
        {
            if (m_Handle) {
                SetPointerFromHandle(m_Handle, this);
                glfwSetWindowSizeCallback(m_Handle, SizeCallback);
            }
        }

        Window(int width, int height, const char *title)
            : Window{ glfwCreateWindow(width, height, title, nullptr, nullptr) }
        {
        }

        Window(const Window &) = delete;

        Window &operator=(const Window &) = delete;

        Window(Window &&other) noexcept = default;

        Window &operator=(Window &&other) noexcept = default;

        operator GLFWwindow *() const
        {
            return m_Handle;
        }

        [[nodiscard]] bool ShouldClose() const
        {
            return glfwWindowShouldClose(m_Handle);
        }

        void SwapBuffers()
        {
            glfwSwapBuffers(m_Handle);
        }

        friend void MakeContextCurrent(const Window &window)
        {
            glfwMakeContextCurrent(window.m_Handle);
        }

        [[nodiscard]] friend Window &GetCurrentContext()
        {
            return GetWrapperFromHandle(glfwGetCurrentContext());
        }

        Event < Window &, int, int> SizeEvent;
    private:
        static void SizeCallback(GLFWwindow *window, int width, int height)
        {
            Window &wrapper = GetWrapperFromHandle(window);
            wrapper.SizeEvent(wrapper, width, height);
        }

        static Window &GetWrapperFromHandle(GLFWwindow *handle)
        {
            return *static_cast<Window *>(glfwGetWindowUserPointer(handle));
        }

        static void SetPointerFromHandle(GLFWwindow *handle, Window *ptr)
        {
            glfwSetWindowUserPointer(handle, ptr);
        }
    };

    inline void MakeContextCurrent(const Window &window);
    [[nodiscard]] inline Window &GetCurrentContext();

    inline void PollEvents()
    {
        glfwPollEvents();
    }

    using GlProc = GLFWglproc;
    [[nodiscard]] inline GlProc GetProcAddress(const char *proc_name)
    {
        return glfwGetProcAddress(proc_name);
    }

    struct Library {
        ~Library()
        {
            glfwTerminate();
        }
        [[nodiscard]] friend Library Init()
        {
            if (!glfwInit()) {
                throw glfw::Error("Could not initialize GLFW");
            }
            return {};
        }
    };

    [[nodiscard]] inline Library Init();
}

namespace glad
{
    class Error : public std::runtime_error {
    public:
        explicit Error(const char *message) noexcept
            : std::runtime_error{ message }
        {
        }
        const char *what() const noexcept
        {
            return std::runtime_error::what();
        }
    };

    using LoadProc = GLADloadproc;
    void InitGLLoader(LoadProc load_proc)
    {
        if (!gladLoadGLLoader(load_proc)) {
            throw Error("Failed to initialize GLAD");
        }
    }
}