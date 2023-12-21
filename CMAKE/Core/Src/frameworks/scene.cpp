#include "scene.h"
#include "main.h"

#include <cstdint>
#include <cstdio>

// extern unsigned char IMAGE_back[9448];
extern uint8_t EVENT[10];
main_menu::main_menu(string name, pii pos, pii shape) : dpo(name, pos, shape) {}
void main_menu::update(display_object *father, pii axis) {

    dpo::update(father, axis);
}

bar::bar(string name, pii pos, pii shape) : dpo(name, pos, shape) {
    // back.img = (unsigned short*)IMAGE_back;
    this->type = BAR;
    this->back.font_size = 24;
    this->home.font_size = 24;
    this->add_son(&this->back);
    this->add_son(&this->home);
    back.backgroud= GRAY;
    home.backgroud= GRAY;
}

void bar::update(display_object *father, pii axis) {
    EVENT[RETURN_BACK] = this->back.isClicked();
    EVENT[RETURN_HOME] = this->home.isClicked();
    dpo::update(father, axis);
}