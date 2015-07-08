#include <algorithm>
#include <vector>
#include <string.h>
#include <stddef.h>
#include "GLExt.hpp"

#if defined(__APPLE__) && !defined(IOS)
#include <mach-o/dyld.h>
#include <cstdlib>
static void* AppleGLGetProcAddress (const char *name)
{
    static const struct mach_header* image = NULL;
    NSSymbol symbol;
    char* symbolName;
    if (NULL == image)
        image = NSAddImage("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", NSADDIMAGE_OPTION_RETURN_ON_ERROR);
    /* prepend a '_' for the Unix C symbol mangling convention */
    symbolName = new char[strlen((const char*)name) + 2];
    strcpy(symbolName+1, (const char*)name);
    symbolName[0] = '_';
    symbol = NULL;
    /* if (NSIsSymbolNameDefined(symbolName))
        symbol = NSLookupAndBindSymbol(symbolName); */
    symbol = image ? NSLookupSymbolInImage(image, symbolName, NSLOOKUPSYMBOLINIMAGE_OPTION_BIND | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR) : NULL;
    delete symbolName;
    return symbol ? NSAddressOfSymbol(symbol) : NULL;
}
#endif /* __APPLE__ */

#if defined(IOS)
typedef int32_t  GLclampx;
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <cstdlib>
#endif

#if defined(__sgi) || defined (__sun)
#include <dlfcn.h>
#include <stdio.h>

static void* SunGetProcAddress (const GLubyte* name)
{
    static void* h = NULL;
    static void* gpa;

    if (h == NULL)
    {
        if ((h = dlopen(NULL, RTLD_LAZY | RTLD_LOCAL)) == NULL) return NULL;
        gpa = dlsym(h, "glXGetProcAddress");
    }

    if (gpa != NULL)
        return ((void*(*)(const GLubyte*))gpa)(name);
    else
        return dlsym(h, (const char*)name);
}
#endif /* __sgi || __sun */

#if defined(_WIN32)

#ifdef _MSC_VER
#pragma warning(disable: 4055)
#pragma warning(disable: 4054)
#pragma warning(disable: 4996) // _CRT_SECURE_NO_WARNINGS
#endif

static int TestPointer(const PROC pTest)
{
    ptrdiff_t iTest;
    if(!pTest) return 0;
    iTest = (ptrdiff_t)pTest;

    if(iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1) return 0;

    return 1;
}

static PROC WinGetProcAddress(const char *name)
{
    HMODULE glMod = NULL;
    PROC pFunc = wglGetProcAddress((LPCSTR)name);
    if(TestPointer(pFunc))
        return pFunc;
    glMod = GetModuleHandleA("OpenGL32.dll");
    return (PROC)GetProcAddress(glMod, (LPCSTR)name);
}

#define IntGetProcAddress(name) WinGetProcAddress(name)
#else
    #if defined(__APPLE__)
        #define IntGetProcAddress(name) AppleGLGetProcAddress(name)
    #else
        #if defined(__sgi) || defined(__sun)
            #define IntGetProcAddress(name) SunGetProcAddress(name)
        #else /* GLX */
            #include <GL/glx.h>

            #define IntGetProcAddress(name) (*glXGetProcAddressARB)((const GLubyte*)name)
        #endif
    #endif
#endif

