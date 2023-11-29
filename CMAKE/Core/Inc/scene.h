#ifndef __SCENE_H
#define __SCENE_H

#include "framework.h"

extern const unsigned char IMAGE_Pineapple[28808];

#ifdef __cplusplus  // 使用C语言的方式编译方法名。
extern "C" {
#endif

typedef class main_menu : public dpo {
   public:
    main_menu(string name, pii pos, pii shape);
    stext test = stext("test", {0, 0}, {120, 40}, (char *)"Test", 24);
    button btn = button("test", {0, 80}, {120, 80}, "test");
    image test_img = image("test_img", {0,0}, {120,120}, (unsigned short *)IMAGE_Pineapple, "PNG");
    void update(display_object *father, pii axis);
} main_menu;

#ifdef __cplusplus  // 使用C语言的方式编译方法名。
}
#endif

#endif