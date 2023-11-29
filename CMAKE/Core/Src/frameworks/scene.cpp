#include "scene.h"

#include <cstdio>

main_menu::main_menu(string name, pii pos, pii shape) : dpo(name, pos, shape) {
    this->add_son(&this->btn);
    this->add_son(&this->test);
    test.backgroud = WHITE;
    test.font_color = BLACK;
}
int click_cnt;
void main_menu::update(display_object *father, pii axis) {
    if (this->btn.isClicked()) {
        this->test.need_render = true;
        char *str = (char *)malloc(255);
        sprintf(str, "Dude, You Just Clicked %d", btn.click_cnt);
        this->test.update_str(str, 16, BLACK, WHITE);
    }

    dpo::update(father, axis);
}