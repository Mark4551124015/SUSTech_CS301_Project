#ifndef __CHAT_SCENE_HPP__
#define __CHAT_SCENE_HPP__

#include <cstdio>
#include "emoji_scene.hpp"
#include "framework.h"

#ifdef __cplusplus
extern "C" {
#endif

extern pii touch;

class page : public dpo {
   public:
    int cnt;
    //        static const int max_message = 6;
    image emojis[6] = {image("image1", {0, -75}, {30, 30}, NULL, ""),
                       image("image2", {0, -45}, {30, 30}, NULL, ""),
                       image("image3", {0, -15}, {30, 30}, NULL, ""),
                       image("image4", {0, 15}, {30, 30}, NULL, ""),
                       image("image5", {0, 45}, {30, 30}, NULL, ""),
                       image("image6", {0, 75}, {30, 30}, NULL, "")};
    stext messages[6] = {
        stext("message1", {0, -75}, {240, 30}, (char *)"", true, 16),
        stext("message2", {0, -45}, {240, 30}, (char *)"", true, 16),
        stext("message3", {0, -15}, {240, 30}, (char *)"", true, 16),
        stext("message4", {0, 15}, {240, 30}, (char *)"", true, 16),
        stext("message5", {0, 45}, {240, 30}, (char *)"", true, 16),
        stext("message6", {0, 75}, {240, 30}, (char *)"", true, 16)};

   public:
    page(string name, pii pos, pii shape);
    string addMessage(string str);
    int addImage(int num, char const *userName);
    void clear();
    void update(display_object *father, pii axis);
};

class chat_scene_main : public dpo {
   public:
    emoji_scene_main emoji_sc = emoji_scene_main("emoji_scene", {0, 0}, {240, 280});
    stext userInfo = stext("userInfo", {0, -130}, {240, 20},
                           (char *)"Chatting with ", false, 16);
    // char *users[3];  // 0: master 1, 2: slave
    string users[3];
    button pre = button("previous_page", {-60, 125}, {120, 30}, "Previous");
    button nxt = button("next_page", {60, 125}, {120, 30}, "Next");
    button emoji = button("emoji", {0, 95}, {240, 30}, "Emoji");
    int page_cnt = 0;
    int now_page = 0;
    page showPage = page("page", {0, -20}, {240, 180});
    string pageMessage[5][6];
    int pageEmoji[5][6];
    
   public:
    chat_scene_main(string name, pii pos, pii shape, string users[3]);
    void addMessageToPage(string message, int user_num);
    void addImageToPage(int num, int user_num);
    void update(display_object *father, pii axis) override;
};

#ifdef __cplusplus
}
#endif
#endif