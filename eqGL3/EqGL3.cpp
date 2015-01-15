/* Copyright (c) 2015, Carlos Duelo <cduelo@cesvima.upm.es>
 *
 * Based on eqHello example of Equalizer
 */

#include <GL/glew.h>
#include <GL/gl.h>
#include <seq/sequel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace
{
const std::string _vertexFileName = "shader.vert";
const std::string _fragmentFileName = "shader.frag";

// LIGHTING
//const float _lightPos[] = { 0.0f, 0.0f, 1.0f, 0.0f };
//const float _lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

char* textFileRead( const char *fn )
{
	FILE *fp;
	char *content = NULL;

	int count=0;
	if( fn != NULL )
    {
		fp = fopen( fn,"rt" );
		if( fp != NULL )
        {
            fseek( fp, 0, SEEK_END );
            count = ftell(fp);
            rewind(fp);
			if( count > 0 )
            {
				content = ( char* ) malloc( sizeof(char) * ( count + 1 ) );
				count = fread( content, sizeof(char), count, fp );
				content[ count ] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}
}

namespace EqGL3
{
class Renderer : public seq::Renderer
{
public:
    Renderer( seq::Application& application ) : seq::Renderer( application ) {}
    virtual ~Renderer() {}

protected:
    virtual bool init( co::Object* initData );
    virtual bool exit();

    virtual void draw( co::Object* frameData );

#if 0
    virtual void applyRenderContext();
    virtual void applyModelMatrix();
#endif

private:
    GLuint _program;
    GLuint _f;
    GLuint _v;

    GLuint _vao[6];

    // Vertex Attribute Locations
    GLint _vertexLoc;
    GLint _colorLoc;

    // Uniform variable Locations
    GLint _modelMatrixLoc;
    GLint _viewMatrixLoc;
    GLint _projMatrixLoc;

    bool _setupShaders();
    bool _setupData();
};

class Application : public seq::Application
{
    virtual ~Application() {}
public:
    virtual seq::Renderer* createRenderer() { return new Renderer( *this ); }
};

typedef lunchbox::RefPtr< Application > ApplicationPtr;

bool Renderer::init( co::Object* /*initData*/ )
{
   return _setupData();
}

bool Renderer::exit()
{
    glDeleteVertexArrays( 6, _vao );
    glDeleteProgram(_program);
    glDeleteShader(_v);
    glDeleteShader(_f);

    if( glGetError() != GL_NO_ERROR )
        return false;

    return true;
}

#if 0
void Renderer::applyRenderContext()
{

}

void Renderer::applyModelMatrix()
{
}
#endif

void Renderer::draw( co::Object* /*frameData*/ )
{
    std::cout << "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH" <<std::endl;
    applyRenderContext(); // set up OpenGL State

#if 1
    glUseProgram( _program );
    #if 0
    modelMatrix *= getModelMatrix();
    eq::Matrix4f modelViewMatrix = modelMatrix;

    for( unsigned i = 0; i < 6; i++ )
    {
        if( i < 3 )
        {
            modelViewMatrix.rotate_x( 90.0f );
            modelViewMatrix.rotate_z( 1.0f );
        }
        else if( i == 3 )
        {
            modelViewMatrix.rotate_x( 90.0f );
            modelViewMatrix.rotate_y( 1.0f );
        }
        else
        {
            modelViewMatrix.rotate_x( 180.0f );
            modelViewMatrix.rotate_y( 1.0f );
        }
        // Set Uniforms
        //const eq::Matrix4f invModelMatrix = modelMatrix
        //compute_inverse( modelMatrix, invModelMatrix );
        //glUniformMatrix4fv(viewMatrixLoc,  1, false, invModelMatrix);
        glUniformMatrix4fv( modelViewMatrixLoc,  1, false, modelViewMatrix.array );
        glBindVertexArray( _vao[i] );
        glDrawArrays(GL_TRIANGLES , 0, 2);

        // Check errors
        if( glGetError() != GL_NO_ERROR )
        {
            std::cerr << "Error rendering" << std::endl;
            return;
        }
    }
    #endif
#else
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
#endif
}

bool Renderer::_setupShaders()
{
    char *vs = NULL,*fs = NULL;

    GLint status;

    _v = glCreateShader( GL_VERTEX_SHADER );
    _f = glCreateShader( GL_FRAGMENT_SHADER );

    vs = textFileRead( _vertexFileName.c_str() );
    fs = textFileRead( _fragmentFileName.c_str() );

    const char * vv = vs;
    const char * ff = fs;

    glShaderSource( _v, 1, &vv, NULL );
    glShaderSource( _f, 1, &ff, NULL );

    delete vs;
    delete fs;

    glCompileShader(_v);
    glCompileShader(_f);
    glGetShaderiv( _v, GL_COMPILE_STATUS, &status );
    if( !status )
    {
        std::cerr << "Error compiling vertex shader" << std::endl;
        printShaderInfoLog(_v);
        return false;
    }
    glGetShaderiv( _f, GL_COMPILE_STATUS, &status );
    if( !status )
    {
        std::cerr << "Error compiling fragment shader" << std::endl;
        printShaderInfoLog(_f);
        return false;
    }

    _program = glCreateProgram();

    glAttachShader( _program, _v );
    glAttachShader( _program, _f );

    glBindFragDataLocation( _program, 0, "outputF" );
    glLinkProgram(_program);
    glGetProgramiv( _program, GL_LINK_STATUS, &status );
    if( !status )
    {
        std::cerr << "Error linking program" << std::endl;
        return false;
    }

    // Get Uniforms location
    _modelMatrixLoc = glGetUniformLocation( _program, "EQ_modelMatrix" );
    _viewMatrixLoc = glGetUniformLocation( _program, "EQ_viewMatrix" );
    _projMatrixLoc = glGetUniformLocation( _program, "EQ_projMatrix" );

    // Get Attributes location
    _vertexLoc = glGetAttribLocation( _program, "position" );
    _colorLoc = glGetAttribLocation( _program, "color" );

    if( glGetError() == GL_NO_ERROR )
        return true;

    std::cerr << "Error creating shaders" << std::endl;
    return false;
}

bool Renderer::_setupData()
{
    if( !_setupShaders() )
        return false;

    float vertices[] =  {
                         .7f,  .7f, -1.0f,
                        -.7f,  .7f, -1.0f,
                         .7f, -.7f, -1.0f,
                        -.7f, -.7f, -1.0f,
                        };
    // Create Vertex Arrays
    glGenVertexArrays(6, _vao);

    for( unsigned i = 0; i < 6; i++ )
    {
        GLuint buffers[3];
        float color[] = { ( i & 1 ) ? 0.5f : 1.0f,
                          ( i & 2 ) ? 1.0f : 0.5f,
                          ( i & 4 ) ? 1.0f : 0.5f
                        };
        // Generate buffers
        glGenBuffers(2, buffers);

        // Vertex
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(_vertexLoc);
        glVertexAttribPointer(_vertexLoc, 4, GL_FLOAT, 0, 0, 0);

        // Colors
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
        glEnableVertexAttribArray(_colorLoc);
        glVertexAttribPointer(_colorLoc, 3, GL_FLOAT, 0, 0, 0);

        if( glGetError() != GL_NO_ERROR )
        {
            std::cerr << "Error creating Buffers" << std::endl;
            return false;
        }
    }
    return true;
}
}

int main( const int argc, char** argv )
{
    EqGL3::ApplicationPtr app = new EqGL3::Application;

    if( !app->init( argc, argv, 0 ) && app->run( 0 ) && app->exit( ))
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}
