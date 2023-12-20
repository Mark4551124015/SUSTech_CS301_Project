#ifndef __FRAMWORK_H
#define __FRAMWORK_H

#include <stdint.h>
#include <stdio.h>

#include <cstddef>
// #include <cstdint>
// #include <cstdio>
#include <string.h>
#include <cstdint>
#include <string>
// #include <pair>
// #include <vector>
#include <cmath>

#include "lcd.h"
#include "stm32f1xx.h"
// #include "ff.h"
// #include "piclib.h"
using namespace std;

// using std::pair;
using std::string;
// using std::vector;

struct pii {
    int first;
    int second;
};
// using pii = std::pair<int, int>;

#ifndef x_p
#define x_p first
#endif
#ifndef y_p
#define y_p second
#endif

#define STR_SIZE 32

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
    uint8_t id;
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

    uint8_t get_id();
    void move(pii pos);
    virtual void update(display_object *father, pii axis);
};

using dpo = display_object;
typedef class var_text : public dpo {
   public:
    char str[32];
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

   public:
    button(string name, pii pos, pii shape, string str);
    bool isClicked();
    void update(display_object *father, pii axis);
    void reset();
} button;

typedef class static_text : public dpo {
   public:
    string str;
    uint16_t len;
    uint16_t font_color;
    uint16_t backgroud;
    uint8_t font_size;
    pii start;
    uint8_t max_col;
    uint8_t max_row;
    uint8_t max_len;
    bool text_aligned;

   public:
    static_text(string name, pii pos, pii shape, string str, bool text_aligned, uint8_t font_size);
    pii get_pos(int index, pii axis);
    void update(display_object *father, pii axis);
    void update_str(string str, uint8_t font_size, uint16_t font_color,
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

    // FILINFO * picfileinfo;
    string image_path;

   public:
    image(string name, pii pos, pii shape, const unsigned short *img,
          string str, string img_name="");
    bool isClicked();
    void update(display_object *father, pii axis);
    void update_img(const unsigned short *img);
    void set_image(string img_name);
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