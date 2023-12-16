#ifndef __CHAT_SELECT_SCENE_HPP__
#define __CHAT_SELECT_SCENE_HPP__

#include "framework.h"

#ifdef __cplusplus
extern "C"{
#endif

extern pii touch;

class chat_select_main : public dpo{
    public:
        stext userNameCol = stext("userName", {-80, -130}, {200, 30}, (char *)"UserName", false, 16);
        stext statusCol = stext("status", {0, -130}, {200, 30}, (char *)"Status", false, 16);
        stext operationCol = stext("operation", {80, -130}, {200, 30}, (char *)"Operation", false, 16);
        pii invite_key_size = {40, 30};
        pii group_key_size = {240, 40};
        button *invite_key[2];
        button *group_key;
        stext userName[2] = {
            stext("userName_0", {-80, -90}, {200, 30}, (char *)"User0", false, 16),
            stext("userName_1", {-80, -50}, {200, 30}, (char *)"User1", false, 16)
        };
        stext status[2] = {
            stext("status_0", {0, -90}, {200, 30}, (char *)"offline", false, 16),
            stext("status_1", {0, -50}, {200, 30}, (char*)"offline", false, 16)
        };

    public:
        chat_select_main(string name, pii pos, pii shape);
        void update(display_object *father, pii axis) override;
};


#ifdef __cplusplus
}
#endif

#endif