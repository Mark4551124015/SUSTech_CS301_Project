#ifndef __SCENE_H
#define __SCENE_H

#include "framework.h"

#ifdef __cplusplus  // 使用C语言的方式编译方法名。
extern "C" {
#endif

class main_menu : public dpo {
   public:
    main_menu(string name, pii pos, pii shape);
    stext test = stext("test", {0, 0}, {120, 40}, (char *)"Test", 24);
    button btn = button("test", {0, 80}, {120, 80}, "test");
    void update(display_object *father, pii axis) override;
};

#ifdef __cplusplus  // 使用C语言的方式编译方法名。
}
#endif

#endif