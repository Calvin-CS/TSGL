/*
 * keynums.h
 *
 *  Created on: Jul 3, 2014
 *      Author: mbv26
 */

#ifndef KEYNUMS_H_
#define KEYNUMS_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*! \brief Enum for keyboard and mouse keys
 \details Key is an enum for the various keyboard and mouse keys.
 \note These are directly mapped to GLFW's integer definitions of the keys.
 */
enum Key {
// Printable keys
    PG_SPACE = GLFW_KEY_SPACE,
    PG_APOSTROPHE = GLFW_KEY_APOSTROPHE, /*! ' */
    PG_COMMA = GLFW_KEY_COMMA, /*! , */
    PG_MINUS = GLFW_KEY_MINUS, /*! - */
    PG_PERIOD = GLFW_KEY_PERIOD, /*! . */
    PG_SLASH = GLFW_KEY_SLASH, /*! / */
    PG_0 = GLFW_KEY_0,
    PG_1 = GLFW_KEY_1,
    PG_2 = GLFW_KEY_2,
    PG_3 = GLFW_KEY_3,
    PG_4 = GLFW_KEY_4,
    PG_5 = GLFW_KEY_5,
    PG_6 = GLFW_KEY_6,
    PG_7 = GLFW_KEY_7,
    PG_8 = GLFW_KEY_8,
    PG_9 = GLFW_KEY_9,
    PG_SEMICOLON = GLFW_KEY_SEMICOLON, /*! ; */
    PG_EQUAL = GLFW_KEY_EQUAL, /*! = */
    PG_A = GLFW_KEY_A,
    PG_B = GLFW_KEY_B,
    PG_C = GLFW_KEY_C,
    PG_D = GLFW_KEY_D,
    PG_E = GLFW_KEY_E,
    PG_F = GLFW_KEY_F,
    PG_G = GLFW_KEY_G,
    PG_H = GLFW_KEY_H,
    PG_I = GLFW_KEY_I,
    PG_J = GLFW_KEY_J,
    PG_K = GLFW_KEY_K,
    PG_L = GLFW_KEY_L,
    PG_M = GLFW_KEY_M,
    PG_N = GLFW_KEY_N,
    PG_O = GLFW_KEY_O,
    PG_P = GLFW_KEY_P,
    PG_Q = GLFW_KEY_Q,
    PG_R = GLFW_KEY_R,
    PG_S = GLFW_KEY_S,
    PG_T = GLFW_KEY_T,
    PG_U = GLFW_KEY_U,
    PG_V = GLFW_KEY_V,
    PG_W = GLFW_KEY_W,
    PG_X = GLFW_KEY_X,
    PG_Y = GLFW_KEY_Y,
    PG_Z = GLFW_KEY_Z,
    PG_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET, /*! [ */
    PG_BACKSLASH = GLFW_KEY_BACKSLASH, /*! \ */
    PG_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET, /*! ] */
    PG_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT, /*! ` */
    PG_WORLD_1 = GLFW_KEY_WORLD_1, /*! non-US #1 */
    PG_WORLD_2 = GLFW_KEY_WORLD_2, /*! non-US #2 */

// Function keys
// PG_KEY_ESCAPE is bound to close window
    PG_KEY_ESCAPE = GLFW_KEY_ESCAPE,
    PG_ENTER = GLFW_KEY_ENTER,
    PG_TAB = GLFW_KEY_TAB,
    PG_BACKSPACE = GLFW_KEY_BACKSPACE,
    PG_INSERT = GLFW_KEY_INSERT,
    PG_DELETE = GLFW_KEY_DELETE,
    PG_RIGHT = GLFW_KEY_RIGHT,
    PG_LEFT = GLFW_KEY_LEFT,
    PG_DOWN = GLFW_KEY_DOWN,
    PG_UP = GLFW_KEY_UP,
    PG_PAGE_UP = GLFW_KEY_PAGE_UP,
    PG_PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
    PG_HOME = GLFW_KEY_HOME,
    PG_END = GLFW_KEY_END,
    PG_CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
    PG_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
    PG_NUM_LOCK = GLFW_KEY_NUM_LOCK,
    PG_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
    PG_PAUSE = GLFW_KEY_PAUSE,
    PG_F1 = GLFW_KEY_F1,
    PG_F2 = GLFW_KEY_F2,
    PG_F3 = GLFW_KEY_F3,
    PG_F4 = GLFW_KEY_F4,
    PG_F5 = GLFW_KEY_F5,
    PG_F6 = GLFW_KEY_F6,
    PG_F7 = GLFW_KEY_F7,
    PG_F8 = GLFW_KEY_F8,
    PG_F9 = GLFW_KEY_F9,
    PG_F10 = GLFW_KEY_F10,
    PG_F11 = GLFW_KEY_F11,
    PG_F12 = GLFW_KEY_F12,
    PG_F13 = GLFW_KEY_F13,
    PG_F14 = GLFW_KEY_F14,
    PG_F15 = GLFW_KEY_F15,
    PG_F16 = GLFW_KEY_F16,
    PG_F17 = GLFW_KEY_F17,
    PG_F18 = GLFW_KEY_F18,
    PG_F19 = GLFW_KEY_F19,
    PG_F20 = GLFW_KEY_F20,
    PG_F21 = GLFW_KEY_F21,
    PG_F22 = GLFW_KEY_F22,
    PG_F23 = GLFW_KEY_F23,
    PG_F24 = GLFW_KEY_F24,
    PG_F25 = GLFW_KEY_F25,
    PG_KP_0 = GLFW_KEY_KP_0,
    PG_KP_1 = GLFW_KEY_KP_1,
    PG_KP_2 = GLFW_KEY_KP_2,
    PG_KP_3 = GLFW_KEY_KP_3,
    PG_KP_4 = GLFW_KEY_KP_4,
    PG_KP_5 = GLFW_KEY_KP_5,
    PG_KP_6 = GLFW_KEY_KP_6,
    PG_KP_7 = GLFW_KEY_KP_7,
    PG_KP_8 = GLFW_KEY_KP_8,
    PG_KP_9 = GLFW_KEY_KP_9,
    PG_KP_DECIMAL = GLFW_KEY_KP_DECIMAL,
    PG_KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
    PG_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
    PG_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
    PG_KP_ADD = GLFW_KEY_KP_ADD,
    PG_KP_ENTER = GLFW_KEY_KP_ENTER,
    PG_KP_EQUAL = GLFW_KEY_KP_EQUAL,
    PG_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
    PG_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
    PG_LEFT_ALT = GLFW_KEY_LEFT_ALT,
    PG_LEFT_SUPER = GLFW_KEY_LEFT_SUPER,
    PG_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
    PG_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
    PG_RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
    PG_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,
    PG_MENU = GLFW_KEY_MENU,

// Mouse buttons
    PG_MOUSE_LEFT = GLFW_MOUSE_BUTTON_LEFT,
    PG_MOUSE_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
    PG_MOUSE_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
};

/*! \brief Enum for keyboard key / mouse button states.
 \details Action is an enum for whether a keyboard / mouse button is pressed or released.
 \note These are directly mapped to GLFW's integer definitions GLFW_PRESS and GLFW_RELEASE.
 */
enum Action {
    PG_PRESS = GLFW_PRESS,
    PG_RELEASE = GLFW_RELEASE
};

#endif /* KEYNUMS_H_ */
