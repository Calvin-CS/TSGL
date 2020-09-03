/*
 * Keynums.h provides an enumeration that maps keys and the mouse buttons to GLFW's integer definitions of the keys.
 */

#ifndef KEYNUMS_H_
#define KEYNUMS_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace tsgl {

/*! \brief Enum for keyboard and mouse keys.
 *  \details Key is an enum for the various keyboard and mouse keys.
 *  \note These are directly mapped to GLFW's integer definitions of the keys.
 */
enum Key {
// Printable keys
    TSGL_SPACE          = GLFW_KEY_SPACE,
    TSGL_APOSTROPHE     = GLFW_KEY_APOSTROPHE,  /*! ' */
    TSGL_COMMA          = GLFW_KEY_COMMA,       /*! , */
    TSGL_MINUS          = GLFW_KEY_MINUS,       /*! - */
    TSGL_PERIOD         = GLFW_KEY_PERIOD,      /*! . */
    TSGL_SLASH          = GLFW_KEY_SLASH,       /*! / */
    TSGL_0              = GLFW_KEY_0,
    TSGL_1              = GLFW_KEY_1,
    TSGL_2              = GLFW_KEY_2,
    TSGL_3              = GLFW_KEY_3,
    TSGL_4              = GLFW_KEY_4,
    TSGL_5              = GLFW_KEY_5,
    TSGL_6              = GLFW_KEY_6,
    TSGL_7              = GLFW_KEY_7,
    TSGL_8              = GLFW_KEY_8,
    TSGL_9              = GLFW_KEY_9,
    TSGL_SEMICOLON      = GLFW_KEY_SEMICOLON,   /*! ; */
    TSGL_EQUAL          = GLFW_KEY_EQUAL,       /*! = */
    TSGL_A              = GLFW_KEY_A,
    TSGL_B              = GLFW_KEY_B,
    TSGL_C              = GLFW_KEY_C,
    TSGL_D              = GLFW_KEY_D,
    TSGL_E              = GLFW_KEY_E,
    TSGL_F              = GLFW_KEY_F,
    TSGL_G              = GLFW_KEY_G,
    TSGL_H              = GLFW_KEY_H,
    TSGL_I              = GLFW_KEY_I,
    TSGL_J              = GLFW_KEY_J,
    TSGL_K              = GLFW_KEY_K,
    TSGL_L              = GLFW_KEY_L,
    TSGL_M              = GLFW_KEY_M,
    TSGL_N              = GLFW_KEY_N,
    TSGL_O              = GLFW_KEY_O,
    TSGL_P              = GLFW_KEY_P,
    TSGL_Q              = GLFW_KEY_Q,
    TSGL_R              = GLFW_KEY_R,
    TSGL_S              = GLFW_KEY_S,
    TSGL_T              = GLFW_KEY_T,
    TSGL_U              = GLFW_KEY_U,
    TSGL_V              = GLFW_KEY_V,
    TSGL_W              = GLFW_KEY_W,
    TSGL_X              = GLFW_KEY_X,
    TSGL_Y              = GLFW_KEY_Y,
    TSGL_Z              = GLFW_KEY_Z,
    TSGL_LEFT_BRACKET   = GLFW_KEY_LEFT_BRACKET,    /*! [ */
    TSGL_BACKSLASH      = GLFW_KEY_BACKSLASH,       /*! \ */
    TSGL_RIGHT_BRACKET  = GLFW_KEY_RIGHT_BRACKET,   /*! ] */
    TSGL_GRAVE_ACCENT   = GLFW_KEY_GRAVE_ACCENT,    /*! ` */
    TSGL_WORLD_1        = GLFW_KEY_WORLD_1,         /*! non-US #1 */
    TSGL_WORLD_2        = GLFW_KEY_WORLD_2,         /*! non-US #2 */

// Function keys
// TSGL_KEY_ESCAPE is bound to close window
    TSGL_KEY_ESCAPE     = GLFW_KEY_ESCAPE,
    TSGL_ENTER          = GLFW_KEY_ENTER,
    TSGL_TAB            = GLFW_KEY_TAB,
    TSGL_BACKSPACE      = GLFW_KEY_BACKSPACE,
    TSGL_INSERT         = GLFW_KEY_INSERT,
    TSGL_DELETE         = GLFW_KEY_DELETE,
    TSGL_RIGHT          = GLFW_KEY_RIGHT,
    TSGL_LEFT           = GLFW_KEY_LEFT,
    TSGL_DOWN           = GLFW_KEY_DOWN,
    TSGL_UP             = GLFW_KEY_UP,
    TSGL_PAGE_UP        = GLFW_KEY_PAGE_UP,
    TSGL_PAGE_DOWN      = GLFW_KEY_PAGE_DOWN,
    TSGL_HOME           = GLFW_KEY_HOME,
    TSGL_END            = GLFW_KEY_END,
    TSGL_CAPS_LOCK      = GLFW_KEY_CAPS_LOCK,
    TSGL_SCROLL_LOCK    = GLFW_KEY_SCROLL_LOCK,
    TSGL_NUM_LOCK       = GLFW_KEY_NUM_LOCK,
    TSGL_PRINT_SCREEN   = GLFW_KEY_PRINT_SCREEN,
    TSGL_PAUSE          = GLFW_KEY_PAUSE,
    TSGL_F1             = GLFW_KEY_F1,
    TSGL_F2             = GLFW_KEY_F2,
    TSGL_F3             = GLFW_KEY_F3,
    TSGL_F4             = GLFW_KEY_F4,
    TSGL_F5             = GLFW_KEY_F5,
    TSGL_F6             = GLFW_KEY_F6,
    TSGL_F7             = GLFW_KEY_F7,
    TSGL_F8             = GLFW_KEY_F8,
    TSGL_F9             = GLFW_KEY_F9,
    TSGL_F10            = GLFW_KEY_F10,
    TSGL_F11            = GLFW_KEY_F11,
    TSGL_F12            = GLFW_KEY_F12,
    TSGL_F13            = GLFW_KEY_F13,
    TSGL_F14            = GLFW_KEY_F14,
    TSGL_F15            = GLFW_KEY_F15,
    TSGL_F16            = GLFW_KEY_F16,
    TSGL_F17            = GLFW_KEY_F17,
    TSGL_F18            = GLFW_KEY_F18,
    TSGL_F19            = GLFW_KEY_F19,
    TSGL_F20            = GLFW_KEY_F20,
    TSGL_F21            = GLFW_KEY_F21,
    TSGL_F22            = GLFW_KEY_F22,
    TSGL_F23            = GLFW_KEY_F23,
    TSGL_F24            = GLFW_KEY_F24,
    TSGL_F25            = GLFW_KEY_F25,
    TSGL_KP_0           = GLFW_KEY_KP_0,
    TSGL_KP_1           = GLFW_KEY_KP_1,
    TSGL_KP_2           = GLFW_KEY_KP_2,
    TSGL_KP_3           = GLFW_KEY_KP_3,
    TSGL_KP_4           = GLFW_KEY_KP_4,
    TSGL_KP_5           = GLFW_KEY_KP_5,
    TSGL_KP_6           = GLFW_KEY_KP_6,
    TSGL_KP_7           = GLFW_KEY_KP_7,
    TSGL_KP_8           = GLFW_KEY_KP_8,
    TSGL_KP_9           = GLFW_KEY_KP_9,
    TSGL_KP_DECIMAL     = GLFW_KEY_KP_DECIMAL,
    TSGL_KP_DIVIDE      = GLFW_KEY_KP_DIVIDE,
    TSGL_KP_MULTIPLY    = GLFW_KEY_KP_MULTIPLY,
    TSGL_KP_SUBTRACT    = GLFW_KEY_KP_SUBTRACT,
    TSGL_KP_ADD         = GLFW_KEY_KP_ADD,
    TSGL_KP_ENTER       = GLFW_KEY_KP_ENTER,
    TSGL_KP_EQUAL       = GLFW_KEY_KP_EQUAL,
    TSGL_LEFT_SHIFT     = GLFW_KEY_LEFT_SHIFT,
    TSGL_LEFT_CONTROL   = GLFW_KEY_LEFT_CONTROL,
    TSGL_LEFT_ALT       = GLFW_KEY_LEFT_ALT,
    TSGL_LEFT_SUPER     = GLFW_KEY_LEFT_SUPER,
    TSGL_RIGHT_SHIFT    = GLFW_KEY_RIGHT_SHIFT,
    TSGL_RIGHT_CONTROL  = GLFW_KEY_RIGHT_CONTROL,
    TSGL_RIGHT_ALT      = GLFW_KEY_RIGHT_ALT,
    TSGL_RIGHT_SUPER    = GLFW_KEY_RIGHT_SUPER,
    TSGL_MENU           = GLFW_KEY_MENU,

// Mouse buttons
    TSGL_MOUSE_LEFT     = GLFW_MOUSE_BUTTON_LEFT,
    TSGL_MOUSE_RIGHT    = GLFW_MOUSE_BUTTON_RIGHT,
    TSGL_MOUSE_MIDDLE   = GLFW_MOUSE_BUTTON_MIDDLE
};

/*! \brief Enum for keyboard key / mouse button states.
 *  \details Action is an enum for whether a keyboard / mouse button is pressed, released, or held until it repeats.
 *  \note These are directly mapped to GLFW's integer definitions GLFW_PRESS, GLFW_RELEASE, and GLFW_REPEAT.
 */
enum Action {
    TSGL_PRESS          = GLFW_PRESS,
    TSGL_RELEASE        = GLFW_RELEASE,
    TSGL_REPEAT         = GLFW_REPEAT
};

}

#endif /* KEYNUMS_H_ */
