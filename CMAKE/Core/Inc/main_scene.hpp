#ifndef MAIN_SCENE_HPP
#define MAIN_SCENE_HPP
#include <string>
#include "framework.h"
#include "icon.h"

#ifdef __cplusplus
extern "C"{
#endif

class main_scene : public dpo{
    public:
        stext time = stext("time", {0, -120} , {200, 40}, "time", false, 24);
        stext userName = stext("userName", {0, -80}, {200, 40}, "UserName: User0", false, 24);
        image chat_icon = image("chat_icon", {-80, 0}, {60, 60}, (unsigned short*)gImage_chat_icon, "");
        image calc_icon = image("calc_icon", {0, 0}, {60, 60}, (unsigned short*)gImage_calc_icon, "");
        image photo_icon = image("photo_icon", {80, 0}, {60, 60}, (unsigned short*)gImage_photo_icon, "");
    public:
        main_scene(string name, pii pos, pii shape);
        void update(display_object *father, pii axis) override;
};


#ifdef __cplusplus
}
#endif
#endif