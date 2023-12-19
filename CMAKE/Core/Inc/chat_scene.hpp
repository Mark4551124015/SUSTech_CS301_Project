#ifndef __CHAT_SCENE_HPP__
#define __CHAT_SCENE_HPP__

#include "framework.h"
#include "emoji_scene.hpp"
#ifdef __cplusplus
extern "C"{
#endif

extern pii touch;

class page : public dpo {
    public:
        int cnt;
//        static const int max_message = 6;
        image emojis[6] = {
            image("image1", {0, -50}, {30, 30}, NULL, ""),
            image("image2", {0, -20}, {30, 30}, NULL, ""),
            image("image3", {0, 10}, {30, 30}, NULL, ""),
            image("image4", {0, 40}, {30, 30}, NULL, ""),
            image("image5", {0, 70}, {30, 30}, NULL, ""),
            image("image6", {0, 100}, {30, 30}, NULL, "")
        };
        stext messages[6] = {
            stext("message1", {0, -50}, {10, 30}, (char *)"", true, 16),
            stext("message2", {0, -20}, {240, 30}, (char *)"", true, 16),
            stext("message3", {0, 10}, {240, 30}, (char *)"", true, 16),
            stext("message4", {0, 40}, {240, 30}, (char *)"", true, 16),
            stext("message5", {0, 70}, {240, 30}, (char *)"", true, 16),
            stext("message6", {0, 100}, {240, 30}, (char *)"", true, 16)
        };

    public:
        page(string name, pii pos, pii shape);
        bool addMessage(char * str);
        bool addImage(int num);
        void clear();
        void update(display_object *father, pii axis);

};




class chat_scene_main : public dpo {
    public:
        stext userInfo = stext("userInfo", {0, -130}, {240, 30}, (char *)"Chatting with ", false, 16);
        char* users[3];//0: master 1, 2: slave
        button *pre = new button("previous_page", {-60, 125}, {120, 30}, "Previous");
        button *nxt = new button("next_page", {60, 125}, {120, 30}, "Next");
        button *emoji = new button("emoji", {0, 95}, {240, 30}, "Emoji");
        int page_cnt = 0;
        int now_page = 0;
        page *pages[5];
    public:
        chat_scene_main(string name, pii pos, pii shape, char* users[3]);
        void updateMessage(char* RX_DATA);
        void addMessageToPage(char* message);
        void addImageToPage(int num);
        void update(display_object *father, pii axis) override;
};

#ifdef __cplusplus
}
#endif
#endif