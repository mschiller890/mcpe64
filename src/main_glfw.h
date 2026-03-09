#ifndef MAIN_GLFW_H__
#define MAIN_GLFW_H__

#include "App.h"
#include "GLFW/glfw3.h"
#include "client/renderer/gles.h"
#include "SharedConstants.h"

#include <cstdio>
#include "platform/input/Keyboard.h"
#include "platform/input/Mouse.h"
#include "platform/input/Multitouch.h"
#include "util/Mth.h"
#include "AppPlatform_glfw.h"

static App* g_app = 0;

int transformKey(int glfwkey) {
	if (glfwkey >= GLFW_KEY_F1 && glfwkey <= GLFW_KEY_F12) {
		return glfwkey - 178;
	}

	switch (glfwkey) {
		case GLFW_KEY_ESCAPE: return Keyboard::KEY_ESCAPE;
		case GLFW_KEY_BACKSPACE: return Keyboard::KEY_BACKSPACE;
		case GLFW_KEY_LEFT_SHIFT: return Keyboard::KEY_LSHIFT;
		case GLFW_KEY_ENTER: return Keyboard::KEY_RETURN;
		default: return glfwkey;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(action == GLFW_REPEAT) return;

	Keyboard::feed(transformKey(key), action);
}

void character_callback(GLFWwindow* window, unsigned int codepoint) {
	Keyboard::feedText(codepoint);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	static double lastX = 0.0, lastY = 0.0;
	static bool firstMouse = true;

	if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

	double deltaX = xpos - lastX;
    double deltaY = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		Mouse::feed(0, 0, xpos, ypos, deltaX, deltaY);
	} else { 
		Mouse::feed( MouseAction::ACTION_MOVE, 0, xpos, ypos);
	}
	Multitouch::feed(0, 0, xpos, ypos, 0);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if(action == GLFW_REPEAT) return;

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		Mouse::feed( MouseAction::ACTION_LEFT, action, xpos, ypos);
		Multitouch::feed(1, action, xpos, ypos, 0);
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		Mouse::feed( MouseAction::ACTION_RIGHT, action, xpos, ypos);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	Mouse::feed(3, 0, xpos, ypos, 0, yoffset);
}

void window_size_callback(GLFWwindow* window, int width, int height) {
	if (g_app) g_app->setSize(width, height);
}

void error_callback(int error, const char* desc) {
	printf("Error: %s\n", desc);
}

int main(void) {
	AppContext appContext;

#ifndef STANDALONE_SERVER
	// Platform init.
	appContext.platform = new AppPlatform_glfw();

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	GLFWwindow* window = glfwCreateWindow(appContext.platform->getScreenWidth(), appContext.platform->getScreenHeight(), "main", NULL, NULL);
	
	if (window == NULL) {
		return 1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, character_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

	glfwMakeContextCurrent(window);
	gladLoadGLES1Loader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);
#endif

	App* app = new MAIN_CLASS();

	g_app = app;
	((MAIN_CLASS*)g_app)->externalStoragePath = ".";
	((MAIN_CLASS*)g_app)->externalCacheStoragePath = ".";
	g_app->init(appContext);
	g_app->setSize(appContext.platform->getScreenWidth(), appContext.platform->getScreenHeight());

	// Main event loop
	while(!glfwWindowShouldClose(window) && !app->wantToQuit()) {
		app->update();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete app;

	appContext.platform->finish();
	
	delete appContext.platform;
	
#ifndef STANDALONE_SERVER
	// Exit.
	glfwDestroyWindow(window);
	glfwTerminate();
#endif

	return 0;
}

#endif /*MAIN_GLFW_H__*/
