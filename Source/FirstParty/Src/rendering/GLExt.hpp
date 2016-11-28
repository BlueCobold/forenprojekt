#ifndef POINTER_CPP_GENERATED_HEADEROPENGL_HPP
#define POINTER_CPP_GENERATED_HEADEROPENGL_HPP

#if defined(__glew_h__) || defined(__GLEW_H__)
#error Attempt to include auto-generated header after including glew.h
#endif
#if defined(__gl_h_) || defined(__GL_H__)
#error Attempt to include auto-generated header after including gl.h
#endif
#if defined(__glext_h_) || defined(__GLEXT_H_)
#error Attempt to include auto-generated header after including glext.h
#endif
#if defined(__gltypes_h_)
#error Attempt to include auto-generated header after gltypes.h
#endif
#if defined(__gl_ATI_h_)
#error Attempt to include auto-generated header after including glATI.h
#endif

#define __glew_h__
#define __GLEW_H__
#define __gl_h_
#define __GL_H__
#define __glext_h_
#define __GLEXT_H_
#define __gltypes_h_
#define __gl_ATI_h_

#ifndef APIENTRY
    #if defined(__MINGW32__)
        #ifndef WIN32_LEAN_AND_MEAN
            #define WIN32_LEAN_AND_MEAN 1
        #endif
        #ifndef NOMINMAX
            #define NOMINMAX
        #endif
        #include <windows.h>
    #elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
        #ifndef WIN32_LEAN_AND_MEAN
            #define WIN32_LEAN_AND_MEAN 1
        #endif
        #ifndef NOMINMAX
            #define NOMINMAX
        #endif
        #include <windows.h>
    #else
        #define APIENTRY
    #endif
#endif /*APIENTRY*/

#ifndef CODEGEN_FUNCPTR
    #define CODEGEN_REMOVE_FUNCPTR
    #if defined(_WIN32)
        #define CODEGEN_FUNCPTR APIENTRY
    #else
        #define CODEGEN_FUNCPTR
    #endif
#endif /*CODEGEN_FUNCPTR*/

#ifndef GLAPI
    #define GLAPI extern
#endif


#ifndef GL_LOAD_GEN_BASIC_OPENGL_TYPEDEFS
#define GL_LOAD_GEN_BASIC_OPENGL_TYPEDEFS


#endif /*GL_LOAD_GEN_BASIC_OPENGL_TYPEDEFS*/

#include <stddef.h>
#ifndef GLEXT_64_TYPES_DEFINED
/* This code block is duplicated in glxext.h, so must be protected */
#define GLEXT_64_TYPES_DEFINED
/* Define int32_t, int64_t, and uint64_t types for UST/MSC */
/* (as used in the GL_EXT_timer_query extension). */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#include <inttypes.h>
#elif defined(__sun__) || defined(__digital__)
#include <inttypes.h>
#if defined(__STDC__)
#if defined(__arch64__) || defined(_LP64)
typedef long int int64_t;
typedef unsigned long int uint64_t;
#else
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#endif /* __arch64__ */
#endif /* __STDC__ */
#elif defined( __VMS ) || defined(__sgi)
#include <inttypes.h>
#elif defined(__SCO__) || defined(__USLC__)
#include <stdint.h>
#elif defined(__UNIXOS2__) || defined(__SOL64__)
typedef long int int32_t;
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#elif defined(_WIN32) && defined(__GNUC__)
#include <stdint.h>
#elif defined(_WIN32)
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
/* Fallback if nothing above works */
#include <inttypes.h>
#endif
#endif
    typedef unsigned int GLenum;
    typedef unsigned char GLboolean;
    typedef unsigned int GLbitfield;
    typedef void GLvoid;
    typedef signed char GLbyte;
    typedef short GLshort;
    typedef int GLint;
#ifdef ANDROID
    typedef char GLubyte;
#else
    typedef unsigned char GLubyte;
#endif
    typedef unsigned short GLushort;
    typedef unsigned int GLuint;
    typedef int GLsizei;
    typedef float GLfloat;
    typedef float GLclampf;
    typedef double GLdouble;
    typedef double GLclampd;
    typedef char GLchar;
    typedef char GLcharARB;
    #ifdef __APPLE__
