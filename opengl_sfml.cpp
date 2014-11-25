//
// This file is published as no copyright, you can use it to do anything you like,
// but I am not responsible for the consequences.
// If you use it, make a link to my blog is an appreciation.
// Website: http://glx.me/docs/distilled-book/
//

#include <GL/gl.h>
#include <SFML/Graphics.hpp>

namespace OpenGL
{
    void Render()
    {
        // Clear the color buffer with the color set by glClearColor().
        glClear(GL_COLOR_BUFFER_BIT);
        // Load an identity matrix to model-view matrix stack.
        glLoadIdentity();
        // Set red as the object's color.
        glColor3f(1.0, 0.0, 0.0);
        // Draw a rectangle.
        glRectf(-1.0, -1.0, 1.0, 1.0);
    }

    void Reset(int w, int h)
    {
        // Set the viewport to full window.
        glViewport(0, 0, w, h);
        // Switch into projection matrix mode.
        glMatrixMode(GL_PROJECTION);
        // Load an identity matrix to projection matrix stack.
        glLoadIdentity();
        // Set to ortho projection: left:-2.0, right:2.0,
        // bottom:-2.0, top:2.0, near:1.0, far:-1.0.
        glOrtho( -2.0, 2.0, -2.0, 2.0, 1.0, -1.0 );
        // Switch back into modelview matrix mode.
        glMatrixMode(GL_MODELVIEW);
    }

    void Init()
    {
        // Set the color buffer to be clear: gray.
        glClearColor(0.3, 0.3, 0.3, 0.0);
    }

    class Window: public sf::RenderWindow
    {
        private:
            sf::RenderWindow* m_window;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(640,480), "OpenGL Hello World! -with sfml");
    window.PreserveOpenGLStates(true);
    OpenGL::Init();
    OpenGL::Reset(640, 480);
    sf::Event event;
    while(window.IsOpened())
    {
        while(window.GetEvent(event))
        {
            if( event.Type == sf::Event::Resized )
            {
                OpenGL::Reset(event.Size.Width, event.Size.Height );
            }
            if( event.Type == sf::Event::Closed )
            {
                window.Close();
            }
            if( event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Escape)
            {
                window.Close();
            }
        }
        OpenGL::Render();
        // It is necessary to call Display() method of sf::RenderWindow,
        // otherwise the buffers will not be swapped.
        window.Display();
    }
    return 0;
}
