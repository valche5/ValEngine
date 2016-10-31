/*
 *   Copyright (C) 2008-2013 by Mathias Paulin, David Vanderhaeghe
 *   Mathias.Paulin@irit.fr
 *   vdh@irit.fr
 */


#ifndef GL_ASSERT_H
#define GL_ASSERT_H

/**
        @author Mathias Paulin <Mathias.Paulin@irit.fr>
 *
 * OpenGL error management class.
 *
 */
#ifndef NDEBUG
//#error "NDEBUG NOT DEFINED "

// Breakpoints
// This macro will trigger a breakpoint where it is placed. With MSVC a dialog
// will ask you if you want to launch the debugger.
#define BREAKPOINT(ARG) asm volatile ("int $3")


#include <iostream>
#include <cassert>

#ifdef __APPLE__
#define glAssert(code) \
    code; \
    {\
        GLuint err = glGetError(); \
        if (err != GL_NO_ERROR) { \
            std::cerr<<"erreur OpenGL ("<<__FILE__<<":"<<__LINE__<<", "<<__STRING(code)<<") :"<<\
    ( (err == GL_INVALID_ENUM) ? " Invalid enum : An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n" : \
                    ( (err == GL_INVALID_VALUE) ? " Invalid value : A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n" : \
                    ( (err == GL_INVALID_OPERATION) ? " Invalid operation : The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n" : \
                    ( (err == GL_INVALID_FRAMEBUFFER_OPERATION) ? " Invalid framebuffer operation : The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n" : \
                    ( (err == GL_OUT_OF_MEMORY) ? " Out of memory : There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n" : \
     /*               ( (err == GL_STACK_UNDERFLOW) ? " Stack underflow : An attempt has been made to perform an operation that would cause an internal stack to underflow.\n" : \
                    ( (err == GL_STACK_OVERFLOW) ? " Stack overflow : An attempt has been made to perform an operation that would cause an internal stack to overflow.\n" : */\
                    nullptr) \
    /*                )\
                    )\
    */                )\
                    )\
                    )\
                    )\
                << " ("<<err<<")"<<std::endl; \
    BREAKPOINT(0);\
            /*assert(false);*/ /*int*b=nullptr; *b=0;*/\
        } \
    }

#define glCheckError() \
    {\
        GLuint err = glGetError(); \
        if (err != GL_NO_ERROR) { \
            std::cerr<<"erreur OpenGL ("<<__FILE__<<":"<<__LINE__<<") :"<<\
    ( (err == GL_INVALID_ENUM) ? " Invalid enum : An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n" : \
                    ( (err == GL_INVALID_VALUE) ? " Invalid value : A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n" : \
                    ( (err == GL_INVALID_OPERATION) ? " Invalid operation : The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n" : \
                    ( (err == GL_INVALID_FRAMEBUFFER_OPERATION) ? " Invalid framebuffer operation : The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n" : \
                    ( (err == GL_OUT_OF_MEMORY) ? " Out of memory : There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n" : \
     /*               ( (err == GL_STACK_UNDERFLOW) ? " Stack underflow : An attempt has been made to perform an operation that would cause an internal stack to underflow.\n" : \
                    ( (err == GL_STACK_OVERFLOW) ? " Stack overflow : An attempt has been made to perform an operation that would cause an internal stack to overflow.\n" : */\
                    nullptr) \
    /*                )\
                    )\
    */                )\
                    )\
                    )\
                    )\
    << " ("<<err<<")"<<std::endl; \
    BREAKPOINT(0);\
            /*assert(false);*/ /*int*b=nullptr; *b=0; */\
        } \
    }
#else
#define STR(x) #x

#define glAssert(code) \
    code; \
{\
    GLuint err = glGetError(); \
    if (err != GL_NO_ERROR) { \
    std::cout<<"erreur OpenGL ("<<__FILE__<<":"<<__LINE__<<", "<<STR(code)<<") : "<<(const char*)gluErrorString (err)<<"("<<err<<")"<<std::endl; \
    } \
    }

#define glCheckError() \
    {\
        GLuint err = glGetError(); \
        if (err != GL_NO_ERROR) { \
    std::cout<<"erreur OpenGL ("<<__FILE__<<":"<<__LINE__<<", "<<STR()<<") : "<<(const char*)gluErrorString (err)<<"("<<err<<")"<<std::endl; \
  BREAKPOINT(0);\
          } \
    }
#endif

#else
//#error "NDEBUG  DEFINED "

#define glAssert(code) \
    code;

#define glCheckError()
#endif

// #undef NDEBUG

#endif