typedef void *GLhandleARB;
#else
typedef unsigned int GLhandleARB;
#endif
    typedef unsigned short GLhalfARB;
    typedef unsigned short GLhalf;
    typedef GLint GLfixed;
    typedef ptrdiff_t GLintptr;
    typedef ptrdiff_t GLsizeiptr;
    typedef int64_t GLint64;
    typedef uint64_t GLuint64;
    typedef ptrdiff_t GLintptrARB;
    typedef ptrdiff_t GLsizeiptrARB;
    typedef int64_t GLint64EXT;
    typedef uint64_t GLuint64EXT;
    typedef struct __GLsync *GLsync;
    struct _cl_context;
    struct _cl_event;
    typedef void (APIENTRY *GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
    typedef void (APIENTRY *GLDEBUGPROCARB)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
    typedef void (APIENTRY *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,const GLchar *message,void *userParam);
    typedef unsigned short GLhalfNV;
    typedef GLintptr GLvdpauSurfaceNV;

namespace gl
{
    namespace exts
    {
        class LoadTest
        {
        private:
            //Safe bool idiom. Joy!
        typedef void (LoadTest::*bool_type)() const;
            void big_long_name_that_really_doesnt_matter() const {}

        public:
            operator bool_type() const
            {
                return m_isLoaded ? &LoadTest::big_long_name_that_really_doesnt_matter : 0;
            }

            int GetNumMissing() const {return m_numMissing;}
            bool GetLoaded() const {return m_isLoaded;}

            LoadTest() : m_isLoaded(false), m_numMissing(0) {}
            LoadTest(bool isLoaded, int numMissing) : m_isLoaded(isLoaded), m_numMissing(numMissing) {}
        private:
            bool m_isLoaded;
            int m_numMissing;
        };
    } //namespace exts

    enum
    {

        CURRENT_PROGRAM                  = 0x8B8D,
        EXTENSIONS                       = 0x1F03,
        VERSION                          = 0x1F02,
        MAJOR_VERSION                    = 0x821B,
        MINOR_VERSION                    = 0x821C,
        NUM_EXTENSIONS                   = 0x821D,
        MAX_TEXTURE_SIZE                 = 0x0D33,
        TEXTURE0                         = 0x84C0,

        STENCIL_TEST                     = 0x0B90,
        STENCIL_VALUE_MASK               = 0x0B93,
        STENCIL_WRITEMASK                = 0x0B98,
        DEPTH_BUFFER_BIT                 = 0x00000100,
        STENCIL_BUFFER_BIT               = 0x00000400,

        ALPHA_TEST                       = 0x0BC0,
        ALPHA_TEST_FUNC                  = 0x0BC1,
        ALPHA_TEST_REF                   = 0x0BC2,

        DEPTH_TEST                       = 0x0B71,

        INVERT                           = 0x150A,
        KEEP                             = 0x1E00,
        LEFT                             = 0x0406,
        LEQUAL                           = 0x0203,
        EQUAL                            = 0x0202,
        LESS                             = 0x0201,

        TRUE_                            = 1,
        ALWAYS                           = 0x0207,
        GEQUAL                           = 0x0206,
        GREATER                          = 0x0204,
        REPLACE                          = 0x1E01,
    };

    extern void (CODEGEN_FUNCPTR *ActiveTexture)(GLenum texture);
    extern void (CODEGEN_FUNCPTR *AlphaFunc)(GLenum func, GLfloat ref);
    extern void (CODEGEN_FUNCPTR *Flush)();
    extern void (CODEGEN_FUNCPTR *GetIntegerv)(GLenum pname, GLint * params);
    extern GLenum (CODEGEN_FUNCPTR *GetError)();
    extern const GLubyte * (CODEGEN_FUNCPTR *GetString)(GLenum name);
    extern GLint (CODEGEN_FUNCPTR *GetUniformLocation)(GLuint program, const GLchar * name);
    extern void (CODEGEN_FUNCPTR *Uniform1f)(GLint location, GLfloat v0);
    extern void (CODEGEN_FUNCPTR *Uniform1i)(GLint location, GLint v0);

    extern void (CODEGEN_FUNCPTR *ClearStencil)(GLint s);
    extern void (CODEGEN_FUNCPTR *Clear)(GLbitfield mask);

    extern void (CODEGEN_FUNCPTR *StencilFunc)(GLenum func, GLint ref, GLuint mask);
    extern void (CODEGEN_FUNCPTR *StencilMask)(GLuint mask);
    extern void (CODEGEN_FUNCPTR *StencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
    extern void (CODEGEN_FUNCPTR *ColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);

    extern void (CODEGEN_FUNCPTR *DepthFunc)(GLenum func);
    extern void (CODEGEN_FUNCPTR *DepthMask)(GLboolean enabled);
    extern void (CODEGEN_FUNCPTR *ClearDepth)(GLclampd depth);

    extern void (CODEGEN_FUNCPTR *Enable)(GLenum cap);
    extern void (CODEGEN_FUNCPTR *Disable)(GLenum cap);

    namespace sys
    {

        exts::LoadTest LoadFunctions();

        int GetMinorVersion();
        int GetMajorVersion();
        bool IsVersionGEQ(int majorVersion, int minorVersion);

    } //namespace sys
} //namespace gl
#endif //POINTER_CPP_GENERATED_HEADEROPENGL_HPP
