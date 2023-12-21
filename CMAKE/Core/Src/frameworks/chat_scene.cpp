#include "chat_scene.hpp"

#include <string.h>
#include <cstddef>
#include <cstdio>

#include "cstring"
#include "emoji.hpp"

const unsigned char* emoji_arr[4] = {gImage_emoji1, gImage_emoji2,gImage_emoji3, gImage_emoji4};
extern uint8_t EVENT[8];
extern int emoji_number;
extern int user_self;
page ::page(string name, pii pos, pii shape) : dpo(name, pos, shape) {
    this->cnt = 0;
    for (int i = 0; i < 6; i++) {
        this->add_son(&this->messages[i]);
        this->emojis[i].isVisible = false;
        this->add_son(&this->emojis[i]);
    }
}

string page :: addMessage(string str) {
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
    this->add_son(&userInfo);
    this->add_son(&pre);
    this->add_son(&nxt);
    this->add_son(&emoji);
    this->add_son(&showPage);
    this->emoji_sc.isVisible = false;
    this->add_son(&emoji_sc);
}

// void chat_scene_main :: updateMessage(char * RX_DATA){
//     message.update_str(RX_DATA, 16, BLACK, WHITE);
// }

void chat_scene_main :: addMessageToPage(string message, int user_num) {
    string userName_string = users[user_num];
    message = userName_string + ": " + message;
    if(showPage.cnt < 6)
    {
        pageMessage[page_cnt][showPage.cnt] = message;
        if(now_page == page_cnt) showPage.addMessage(message);
        showPage.cnt++;
    }
    else 
    {
        if(page_cnt >=6) return;
        //这里要分开写，因为在切换页面的时候cnt要保留
        showPage.cnt = 0;
        showPage.clear();
        now_page = ++page_cnt;
        pageMessage[page_cnt][showPage.cnt] = message;
        if(now_page == page_cnt) showPage.addMessage(message);
        showPage.cnt++;
    }
    printf("Message added, showPage.cnt: %d\n", showPage.cnt);
}

void chat_scene_main ::addImageToPage(int num, int user_num) {
    if(showPage.cnt < 6)
    {
        pageEmoji[page_cnt][showPage.cnt] = num;
        pageMessage[page_cnt][showPage.cnt] = users[user_num];
        if(now_page == page_cnt) showPage.addImage(num, users[user_num].c_str());
        showPage.cnt++;
    }
    else 
    {
        if(page_cnt >= 6) return;
        showPage.cnt = 0;
        showPage.clear();
        now_page = ++page_cnt;
        pageEmoji[page_cnt][showPage.cnt] = num;
        pageMessage[page_cnt][showPage.cnt] = users[user_num];
        if(now_page == page_cnt) showPage.addImage(num, users[user_num].c_str());
        showPage.cnt++; 
    }
    printf("Emoji added, showPage.cnt: %d\n", showPage.cnt);
}

void chat_scene_main ::update(display_object *father, pii axis) {
    
    if (this->pre.isClicked()) {
        if (now_page > 0) {
            now_page--;
            showPage.clear();
            for(int i=0; i<6; i++)
            {
                this->showPage.messages[i].update_str(pageMessage[now_page][i], 16, BLACK, WHITE);
                this->showPage.emojis[i].update_img(pageEmoji[now_page][i]?(unsigned short *)emoji_arr[pageEmoji[now_page][i] - 1]:NULL);
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
                this->showPage.emojis[i].update_img(pageEmoji[now_page][i]?(unsigned short *)emoji_arr[pageEmoji[now_page][i] - 1]:NULL);
            }
        }
    }

    if (this->emoji.isClicked()) {
        EVENT[EMOJI_SELECT] = 1;
    }

    if (EVENT[EMOJI_SELECT]) {
            printf("[EVENT] Press Emoji\n");
            userInfo.setVisbility(false);
            showPage.setVisbility(false);
            emoji_sc.setVisbility(true);
            EVENT[EMOJI_SELECT] = 0;
        }
        if (EVENT[EMOJI_SELECTED]) {
            printf("[EVENT] Emoji Selected\n");
            emoji_sc.setVisbility(false);
            userInfo.setVisbility(true);
            showPage.setVisbility(true);
            addImageToPage(emoji_sc.emoji_num, user_self);
            printf("emoji name: %d\n", emoji_sc.emoji_num);
            EVENT[EMOJI_SELECTED] = 0;
            need_render = true;
        }

    dpo::update(father, axis);
}