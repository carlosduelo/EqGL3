/* Copyright (c) 2015, Carlos Duelo <cduelo@cesvima.upm.es>
 *
 * Based on eqHello example of Equalizer
 */

#include <seq/sequel.h>
#include <stdlib.h>

namespace EqGL3 
{
class Renderer : public seq::Renderer
{
public:
    Renderer( seq::Application& application ) : seq::Renderer( application ) {}
    virtual ~Renderer() {}

protected:
    virtual void draw( co::Object* frameData );
};

class Application : public seq::Application
{
    virtual ~Application() {}
public:
    virtual seq::Renderer* createRenderer() { return new Renderer( *this ); }
};
typedef lunchbox::RefPtr< Application > ApplicationPtr;
}

int main( const int argc, char** argv )
{
    EqGL3::ApplicationPtr app = new EqGL3::Application;

    if( app->init( argc, argv, 0 ) && app->run( 0 ) && app->exit( ))
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}

/** The rendering routine, a.k.a., glutDisplayFunc() */
void EqGL3::Renderer::draw( co::Object* /*frameData*/ )
{
    applyRenderContext(); // set up OpenGL State

    const float lightPos[] = { 0.0f, 0.0f, 1.0f, 0.0f };
    glLightfv( GL_LIGHT0, GL_POSITION, lightPos );

    const float lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbient );

    applyModelMatrix(); // global camera

    // render six axis-aligned colored quads around the origin
    for( int i = 0; i < 6; i++ )
    {
        glColor3f( (i&1) ? 0.5f:1.0f, (i&2) ? 1.0f:0.5f, (i&4) ? 1.0f:0.5f );

        glNormal3f( 0.0f, 0.0f, 1.0f );
        glBegin( GL_TRIANGLE_STRIP );
            glVertex3f(  .7f,  .7f, -1.0f );
            glVertex3f( -.7f,  .7f, -1.0f );
            glVertex3f(  .7f, -.7f, -1.0f );
            glVertex3f( -.7f, -.7f, -1.0f );
        glEnd();

        if( i < 3 )
            glRotatef(  90.0f, 0.0f, 1.0f, 0.0f );
        else if( i == 3 )
            glRotatef(  90.0f, 1.0f, 0.0f, 0.0f );
        else
            glRotatef( 180.0f, 1.0f, 0.0f, 0.0f );
    }
}
