#include "chat_scene.hpp"

#include <string.h>
#include <cstdio>

#include "cstring"
#include "emoji.hpp"

// const unsigned char* emoji_arr[4] = {gImage_emoji1, gImage_emoji2,
// gImage_emoji3, gImage_emoji4};
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

bool page ::addMessage(char *str, const char *userName) {
    if (this->cnt >= 6) return false;
    int str_len = strlen(str);
    int name_len = strlen(userName);
    for (int i = str_len - 1; i >= 0; i--) str[i + name_len + 2] = str[i];
    for (int i = 0; i < name_len; i++) str[i] = userName[i];
    str[name_len] = ':';
    str[name_len + 1] = ' ';
    str[str_len + name_len + 2] = '\0';
    this->messages[this->cnt++].update_str(str, 16, BLACK, WHITE);
    return true;
}

bool page ::addImage(int num, const char *userName) {
    if (this->cnt >= 6) return false;
    // this->emojis[this->cnt].update_img((unsigned short *)emoji_arr[num-1]);
    this->emojis[this->cnt].setVisbility(true);
    printf("set emoji %d\n", num);  // arrived
    printf("%d", this->emojis[this->cnt].isVisible);
    this->messages[this->cnt].update_str(userName, 16, BLACK, WHITE);
    this->cnt++;
    return true;
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
    // strcpy(newStr, userInfo.str);
    new_str += users[1];
    // strcat(newStr, users[1]);
    if ((users[2] != "")) {
        // strcat(newStr, ", ");
        // strcat(newStr, users[2]);
        new_str += ", ";
        new_str += users[2];
    }
    userInfo.update_str(new_str, 16, BLACK, WHITE);
    this->page_cnt = 0;
    this->pages[0].isVisible = (true);
    for (int i = 1; i < 5; i++) {
        this->pages[i].isVisible = (false);
        printf("setted page %d\n", i);
    }
    printf("Done\n");
    this->add_son(&this->userInfo);
    this->add_son(&pre);
    this->add_son(&nxt);
    this->add_son(&emoji);
}

// void chat_scene_main :: updateMessage(char * RX_DATA){
//     message.update_str(RX_DATA, 16, BLACK, WHITE);
// }

void chat_scene_main ::addMessageToPage(char *message, int user_num) {
    if (!this->pages[page_cnt].addMessage(message, this->users[user_num].c_str())) {
        if (page_cnt >= 6) return;
        this->pages[page_cnt].setVisbility(false);
        now_page = ++page_cnt;
        this->pages[page_cnt].setVisbility(true);
        this->pages[page_cnt].addMessage(message, this->users[user_num].c_str());
    }
}

void chat_scene_main ::addImageToPage(int num, int user_num) {
    if (!this->pages[page_cnt].addImage(num, this->users[user_num].c_str())) {
        if (page_cnt >= 6) return;
        this->pages[page_cnt].setVisbility(false);
        now_page = ++page_cnt;
        this->pages[page_cnt].setVisbility(true);
        this->pages[page_cnt].addImage(num, this->users[user_num].c_str());
    }
}

void chat_scene_main ::update(display_object *father, pii axis) {
    dpo::update(father, axis);
    if (this->pre.isClicked()) {
        if (now_page > 0) {
            this->pages[now_page].setVisbility(false);
            this->pages[--now_page].setVisbility(true);
        }
    }
    if (this->nxt.isClicked()) {
        if (now_page < page_cnt) {
            this->pages[now_page].setVisbility(false);
            this->pages[++now_page].setVisbility(true);
        }
    }
    if (this->emoji.isClicked()) {
        EVENT[EMOJI_SELECT] = 1;
    }
}