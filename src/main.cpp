#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Platform/GLContext.h>

using namespace Magnum;

class MyApplication : public Platform::Application
{
  public:
    MyApplication(const Arguments &arguments);

  private:
    void drawEvent() override;
};

MyApplication::MyApplication(const Arguments &arguments) : Platform::Application{arguments}
{
    using namespace Math::Literals;
    /* Set clear color to dark blue */
    GL::Renderer::setClearColor(0x000066_rgbf);
}

void MyApplication::drawEvent()
{
    /* Clear the window */
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

    /* The context is double-buffered, swap buffers */
    swapBuffers();
}

/* main() function implementation */
MAGNUM_APPLICATION_MAIN(MyApplication)