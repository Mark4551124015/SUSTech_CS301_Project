#include "chat_scene.hpp"

chat_scene_main :: chat_scene_main(string name, pii pos, pii shape) : dpo(name, pos, shape){
    for(int i = 0; i < 3; i++)
        this->users[i] = users[i];
    this->add_son(&this->userInfo);
    this->add_son(&this->message);
    this->add_son(pre);
    this->add_son(nxt);
    this->add_son(send);
}

void chat_scene_main :: update(display_object *father, pii axis){
    dpo::update(father, axis);
}