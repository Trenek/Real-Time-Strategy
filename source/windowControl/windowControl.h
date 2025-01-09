#include <GLFW/glfw3.h>

union keyState {
    struct {
        uint8_t press : 1;
        uint8_t change : 1;
        uint8_t : 6;
    };
    uint8_t val;
};

struct windowControl {
    union keyState mouseButton[GLFW_MOUSE_BUTTON_LAST + 1];
    union keyState key[GLFW_KEY_LAST + 1];
};

enum keyStateNum {
    KEY_RELEASE = 1 << 0,
    KEY_PRESS =   1 << 1,
    KEY_REPEAT =  1 << 2,
    KEY_CHANGE =  1 << 3
};

inline uint8_t getKeyState(struct windowControl *windowControl, int key) {
    uint8_t result = (windowControl->key[key].press ? KEY_PRESS : KEY_RELEASE) |
                     (windowControl->key[key].change ? KEY_CHANGE : KEY_REPEAT);
    windowControl->key[key].change = 0;

    return result;
}

inline uint8_t getMouseState(struct windowControl *windowControl, int key) {
    uint8_t result = (windowControl->mouseButton[key].press ? KEY_PRESS : KEY_RELEASE) |
           (windowControl->mouseButton[key].change ? KEY_CHANGE : KEY_REPEAT);

    windowControl->mouseButton[key].change = 0;

    return result;
}
