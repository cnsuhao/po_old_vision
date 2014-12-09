#ifndef POERROR_H_INCLUDED
#define POERROR_H_INCLUDED
#include <exception>
#include <string>
struct EnvironmentalError:public std::exception
{
    const char* what(void) const throw() { return "Can not Create a Suitable Environment.";  }
};

struct CreatingWindowFail:public EnvironmentalError
{
    const char* what(void) const throw() { return "Can not Create a SDL_Window, Use SDL_GetError() To Get More Information.";}
};

struct CreatingGLContextFail:public EnvironmentalError
{
    const char* what(void) const throw() { return "Can not Create a SDL_GLContext, Use SDL_GetError() to Get More Information.";}
};

struct ResourceUnavailable:public std::exception
{
    std::string Source;
    const char* what(void) const throw() { return "The Resource you Use is Unavailable.";}
    ResourceUnavailable(const std::string& S):Source(S){}
};

struct PictureUnavailable:public ResourceUnavailable
{
    PictureUnavailable(const std::string& L):ResourceUnavailable(L){}
};

struct FontUnavailable:public ResourceUnavailable
{
    int Size;
    FontUnavailable(const std::string& L,int S):ResourceUnavailable(L),Size(S){}
};

struct TextUnavailable:public std::exception
{
    const char* what(void) const throw() {return "Unable to Create Text.";}
    //TextUnavailable(const std::wstring& L):ResourceUnavailable(L){}
};

struct MusicUnavailable:public std::exception
{
    const char* what(void) const throw(){ return "Unable To Create Music";}
};

#endif // POERROR_H_INCLUDED
