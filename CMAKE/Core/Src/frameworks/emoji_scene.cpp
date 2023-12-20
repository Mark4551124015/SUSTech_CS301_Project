#include "emoji_scene.hpp"
// #include <string.h>
// #include "cstring"

extern uint8_t EVENT[8];
emoji_scene_main :: emoji_scene_main(string name, pii pos, pii shape) : dpo(name, pos, shape){
    this->emoji1.img = (unsigned short *)gImage_emoji1;
    this->emoji2.img = (unsigned short *)gImage_emoji2;
    this->emoji3.img = (unsigned short *)gImage_emoji3;
    this->emoji4.img = (unsigned short *)gImage_emoji4;
    this->add_son(&this->emoji1);
    this->add_son(&this->emoji2);
    this->add_son(&this->emoji3);
    this->add_son(&this->emoji4);
}

void emoji_scene_main :: update(display_object *father, pii axis){
    if(this->emoji1.isClicked()){
        this->emoji_num = 1;
        EVENT[EMOJI_SELECTED] = 1;
    }
    if(this->emoji2.isClicked()){
        this->emoji_num = 2;
        EVENT[EMOJI_SELECTED] = 1;
    }
    if(this->emoji3.isClicked()){
        this->emoji_num = 3;
        EVENT[EMOJI_SELECTED] = 1;
    }
    if(this->emoji4.isClicked()){
        this->emoji_num = 4;
        EVENT[EMOJI_SELECTED] = 1;
    }
    dpo::update(father, axis);
}