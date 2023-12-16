#include "chat_scene.hpp"
#include <string.h>
#include "cstring"


page :: page(string name, pii pos, pii shape) : dpo(name, pos, shape)
{
    this->cnt = 0;
    for(int i=0; i<4; i++)
        this->add_son(&this->messages[i]);

    
}

bool page :: addMessage(char * str){
    if(this->cnt >= 4) return false;
    this->messages[this->cnt++].update_str(str, 16, BLACK, WHITE);
    this->need_render = true;
    return true;
}

void page :: clear(){
    for(int i=0; i<4; i++)
        this->messages[i].clear();
}

void page :: update(display_object *father, pii axis){
    dpo :: update(father, axis);
}

chat_scene_main :: chat_scene_main(string name, pii pos, pii shape, char* users[3]) : dpo(name, pos, shape){
    char* newStr = new char[30];
    for(int i = 0; i < 3; i++)
        this->users[i] = users[i];
    strcpy(newStr, userInfo.str);
    strcat(newStr, users[1]);
    if(strcmp(users[2], ""))
        {
            strcat(newStr, ", ");
            strcat(newStr, users[2]);
        }
    userInfo.update_str((char *)newStr, 16, BLACK, WHITE);
    this->page_cnt = 0;
    this->pages[page_cnt] = new page("page0", {0, -45}, {240, 140});
    this->add_son(pages[page_cnt]);
    this->add_son(&this->userInfo);
    this->add_son(&this->message);
    this->add_son(pre);
    this->add_son(nxt);
    this->add_son(send);
    this->add_son(emoji);
}


void chat_scene_main :: updateMessage(char * RX_DATA){
    message.update_str(RX_DATA, 16, BLACK, WHITE);
}

void chat_scene_main :: addMessageToPage(char * message){
    if(!pages[page_cnt]->addMessage(message)){
        if(page_cnt >= 4) return;
        pages[page_cnt]->clear();
        char * pageName;
        sprintf(pageName, "page%d", page_cnt);
        pages[++page_cnt] = new page(pageName, {0, -45}, {240, 140});
        this->add_son(pages[page_cnt]);
        pages[page_cnt]->addMessage(message);
    }
}
void chat_scene_main :: update(display_object *father, pii axis){
    dpo::update(father, axis);
}
