#ifndef __CHAT_SCENE_HPP__
#define __CHAT_SCENE_HPP__

#include "framework.h"

#ifdef __cplusplus
extern "C"{
#endif

extern pii touch;

class chat_scene_main : public dpo {
    public:
        stext userInfo = stext("userInfo", {0, -130}, {240, 30}, (char *)"Chatting with ", 16);
        char* users[3];//0: master 1, 2: slave
        button *pre = new button("previous_page", {-60, 125}, {120, 30}, "Previous");
        button *nxt = new button("next_page", {60, 125}, {120, 30}, "Next");
        button *send = new button("send", {-30, 40}, {180, 30}, "Send Message");
        button *emoji = new button("emoji", {90, 40}, {60, 30}, "Emoji");
        stext message = stext("message", {0, 80}, {240, 50}, (char *)"message", 16);
    public:
        chat_scene_main(string name, pii pos, pii shape, char* users[3]);
        void update(display_object *father, pii axis) override;
};

#ifdef __cplusplus
}
#endif
#endif