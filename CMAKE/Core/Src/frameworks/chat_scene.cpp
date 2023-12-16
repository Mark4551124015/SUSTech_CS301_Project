#include "chat_scene.hpp"
#include <string.h>
#include "cstring"
chat_scene_main :: chat_scene_main(string name, pii pos, pii shape, char* users[3]) : dpo(name, pos, shape){
    char* newStr = new char[30];
    for(int i = 0; i < 3; i++)
        this->users[i] = users[i];
    strcpy(newStr, userInfo.str);
    for(int i = 1; i < 3; i++)
    {
        printf("%s\n", newStr);
        strcat(newStr, users[i]);
        if(i == 1)
            strcat(newStr, ", ");
    }
    userInfo.update_str((char *)newStr, 16, BLACK, WHITE);
    this->add_son(&this->userInfo);
    this->add_son(&this->message);
    this->add_son(pre);
    this->add_son(nxt);
    this->add_son(send);
    this->add_son(emoji);
}

void chat_scene_main :: update(display_object *father, pii axis){
    dpo::update(father, axis);
}