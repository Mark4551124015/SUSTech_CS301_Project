#include "chat_select_scene.hpp"
extern string users[3];
extern u8 user_status[3];
extern int selected_chat;
extern int user_code;
chat_select_main::chat_select_main(string name, pii pos, pii shape) : dpo (name, pos, shape){
    for(int i = 0; i < 2; i++)
    {
        this->add_son(&invite_key[i]);
        this->add_son(&this->userName[i]);
        this->add_son(&this->status[i]);
    }
    change_status();
    this->add_son(&group_key);
    this->add_son(&this->userNameCol);
    this->add_son(&this->statusCol);
    this->add_son(&this->operationCol);
    this->add_son(&this->message);
    users[0] = "User0";
    users[1] = "User1";
    users[2] = "User2";

}

void chat_select_main::change_status(){
    switch (user_code) {
        case 0:
            if (user_status[1] == 1) {
                this->status[0].update_str("Online", 16, BLACK, WHITE);
            }
            if (user_status[2] == 1) {
                this->status[1].update_str("Online", 16, BLACK, WHITE);
            }
            break;
        case 1:
            if (user_status[0] == 1) {
                this->status[0].update_str("Online", 16, BLACK, WHITE);
            }
            if (user_status[2] == 1) {
                this->status[1].update_str("Online", 16, BLACK, WHITE);
            }
            break;
        case 2:
            if (user_status[0] == 1) {
                this->status[0].update_str("Online", 16, BLACK, WHITE);
            }
            if (user_status[1] == 1) {
                this->status[1].update_str("Online", 16, BLACK, WHITE);
            }
            break;
    }
}

void chat_select_main::change_user(int user){
    if (user == 0) {
            users[0] = "User1";
            users[1] = "User2";
            users[2] = "";
        } else if (user == 1) {
            users[0] = "User0";
            users[1] = "User2";
            users[2] = "";
        } else {
            users[0] = "User0";
            users[1] = "User1";
            users[2] = "";
        }
}

bool chat_select_main::check_can_create(int num){
    switch (num) {
        case 0:
            switch (user_code) {
                case 0:
                    return user_status[1] == 1;
                case 1:
                    return user_status[0] == 1;
                case 2:
                    return user_status[0] == 1;
            }
            break;
        case 1:
            switch (user_code) {
                case 0:
                    return user_status[2] == 1;
                case 1:
                    return user_status[2] == 1;
                case 2:
                    return user_status[1] == 1;
            }
            break;
        case 2:
            switch (user_code) {
                case 0:
                    return user_status[1] == 1 && user_status[2] == 1;
                case 1:
                    return user_status[0] == 1 && user_status[2] == 1;
                case 2:
                    return user_status[0] == 1 && user_status[1] == 1;
            }
            break;
    }
}

void chat_select_main::update(display_object *father, pii axis){
    if(invite_key[0].isClicked())
    {
        if (check_can_create(0)) {
            EVENT[CREAT_CHAT] = 1;
            selected_chat = 0;
            printf("create chat 0\n");
            this->message.update_str("", 16, BLACK, WHITE);
        }else {
            this->message.update_str("This User is offline, so you can not create chat.", 16, BLACK, WHITE);
        }
    }
    if(invite_key[1].isClicked())
    {
        if (check_can_create(1)) {
            EVENT[CREAT_CHAT] = 1;
            selected_chat = 1;
            printf("create chat 1\n");
            this->message.update_str("", 16, BLACK, WHITE);
        }
        else {
            this->message.update_str("This User is offline, so you can not create chat.", 16, BLACK, WHITE);
        }
    }
    if(group_key.isClicked())
    {
        if (check_can_create(2)) {
            EVENT[CREAT_CHAT] = 1;
            selected_chat = 2;
            printf("create chat 2\n");
            this->message.update_str("", 16, BLACK, WHITE);
        }
        else {
            this->message.update_str("Not all users are online, so you cannot create a chat", 16, BLACK, WHITE);
        }
    }
    dpo::update(father, axis);
}