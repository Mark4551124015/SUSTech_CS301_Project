#ifndef __CALC_SCENE_HPP__
#define __CALC_SCENE_HPP__

#include "framework.h"

#ifdef __cplusplus
extern "C" {
#endif

extern pii touch;

class calc_var_text : public dpo {
   public:
    char str[255];
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

#ifdef __cplusplus
}
#endif

#endif