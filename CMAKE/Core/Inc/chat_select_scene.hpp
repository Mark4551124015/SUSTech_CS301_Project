#ifndef __CHAT_SELECT_SCENE_HPP__
#define __CHAT_SELECT_SCENE_HPP__

#include "framework.h"

#ifdef __cplusplus
extern "C"{
#endif

extern pii touch;

class chat_select_main : public dpo{
    public:
        stext userNameCol = stext("userName", {-80, -130}, {200, 30}, "UserName", false, 16);
        stext statusCol = stext("status", {0, -130}, {200, 30}, "Status", false, 16);
        stext operationCol = stext("operation", {80, -130}, {200, 30}, "Operation", false, 16);
        pii invite_key_size = {40, 30};
        pii group_key_size = {240, 40};
        button invite_key[2] = {
             button("invite_user_0", {80, -90}, this->invite_key_size, "Chat" ),
             button("invite_user_1", {80, -50}, this->invite_key_size, "Chat")
        };
        button group_key = button("group_chat", {0, 120}, this->group_key_size, "Group Chat");
        stext userName[2] = {
            stext("userName_0", {-80, -90}, {200, 30}, "User0", false, 16),
            stext("userName_1", {-80, -50}, {200, 30}, "User1", false, 16)
        };
        stext status[2] = {
            stext("status_0", {0, -90}, {200, 30}, "offline", false, 16),
            stext("status_1", {0, -50}, {200, 30}, "offline", false, 16)
        };
        stext message = stext("message", {0,20}, {240, 60}, "Message",false, 12);
    public:
        chat_select_main(string name, pii pos, pii shape);
        void change_user(int user);
        void change_status();
        bool check_can_create(int num);
        void update(display_object *father, pii axis) override;
};


#ifdef __cplusplus
}
#endif

#endif