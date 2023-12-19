#ifndef __SCENE_H
#define __SCENE_H

#include "framework.h"
#include "calc_scene.hpp"


#ifdef __cplusplus  // 使用C语言的方式编译方法名。
extern "C" {
#endif

class main_menu : public dpo {
   public:
    main_menu(string name, pii pos, pii shape);
    stext test = stext("test", {0, 0}, {120, 40}, (char *)"Test", false, 24);
    button btn = button("test", {0, 80}, {120, 80}, "test");
    void update(display_object *father, pii axis) override;
};


typedef class bar : public dpo {
   public:
    bar(string name, pii pos, pii shape);
    button back = button("return", {-60, 0}, {120, 40}, "BACK");
    button home = button("home", {60, 0}, {120, 40}, "HOME");
    void update(display_object *father, pii axis);
} bar;

#ifdef __cplusplus  // 使用C语言的方式编译方法名。
}
#endif

#endif