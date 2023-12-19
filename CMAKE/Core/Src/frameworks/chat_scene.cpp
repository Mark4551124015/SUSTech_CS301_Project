#include "chat_scene.hpp"
#include "emoji.hpp"
#include <string.h>
#include "cstring"
const unsigned char* emoji_arr[4] = {gImage_emoji1, gImage_emoji2, gImage_emoji3, gImage_emoji4};
extern uint8_t EVENT[32];
extern int emoji_number;
page :: page(string name, pii pos, pii shape) : dpo(name, pos, shape)
{
    this->cnt = 0;
    for(int i=0; i<6; i++)
    {
        this->add_son(&this->messages[i]);
        this->emojis[i].isVisible = false;
        this->add_son(&this->emojis[i]);
    }
    
}

bool page :: addMessage(char * str, char * userName){
    if(this->cnt >= 6) return false;
    int str_len = strlen(str);
    int name_len = strlen(userName);
    for(int i=str_len - 1; i >= 0; i--)
        str[i + name_len + 2] = str[i];
    for(int i=0; i<name_len; i++)
        str[i] = userName[i];
    str[name_len] = ':';
    str[name_len + 1] = ' ';
    str[str_len + name_len + 2] = '\0';
    this->messages[this->cnt++].update_str(str, 16, BLACK, WHITE);
    return true;
}

bool page :: addImage(int num, char * userName){
    if(this->cnt >= 6) return false;
    this->emojis[this->cnt].update_img((unsigned short *)emoji_arr[num-1]);
    this->emojis[this->cnt].setVisbility(true);
    printf("set emoji %d\n", num);//arrived
    printf("%d", this->emojis[this->cnt].isVisible);
    this->messages[this->cnt].update_str(userName, 16, BLACK, WHITE);
    this->cnt++;
    return true;
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

    for (int i = 0; i < 6; ++i) {
        char c = '0' + i;
        string name = "page_";
        name += c;
        this->pages[i] = new page(name, {0, -20}, {240, 180});
        this->add_son(this->pages[i]);
        this->pages[i]->setVisbility(false);
    }
    this->pages[0]->setVisbility(true);
    this->add_son(&this->userInfo);
    this->add_son(pre);
    this->add_son(nxt);
    this->add_son(emoji);
}


//void chat_scene_main :: updateMessage(char * RX_DATA){
//    message.update_str(RX_DATA, 16, BLACK, WHITE);
//}

void chat_scene_main :: addMessageToPage(char * message, int user_num){
    if(!this->pages[page_cnt]->addMessage(message, this->users[user_num])){
        if(page_cnt >= 6) return;
        this->pages[page_cnt]->setVisbility(false);
        now_page = ++page_cnt;
        this->pages[page_cnt]->setVisbility(true);
        this->pages[page_cnt]->addMessage(message, this->users[user_num]);
    }
}

void chat_scene_main :: addImageToPage(int num, int user_num){
    if(!this->pages[page_cnt]->addImage(num, this->users[user_num])){
        if(page_cnt >= 6) return;
        this->pages[page_cnt]->setVisbility(false);
        now_page = ++page_cnt;
        this->pages[page_cnt]->setVisbility(true);
        this->pages[page_cnt]->addImage(num, this->users[user_num]);
    }
}

void chat_scene_main :: update(display_object *father, pii axis){
    dpo::update(father, axis);
    if(this->pre->isClicked()){
        if(now_page > 0){
            this->pages[now_page]->setVisbility(false);
            this->pages[--now_page]->setVisbility(true);
        }
    }
    if(this->nxt->isClicked()){
        if(now_page < page_cnt){
            this->pages[now_page]->setVisbility(false);
            this->pages[++now_page]->setVisbility(true);
        }
    }
    if(this->emoji->isClicked()){
        EVENT[EMOJI_SELECT] = 1;
    }


}