namespace gl
{
    namespace exts
    {
    } //namespace exts
    typedef const GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRING)(GLenum);
    PFNGETSTRING GetString = 0;

    typedef void (CODEGEN_FUNCPTR *PFNACTIVETEXTURE)(GLenum);
    PFNACTIVETEXTURE ActiveTexture = 0;
    typedef void (CODEGEN_FUNCPTR *PFNFLUSH)();
    PFNFLUSH Flush = 0;
    typedef void (CODEGEN_FUNCPTR *PFNGETINTEGERV)(GLenum, GLint *);
    PFNGETINTEGERV GetIntegerv = 0;
    typedef const GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGI)(GLenum, GLuint);
    PFNGETSTRINGI GetStringi = 0;
    typedef GLint (CODEGEN_FUNCPTR *PFNGETUNIFORMLOCATION)(GLuint, const GLchar *);
    PFNGETUNIFORMLOCATION GetUniformLocation = 0;
    typedef void (CODEGEN_FUNCPTR *PFNUNIFORM1I)(GLint, GLint);
    PFNUNIFORM1I Uniform1i = 0;
    typedef void (CODEGEN_FUNCPTR *PFNUNIFORM1F)(GLint, GLfloat);
    PFNUNIFORM1F Uniform1f = 0;
    
    typedef void (CODEGEN_FUNCPTR *PFNALPHAFUNC)(GLenum, GLfloat);
    PFNALPHAFUNC AlphaFunc = 0;
    
    typedef void (CODEGEN_FUNCPTR *PFNCLEAR)(GLbitfield);
    PFNCLEAR Clear = 0;
    typedef void (CODEGEN_FUNCPTR *PFNCLEARSTENCIL)(GLint);
    PFNCLEARSTENCIL ClearStencil = 0;

    typedef void (CODEGEN_FUNCPTR *PFNSTENCILFUNC)(GLenum, GLint, GLuint);
    PFNSTENCILFUNC StencilFunc = 0;
    typedef void (CODEGEN_FUNCPTR *PFNSTENCILMASK)(GLuint);
    PFNSTENCILMASK StencilMask = 0;
    typedef void (CODEGEN_FUNCPTR *PFNSTENCILOP)(GLenum, GLenum, GLenum);
    PFNSTENCILOP StencilOp = 0;
    typedef void (CODEGEN_FUNCPTR *PFNCOLORMASK)(GLboolean, GLboolean, GLboolean, GLboolean);
    PFNCOLORMASK ColorMask = 0;
    
    typedef void (CODEGEN_FUNCPTR *PFNENABLE)(GLenum);
    PFNENABLE Enable = 0;
    typedef void (CODEGEN_FUNCPTR *PFNDISABLE)(GLenum);
    PFNDISABLE Disable = 0;

    static int LoadCoreFunctions()
    {
        int numFailed = 0;
#if !defined(IOS)
        GetString = reinterpret_cast<PFNGETSTRING>(IntGetProcAddress("glGetString"));
        if(!GetString) ++numFailed;
		GetIntegerv = reinterpret_cast<PFNGETINTEGERV>(IntGetProcAddress("glGetIntegerv"));
		if(!GetIntegerv) ++numFailed;

        Uniform1f = reinterpret_cast<PFNUNIFORM1F>(IntGetProcAddress("glUniform1f"));
        if(!Uniform1f) ++numFailed;
        Uniform1i = reinterpret_cast<PFNUNIFORM1I>(IntGetProcAddress("glUniform1i"));
        if(!Uniform1i) ++numFailed;
        ActiveTexture = reinterpret_cast<PFNACTIVETEXTURE>(IntGetProcAddress("glActiveTexture"));
        if(!ActiveTexture) ++numFailed;
        GetUniformLocation = reinterpret_cast<PFNGETUNIFORMLOCATION>(IntGetProcAddress("glGetUniformLocation"));
        if(!GetUniformLocation) ++numFailed;

        AlphaFunc = reinterpret_cast<PFNALPHAFUNC>(IntGetProcAddress("glAlphaFunc"));
        if(!AlphaFunc) ++numFailed;

        StencilFunc = reinterpret_cast<PFNSTENCILFUNC>(IntGetProcAddress("glStencilFunc"));
        if(!StencilFunc) ++numFailed;
        StencilMask = reinterpret_cast<PFNSTENCILMASK>(IntGetProcAddress("glStencilMask"));
        if(!StencilMask) ++numFailed;
        StencilOp = reinterpret_cast<PFNSTENCILOP>(IntGetProcAddress("glStencilOp"));
        if(!StencilOp) ++numFailed;
        ColorMask = reinterpret_cast<PFNCOLORMASK>(IntGetProcAddress("glColorMask"));
        if(!ColorMask) ++numFailed;

        ClearStencil = reinterpret_cast<PFNCLEARSTENCIL>(IntGetProcAddress("glClearStencil"));
        if(!ClearStencil) ++numFailed;
        Clear = reinterpret_cast<PFNCLEAR>(IntGetProcAddress("glClear"));
        if(!Clear) ++numFailed;

        Disable = reinterpret_cast<PFNDISABLE>(IntGetProcAddress("glDisable"));
        if(!Disable) ++numFailed;
        Enable = reinterpret_cast<PFNENABLE>(IntGetProcAddress("glEnable"));
        if(!Enable) ++numFailed;
        Flush = reinterpret_cast<PFNFLUSH>(IntGetProcAddress("glFlush"));
        if(!Flush) ++numFailed;
#else
        GetString = glGetString;
        GetIntegerv = glGetIntegerv;
        ActiveTexture = glActiveTexture;
        AlphaFunc = glAlphaFunc;
        StencilFunc = glStencilFunc;
        StencilMask = glStencilMask;
        StencilOp = glStencilOp;
        ColorMask = glColorMask;
        ClearStencil = glClearStencil;
        Clear = glClear;
        Disable = glDisable;
        Enable = glEnable;
        Flush = glFlush;
#endif
        return numFailed;
    }

    namespace sys
    {
        namespace 
        {
            typedef int (*PFN_LOADEXTENSION)();
            struct MapEntry
            {
                MapEntry(const char *_extName, exts::LoadTest *_extVariable)
                    : extName(_extName)
                    , extVariable(_extVariable)
                    , loaderFunc(0)
                    {}

                MapEntry(const char *_extName, exts::LoadTest *_extVariable, PFN_LOADEXTENSION _loaderFunc)
                    : extName(_extName)
                    , extVariable(_extVariable)
                    , loaderFunc(_loaderFunc)
                    {}

                const char *extName;
                exts::LoadTest *extVariable;
                PFN_LOADEXTENSION loaderFunc;
            };

            struct MapCompare
            {
                MapCompare(const char *test_) : test(test_) {}
                bool operator()(const MapEntry &other) { return strcmp(test, other.extName) == 0; }
                const char *test;
            };

            void InitializeMappingTable(std::vector<MapEntry> &table)
            {
                table.reserve(0);
            }

            void ClearExtensionVars()
            {
            }

            void LoadExtByName(std::vector<MapEntry> &table, const char *extensionName)
            {
                std::vector<MapEntry>::iterator entry = std::find_if(table.begin(), table.end(), MapCompare(extensionName));

                if(entry != table.end())
                {
                    if(entry->loaderFunc)
                        (*entry->extVariable) = exts::LoadTest(true, entry->loaderFunc());
                    else
                        (*entry->extVariable) = exts::LoadTest(true, 0);
                }
            }
        } //namespace

        namespace 
        {
            static void ProcExtsFromExtString(const char *strExtList, std::vector<MapEntry> &table)
            {
                size_t iExtListLen = strlen(strExtList);
                const char *strExtListEnd = strExtList + iExtListLen;
                const char *strCurrPos = strExtList;
                char strWorkBuff[256];

                while(*strCurrPos)
                {
                    /*Get the extension at our position.*/
                    int iStrLen = 0;
                    const char *strEndStr = strchr(strCurrPos, ' ');
                    int iStop = 0;
                    if(strEndStr == NULL)
                    {
                        strEndStr = strExtListEnd;
                        iStop = 1;
                    }

                    iStrLen = (int)((ptrdiff_t)strEndStr - (ptrdiff_t)strCurrPos);

                    if(iStrLen > 255)
                        return;

                    strncpy(strWorkBuff, strCurrPos, iStrLen);
                    strWorkBuff[iStrLen] = '\0';

                    LoadExtByName(table, strWorkBuff);

                    strCurrPos = strEndStr + 1;
                    if(iStop)
                        break;
                }
            }
        } //namespace 

        exts::LoadTest LoadFunctions()
        {
            ClearExtensionVars();
            std::vector<MapEntry> table;
            InitializeMappingTable(table);

#ifndef IOS
            GetString = reinterpret_cast<PFNGETSTRING>(IntGetProcAddress("glGetString"));
            if(!GetString)
                return exts::LoadTest();
#else
            GetString = glGetString;
#endif
            ProcExtsFromExtString((const char *)gl::GetString(gl::EXTENSIONS), table);

            int numFailed = LoadCoreFunctions();
            return exts::LoadTest(true, numFailed);
        }

        static void ParseVersionFromString(int *pOutMajor, int *pOutMinor, const char *strVersion)
        {
#if !defined(IOS)
            const char *strDotPos = NULL;
            int iLength = 0;
            char strWorkBuff[10];
            *pOutMinor = 0;
            *pOutMajor = 0;
        
            strDotPos = strchr(strVersion, '.');
            if(!strDotPos)
                return;
        
            iLength = (int)((ptrdiff_t)strDotPos - (ptrdiff_t)strVersion);
            strncpy(strWorkBuff, strVersion, iLength);
            strWorkBuff[iLength] = '\0';
        
            *pOutMajor = atoi(strWorkBuff);
            strDotPos = strchr(strVersion + iLength + 1, ' ');
            if(!strDotPos)
            {
                /*No extra data. Take the whole rest of the string.*/
                strcpy(strWorkBuff, strVersion + iLength + 1);
            }
            else
            {
                /*Copy only up until the space.*/
                int iLengthMinor = (int)((ptrdiff_t)strDotPos - (ptrdiff_t)strVersion);
                iLengthMinor = iLengthMinor - (iLength + 1);
                strncpy(strWorkBuff, strVersion + iLength + 1, iLengthMinor);
                strWorkBuff[iLengthMinor] = '\0';
            }
        
            *pOutMinor = atoi(strWorkBuff);
#else
            *pOutMajor = 2;
            *pOutMinor = 0;
#endif
        }

        static int g_major_version = 0;
        static int g_minor_version = 0;

        static void GetGLVersion()
        {
            ParseVersionFromString(&g_major_version, &g_minor_version, reinterpret_cast<const char*>(GetString(VERSION)));
        }

        int GetMajorVersion()
        {
            if(g_major_version == 0)
                GetGLVersion();
            return g_major_version;
        }

        int GetMinorVersion()
        {
            if(g_major_version == 0) //Yes, check the major version to get the minor one.
                GetGLVersion();
            return g_minor_version;
        }

        bool IsVersionGEQ(int majorVersion, int minorVersion)
        {
            if(g_major_version == 0)
                GetGLVersion();

            if(majorVersion > g_major_version)
                return true;
            if(majorVersion < g_major_version)
                return false;
            if(minorVersion >= g_minor_version)
                return true;
            return false;
        }

    } //namespace sys
} //namespace gl
