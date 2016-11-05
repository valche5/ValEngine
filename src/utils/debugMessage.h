#ifndef DEBUGMESSAGE_H
#define DEBUGMESSAGE_H

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#define APIENTRY
#endif

#include "../core/openGL.h"

using std::cout;
using std::endl;

void APIENTRY debugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
	// Some debug messages are just annoying informational messages
	switch (id) {
	case 131169: // The driver allocated storage for renderbuffer
	case 131185: // glBufferData
		return;
	}

	cout << "Message: " <<  message << "\n";
	cout << "Source: ";

	switch (source) {
	case GL_DEBUG_SOURCE_API:
		cout << "API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		cout << "Window System";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		cout << "Shader Compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		cout << "Third Party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		cout << "Application";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		cout << "Other";
		break;
	}

	cout << "\n";
	cout << "Type: ";

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		cout << "Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		cout << "Deprecated Behaviour";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << "Undefined Behaviour";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		cout << "Portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		cout << "Performance";
		break;
	case GL_DEBUG_TYPE_MARKER:
		cout << "Marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		cout << "Push Group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		cout << "Pop Group";
		break;
	case GL_DEBUG_TYPE_OTHER:
		cout << "Other";
		break;
	}

	cout << "\n";
	cout << "ID: " << id << "\n";
	cout << "Severity: ";

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		cout << "High";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		cout << "Medium";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		cout << "Low";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		cout << "Notification";
		break;
	}

	cout << endl << endl;
};

#endif // DEBUGMESSAGE_H
