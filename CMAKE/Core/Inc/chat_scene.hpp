#ifndef __CHAT_SCENE_HPP__
#define __CHAT_SCENE_HPP__

#include "framework.h"

#ifdef __cplusplus
extern "C"{
#endif

extern pii touch;

class page : public dpo {
    public:
        int cnt;
        stext messages[4] = {
            stext("message1", {0, -50}, {240, 30}, (char *)"", true, 16),
            stext("message2", {0, -20}, {240, 30}, (char *)"", true, 16),
            stext("message3", {0, 10}, {240, 30}, (char *)"", true, 16),
            stext("message4", {0, 40}, {240, 30}, (char *)"", true, 16)
        };
    public:
        page(string name, pii pos, pii shape);
        bool addMessage(char * str);
        void clear();
        void update(display_object *father, pii axis);
};




class chat_scene_main : public dpo {
    public:
        stext userInfo = stext("userInfo", {0, -130}, {240, 30}, (char *)"Chatting with ", false, 16);
        char* users[3];//0: master 1, 2: slave
        button *pre = new button("previous_page", {-60, 125}, {120, 30}, "Previous");
        button *nxt = new button("next_page", {60, 125}, {120, 30}, "Next");
        button *send = new button("send", {-30, 40}, {180, 30}, "Send Message");
        button *emoji = new button("emoji", {90, 40}, {60, 30}, "Emoji");
        stext message = stext("message", {0, 85}, {240, 50}, (char *)"message", true, 16);
        int page_cnt;
        page* pages[5];
    public:
        chat_scene_main(string name, pii pos, pii shape, char* users[3]);
        void updateMessage(char* RX_DATA);
        void addMessageToPage(char* message);
        void update(display_object *father, pii axis) override;
};

#ifdef __cplusplus
}
#endif
#endif