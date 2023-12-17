#ifndef __FRAMWORK_H
#define __FRAMWORK_H

#include <stdint.h>
#include <stdio.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>

#include "lcd.h"
#include "stm32f1xx.h"

using namespace std;

using std::pair;
using std::string;
using std::vector;

using pii = std::pair<int, int>;

#ifndef x_p
#define x_p first
#endif
#ifndef y_p
#define y_p second
#endif



#ifdef __cplusplus  // 使用C语言的方式编译方法名。

pii adding(pii a, pii b);
pii randPII(pii x_range, pii y_range);
pii rotate_pos(pii pos, pii axis, int angle);

extern "C" {
#endif
bool IN(pii p1, pii p2, pii p3);
bool equal_pii(pii a, pii b);



enum dpo_type { DPO, BUTTON, V_TEXT, KEYBOARD, S_TEXT, IMAGE, BAR, SLD, MOVIMG, RECT, MARKER, CANVAS, CV_TEXT };

class display_object {
   public:
    int id;
    dpo_type type;
    display_object *parent;
    pii pos;
    pii my_axis;
    pii shape;
    bool need_render;
    // vector<display_object *> sub_object ;
    display_object * sub_object [32];
    size_t sub_object_cnt;

    float alpha;
    bool isVisible;

   public:
    string name;
    static unsigned int cnt;
    static UART_HandleTypeDef uart;

   public:
    display_object(string name, pii pos, pii shape);

    bool set_parent(display_object *parent);
    display_object *get_parent();

    bool add_son(display_object *son);
    display_object ** get_son();

    void setAlpha(float alpha);
    float getAlpha();

    void setVisbility(bool flag);
    bool getVisbility();

    int get_id();
    void move(pair<int, int> pos);
    virtual void update(display_object *father, pii axis);
};

using dpo = display_object;
typedef class var_text : public dpo {
   public:
    char str[255];
    uint16_t len;
    uint16_t backgroud;
    uint16_t font_color;
    uint8_t font_size;
    pii start;
    uint8_t max_col;
    uint8_t max_row;
    uint8_t max_len;
    bool touching;
    bool choosing;
    bool click;
    int click_cnt;
    uint8_t cursor;

   public:
    var_text(string name, pii pos, pii shape);
    void move_cursor(int index, pii axis);
    void add_on_cursor(char c);
    void delete_on_cursor();
    void clear();
    bool isClicked();
    pii get_pos(int index, pii axis);
    void render_char(int index, pii axis, bool clean);
    void render_cursor(int index, pii axis, uint16_t color);
    void update_char(int start, int end);
    void update(display_object *father, pii axis);
} vtext;

typedef class button : public dpo {
   public:
    bool touching;
    string str;
    uint16_t backgroud;
    uint16_t font_color;
    uint8_t font_size;
    bool click;
    int click_cnt;

   public:
    button(string name, pii pos, pii shape, string str);
    bool isClicked();
    void update(display_object *father, pii axis);
    void reset();
} button;

typedef class keyboard : public dpo {
   public:
    button *keys[26];
    button *shift;
    button *del;
    button *left;
    button *right;
    button *space;

    uint16_t backgroud;
    uint16_t key_color;
    uint16_t font_color;
    pii key_shape;
    pii start;

    uint8_t max_col;
    uint8_t max_row;
    bool shifting;
    char output;

   public:
    keyboard();
    bool isClicked();
    void init_keys();
    char typing();
    pii get_pos(int index);
    void update(display_object *father, pii axis);
    // void setVisbility(bool flag);
} keyboard;

typedef class static_text : public dpo {
   public:
    char * str;
    uint16_t len;
    uint16_t font_color;
    uint16_t backgroud;
    uint8_t font_size;
    pii start;
    uint8_t max_col;
    uint8_t max_row;
    uint8_t max_len;

   public:
    static_text(string name, pii pos, pii shape, char *str, uint8_t font_size);
    pii get_pos(int index, pii axis);
    void update(display_object *father, pii axis);
    void update_str(char *str, uint8_t font_size, uint16_t font_color,
                    uint16_t backgroud);
    void render_char(int index, pii axis, bool clean);
    void clear();
} stext;

typedef class image : public dpo {
   public:
    bool touching;
    string str;
    uint16_t backgroud;
    uint16_t font_color;
    uint8_t font_size;
    bool click;
    int click_cnt;
    const unsigned short *img;

   public:
    image(string name, pii pos, pii shape, const unsigned short *img,
          string str);
    bool isClicked();
    void update(display_object *father, pii axis);
} image;

typedef class rectangle : public dpo {
    public:
    uint16_t backgroud;
     public:
     rectangle(string name, pii pos, pii shape, uint16_t backgroud);
    void update(display_object *father, pii axis);
} rect;

typedef class moving_image : public dpo {
   public:
    bool touching;
    moving_image(string name, pii pos, pii shape, const unsigned short *img,
                 pii image_shape, pii corp_start);
    uint32_t pixel_cnt;
    pii corp_start;
    pii image_shape;
    uint16_t *backup;
    void backup_lcd();
    void restore_lcd();
    bool dragging;
    pii last_touch;
    pii last_pos;
    void update(display_object *father, pii axis);
    dpo_type type = MOVIMG;
    const unsigned short *img;

} mov_img;


typedef class marker : public dpo {
   public:
    marker(string name, pii pos, pii shape, char c);
    char c;
    size_t font_size;
    uint16_t font_color;
    uint16_t *backup;

    void set(pii pos);
    void remove();

    void backup_lcd();
    void restore_lcd();
    
    dpo_type type = MARKER;

    void update(display_object *father, pii axis);

} marker;

#ifdef __cplusplus  // 使用C语言的方式编译方法名。
}
#endif

#endif