#include "chat_select_scene.hpp"

chat_select_main::chat_select_main(string name, pii pos, pii shape) : dpo (name, pos, shape){
    this->invite_key[0] = new button("invite_user_0", {80, -90}, this->invite_key_size, "Chat" );
    this->invite_key[1] = new button("invite_user_1", {80, -50}, this->invite_key_size, "Chat");
    this->group_key = new button("group_chat", {0, 120}, this->group_key_size, "Group Chat");
    for(int i = 0; i < 2; i++)
    {
        this->add_son(invite_key[i]);
        this->add_son(&this->userName[i]);
        this->add_son(&this->status[i]);
    }
    this->add_son(group_key);
    this->add_son(&this->userNameCol);
    this->add_son(&this->statusCol);
    this->add_son(&this->operationCol);


}

void chat_select_main::update(display_object *father, pii axis){
    dpo::update(father, axis);
}