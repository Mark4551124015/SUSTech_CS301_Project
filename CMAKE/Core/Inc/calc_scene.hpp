#ifndef __CALC_SCENE_HPP__
#define __CALC_SCENE_HPP__

#include "framework.h"

#ifdef __cplusplus
extern "C" {
#endif

extern pii touch;

class calc_var_text : public dpo {
   public:
    char str[64];
    uint16_t len;
    uint16_t backgroud;
    uint16_t font_color;
    uint8_t font_size;
    pii start;
    uint8_t max_col;
    uint8_t max_row;
    int max_v_len;
    bool touching;
    bool choosing;
    bool click;
    int click_cnt;
    int cursor;
    int MAX_CALC_LEN = 30;

    int v_begin_id = 0;
    int end_id;

   public:
    calc_var_text(string name, pii pos, pii shape);

    void move_cursor_update_char(int index, pii axis);
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
using cvtext = class calc_var_text;

class calc_main : public dpo {
   public:
    enum calc_mode { IDLE, EX, EQ, BIN };
    pii key_sz = {40, 30};
    // pii key_big_sz = {60, 20};
    int max_col = 5;

    calc_mode cmode;
    cvtext input = cvtext("cv", {0, -120}, {200, 30});
    stext res = stext("res", {0, -90}, {200, 30}, (char *)"", true, 16);
    stext ex_mode_s = stext("ex", get_key_pos(2), {85, 30}, (char *)"EXPESSISON", true, 12);
    stext eq_mode_s = stext("eq", get_key_pos(2), {85, 30}, (char *)"EQUATION", true, 12);
    stext bin_mode_s =
        stext("bin", get_key_pos(2), {85, 30}, (char *)"BINARY",  true, 16);
    // button exit = button("exit", {60, 80}, {120, 80}, "Exit");
    button *num_keys[10];
    button *op_keys[7];
    button *eq_keys[3];
    button *del;
    button *equal;
    button *clear;
    button *mov_l;
    button *mov_r;
    button *mode_btn;

   public:
    calc_main(string name, pii pos, pii shape);
    ~calc_main();
    pii get_key_pos(int index);
    void set_mode(calc_mode mode);
    void update(display_object *father, pii axis) override;
};

// class calc_ex : public dpo {
//    public:
//     calc_ex(string name, pii pos, pii shape);
//     stext test = stext("test", {0, 0}, {120, 40}, (char *)"Test", 24);
//     button btn = button("test", {0, 80}, {120, 80}, "test");
//     void update(display_object *father, pii axis) override;
// };

// class calc_bin : public dpo {
//    public:
//     calc_bin(string name, pii pos, pii shape);
//     stext test = stext("test", {0, 0}, {120, 40}, (char *)"Test", 24);
//     button btn = button("test", {0, 80}, {120, 80}, "test");
//     void update(display_object *father, pii axis) override;
// };

// v_text

#ifdef __cplusplus
}
#endif

#endif