#include "main_scene.hpp"
extern uint8_t EVENT[10];
main_scene :: main_scene(string name, pii pos, pii shape) : dpo(name, pos, shape)
{
    this->add_son(&time);
    this->add_son(&userName);
    this->add_son(&chat_icon);
    this->add_son(&calc_icon);
    this->add_son(&photo_icon);
}

void main_scene :: update(display_object *father, pii axis)
{
    if(chat_icon.isClicked())
    {
        EVENT[CHAT_SELECTED] = 1;
    }
    if(calc_icon.isClicked())
    {
        EVENT[CALC_SELECTED] = 1;
    }
    if(photo_icon.isClicked())
    {
        EVENT[PHOTO_SELECTED] = 1;
    }
    dpo::update(father, axis);
}