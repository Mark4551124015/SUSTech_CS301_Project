#include "chat_select_scene.hpp"
extern uint8_t EVENT[10];
extern string users[3];
extern int selected_chat;
chat_select_main::chat_select_main(string name, pii pos, pii shape) : dpo (name, pos, shape){
    for(int i = 0; i < 2; i++)
    {
        this->add_son(&invite_key[i]);
        this->add_son(&this->userName[i]);
        this->add_son(&this->status[i]);
    }
    this->add_son(&group_key);
    this->add_son(&this->userNameCol);
    this->add_son(&this->statusCol);
    this->add_son(&this->operationCol);


}

void chat_select_main::update(display_object *father, pii axis){
    if(invite_key[0].isClicked())
    {
        EVENT[CREAT_CHAT] = 1;
        selected_chat = 0;
        users[0] = "User0";
        users[1] = "User1";
        users[2] = "";
    }

    if(invite_key[1].isClicked())
    {
        EVENT[CREAT_CHAT] = 1;
        selected_chat = 1;
        users[0] = "User0";
        users[1] = "User2";
        users[2] = "";
    }
    if(group_key.isClicked())
    {
        EVENT[CREAT_CHAT] = 1;
        selected_chat = 2;
        users[0] = "User0";
        users[1] = "User1";
        users[2] = "User2";
    }

    dpo::update(father, axis);
}