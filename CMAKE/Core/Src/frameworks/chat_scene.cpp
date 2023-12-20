#include "chat_scene.hpp"

#include <string.h>
#include <cstdio>

#include "cstring"
#include "emoji.hpp"

const unsigned char* emoji_arr[4] = {gImage_emoji1, gImage_emoji2,gImage_emoji3, gImage_emoji4};
extern uint8_t EVENT[8];
extern int emoji_number;
page ::page(string name, pii pos, pii shape) : dpo(name, pos, shape) {
    this->cnt = 0;
    for (int i = 0; i < 6; i++) {
        this->add_son(&this->messages[i]);
        this->emojis[i].isVisible = false;
        this->add_son(&this->emojis[i]);
    }
}

string page :: addMessage(string str, const char *userName) {
    int str_len = str.length();
    int name_len = strlen(userName);
    for (int i = str_len - 1; i >= 0; i--) str[i + name_len + 2] = str[i];
    for (int i = 0; i < name_len; i++) str[i] = userName[i];
    str[name_len] = ':';
    str[name_len + 1] = ' ';
    str[str_len + name_len + 2] = '\0';
    this->messages[this->cnt].update_str(str, 16, BLACK, WHITE);
    return str;
}

int page ::addImage(int num, const char *userName) {
    if (this->cnt >= 6) return false;
    this->emojis[this->cnt].setVisbility(true);
    this->emojis[this->cnt].update_img((unsigned short *)emoji_arr[num-1]);
    printf("set emoji %d\n", num);  // arrived
    this->messages[this->cnt].update_str(userName, 16, BLACK, WHITE); 
    return num;
}

void page :: clear(){
    cnt = 0;
    for(int i=0; i<6; i++)
    {
        this->messages[i].update_str("", 16, BLACK, WHITE);
        this->emojis[i].update_img(NULL);
    }
}


void page ::update(display_object *father, pii axis) {
    dpo ::update(father, axis);
}

chat_scene_main ::chat_scene_main(string name, pii pos, pii shape,
                                  string users[3])
    : dpo(name, pos, shape) {
    string new_str = userInfo.str;
    for (int i = 0; i < 3; i++) this->users[i] = users[i];
    printf("Addded user \n");
    new_str += users[1];
    if ((users[2] != "")) {
        new_str += ", ";
        new_str += users[2];
    }
    userInfo.update_str(new_str, 16, BLACK, WHITE);
    this->page_cnt = 0;
    for(int i=0; i<5; i++)
        for(int j=0; j<6; j++)
        {
            this->pageMessage[i][j] = "";
            this->pageEmoji[i][j] = 0;
        }

    printf("Done\n");
    this->add_son(&this->userInfo);
    this->add_son(&pre);
    this->add_son(&nxt);
    this->add_son(&emoji);
    this->add_son(&showPage);
}

// void chat_scene_main :: updateMessage(char * RX_DATA){
//     message.update_str(RX_DATA, 16, BLACK, WHITE);
// }

void chat_scene_main :: addMessageToPage(string message, int user_num) {
    if(showPage.cnt < 6)
    {
        pageMessage[page_cnt][showPage.cnt] = showPage.addMessage(message, users[user_num].c_str());
        showPage.cnt++;
    }
    else 
    {
        if(page_cnt >=6) return;
        showPage.clear();
        now_page = ++page_cnt;
        pageMessage[page_cnt][showPage.cnt] = showPage.addMessage(message, users[user_num].c_str());
        showPage.cnt++;
    }
    printf("Message added, showPage.cnt: %d\n", showPage.cnt);
}

void chat_scene_main ::addImageToPage(int num, int user_num) {
    if(showPage.cnt < 6)
    {
        pageEmoji[page_cnt][showPage.cnt] = showPage.addImage(num, users[user_num].c_str());
        showPage.cnt++;
    }
    else 
    {
        if(page_cnt >= 6) return;
        showPage.clear();
        now_page = ++page_cnt;
        pageEmoji[page_cnt][showPage.cnt] = showPage.addImage(num, users[user_num].c_str());
        showPage.cnt++;    
    }
    printf("Emoji added, showPage.cnt: %d\n", showPage.cnt);
}

void chat_scene_main ::update(display_object *father, pii axis) {
    dpo::update(father, axis);
    if (this->pre.isClicked()) {
        if (now_page > 0) {
            now_page--;
            showPage.clear();
            for(int i=0; i<6; i++)
            {
                this->showPage.messages[i].update_str(pageMessage[now_page][i], 16, BLACK, WHITE);
                this->showPage.emojis[i].update_img((unsigned short *)emoji_arr[pageEmoji[now_page][i] - 1]);
            }
        }
    }
    if (this->nxt.isClicked()) {
        if (now_page < page_cnt) {
            now_page++;
            showPage.clear();
            for(int i=0; i<6; i++)
            {
                this->showPage.messages[i].update_str(pageMessage[now_page][i], 16, BLACK, WHITE);
                this->showPage.emojis[i].update_img((unsigned short *)emoji_arr[pageEmoji[now_page][i] - 1]);
            }
        }
    }
    if (this->emoji.isClicked()) {
        EVENT[EMOJI_SELECT] = 1;
    }
}