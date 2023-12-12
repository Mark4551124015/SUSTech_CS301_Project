#ifndef __CHAT_SCENE_HPP__
#define __CHAT_SCENE_HPP__

#include "framework.h"

#ifdef __cplusplus
extern "C"{
#endif

extern pii touch;

class chat_scene_main : public dpo {
    public:
        stext userInfo = stext("userInfo", {0, -130}, {200, 30}, "You are chatting with ", 16);
        string users[3];//0: master 1, 2: slave
        button *pre = new button("previous_page", {-60, 125}, {120, 30}, "Previous");
        button *nxt = new button("next_page", {60, 125}, {120, 30}, "Next");
        button *send = new button("send", {0, 40}, {240, 30}, "Send Message");
        stext message = stext("message", {-80, 65}, {240, 30}, "message", 16);
    public:
        chat_scene_main(string name, pii pos, pii shape);
        void update(display_object *father, pii axis) override;
};

#ifdef __cplusplus
}
#endif
#endif