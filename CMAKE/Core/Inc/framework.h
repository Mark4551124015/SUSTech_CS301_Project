#ifndef __FRAMWORK_H
#define __FRAMWORK_H

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

#include "lcd.h"
#include "stm32f1xx.h"

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
extern "C" {
#endif

bool IN(pii p1, pii p2, pii p3);
bool equal_pii(pii a, pii b);

enum dpo_type { DPO, BUTTON, V_TEXT, KEYBOARD, S_TEXT };

class display_object {
   public:
    int id;
    dpo_type type;
    display_object *parent;
    pii pos;
    pii my_axis;
    pii shape;
    bool need_render;
    vector<display_object *> sub_object;
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
    vector<display_object *> get_son();

    void setAlpha(float alpha);
    float getAlpha();

    void setVisbility(bool flag);
    bool getVisbility();

    int get_id();
    void move(pii pos);
    virtual void update(display_object *father, pii axis);
};
using dpo = display_object;

class var_text : public dpo {
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
    void update(display_object *father, pii axis) override;
};
using vtext = var_text;

class button : public dpo {
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
    void update(display_object *father, pii axis) override;
};

class keyboard : public dpo {
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
    void update(display_object *father, pii axis) override;
    // void setVisbility(bool flag);
};

class static_text : public dpo {
   public:
    char str[255];
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
    void update(display_object *father, pii axis) override;
    void update_str(char *str, uint8_t font_size, uint16_t font_color,
                    uint16_t backgroud);
    void render_char(int index, pii axis, bool clean);
    void clear();
};
using stext = static_text;

#ifdef __cplusplus  // 使用C语言的方式编译方法名。
}
#endif

#endif