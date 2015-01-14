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

GLuint program;
GLuint vao[6];
// Vertex Attribute Locations
GLint vertexLoc, colorLoc, rotationLoc;
// Uniform variable Locations
GLint modelMatrixLoc;//, invModelMatrixLoc;

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

bool setupShaders()
{
    char *vs = NULL,*fs = NULL;

    GLuint v,f;
    GLint status;

    v = glCreateShader( GL_VERTEX_SHADER );
    f = glCreateShader( GL_FRAGMENT_SHADER );

    vs = textFileRead( _vertexFileName.c_str() );
    fs = textFileRead( _fragmentFileName.c_str() );

    const char * vv = vs;
    const char * ff = fs;

    glShaderSource( v, 1, &vv, NULL );
    glShaderSource( f, 1, &ff, NULL );

    delete vs;
    delete fs;

    glCompileShader(v);
    glCompileShader(f);
    glGetShaderiv( v, GL_COMPILE_STATUS, &status );
    if( !status )
    {
        std::cerr << "Error compiling vertex shader" << std::endl;
        printShaderInfoLog(v);
        return false;
    }
    glGetShaderiv( f, GL_COMPILE_STATUS, &status );
    if( !status )
    {
        std::cerr << "Error compiling fragment shader" << std::endl;
        printShaderInfoLog(f);
        return false;
    }

    program = glCreateProgram();

    glAttachShader( program, v );
    glAttachShader( program, f );

    glBindFragDataLocation( program, 0, "outputF" );
    glLinkProgram(program);
    glGetProgramiv( program, GL_LINK_STATUS, &status );
    if( !status )
    {
        std::cerr << "Error linking program" << std::endl;
        return false;
    }

    modelMatrixLoc = glGetUniformLocation( program, "modelMatrix" );
    vertexLoc = glGetAttribLocation( program, "position" );
    colorLoc = glGetAttribLocation( program, "color" );
    rotationLoc = glGetAttribLocation( program, "rotation" );

    //invModelMatrixLoc = glGetUniformLocation( p, "invModelMatrix" );
    GLenum glErr = glGetError();
    if( glErr == GL_NO_ERROR )
        return true;

    std::cerr << "Error creating shaders" << std::endl;
    return false;
}

bool setup()
{
    if( !setupShaders() )
        return false;

    float vertices[] =  {
                         .7f,  .7f, -1.0f,
                        -.7f,  .7f, -1.0f,
                         .7f, -.7f, -1.0f,
                        -.7f, -.7f, -1.0f,
                        };
    // Create Vertex Arrays
    glGenVertexArrays(6, vao);

    for( unsigned i = 0; i < 6; i++ )
    {
        GLuint buffers[3];
        float color[] = { ( i & 1 ) ? 0.5f : 1.0f,
                          ( i & 2 ) ? 1.0f : 0.5f,
                          ( i & 4 ) ? 1.0f : 0.5f
                        };
        float rotation[] = { i <= 3 ? 90.0f : 180.0f,
                             i >= 3 ? 1.0f  : 0.0f,
                             i < 3  ? 1.0f  : 0.0f,
                             0.0f
                           };
        // Generate buffers
        glGenBuffers(3, buffers);

        // Vertex
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(vertexLoc);
        glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);

        // Colors
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
        glEnableVertexAttribArray(colorLoc);
        glVertexAttribPointer(colorLoc, 3, GL_FLOAT, 0, 0, 0);

        // Rotations
        glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rotation), rotation, GL_STATIC_DRAW);
        glEnableVertexAttribArray(rotationLoc);
        glVertexAttribPointer(rotationLoc, 4, GL_FLOAT, 0, 0, 0);
    }

    return true;
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
    virtual void draw( co::Object* frameData );
    virtual bool init( co::Object* initData );
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

    if( !app->init( argc, argv, 0 ) )
        return EXIT_FAILURE;

    if ( app->run( 0 ) && app->exit( ))
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}

bool EqGL3::Renderer::init( co::Object* /*initData*/ )
{
   return setup();
}

/** The rendering routine, a.k.a., glutDisplayFunc() */
void EqGL3::Renderer::draw( co::Object* /*frameData*/ )
{
    applyRenderContext(); // set up OpenGL State

#if 1
    glUseProgram( program );
    // Set Uniforms
    const eq::Matrix4f modelMatrix = getModelMatrix();
    //const eq::Matrix4f invModelMatrix = modelMatrix
    //compute_inverse( modelMatrix, invModelMatrix );
    glUniformMatrix4fv(modelMatrixLoc,  1, false, modelMatrix.array);
    GLenum glErr = glGetError();
    if( glErr != GL_NO_ERROR )
    {
        std::cerr << "Error setting uniforms " << modelMatrixLoc <<std::endl;
        return;
    }
    //glUniformMatrix4fv(viewMatrixLoc,  1, false, invModelMatrix);

    for( unsigned i = 0; i < 6; i++ )
    {
        glBindVertexArray(vao[i]);
        glDrawArrays(GL_TRIANGLES , 0, 2);
    }

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
