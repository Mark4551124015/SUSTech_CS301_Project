#include "calc_scene.hpp"

#include <algorithm>
#include <cstdio>

using std::max;
using std::min;

cvtext::calc_var_text(string name, pii pos, pii shape) : dpo(name, pos, shape) {
    memset(this->str, 0, 256);
    this->len = 0;
    this->backgroud = WHITE;
    this->font_color = BLACK;
    this->font_size = 24;
    this->type = CV_TEXT;
    this->choosing = false;
    this->touching = false;
    this->cursor = 0;
    this->start = {(-this->shape.x_p + 4) / 2, (-this->shape.y_p + 4) / 2};
    this->click_cnt = 0;

    // except the boarder
    this->max_col = (this->shape.x_p - 4) / (this->font_size / 2);
    this->max_row = 1;
    this->max_v_len = (int)this->max_row * this->max_col;
    this->max_v_len = min(this->max_v_len, (int)sizeof(this->str));
}

pii cvtext::get_pos(int index, pii axis) {
    pii target;
    target = {(index % this->max_col) * (this->font_size / 2),
              (index / this->max_col) * this->font_size};
    target = adding(target, this->start);
    target = adding(target, axis);
    return target;
}

void cvtext::render_char(int real_id, pii axis, bool clean) {
    if (real_id == this->len) {
        clean = true;
    } else if (!(real_id >= 0 && real_id < this->len)) {
        return;
    }

    int pos_id = real_id - this->v_begin_id;
    if (!(pos_id >= 0 && pos_id < this->max_v_len)) {
        return;
    }

    pii pos = get_pos(pos_id, axis);
    int width = this->font_size / 2;
    int height = this->font_size;
    POINT_COLOR = this->font_color;
    BACK_COLOR = this->backgroud;

    if (clean || !(this->str[real_id]))
        LCD_Fill(pos.x_p, pos.y_p, pos.x_p + width, pos.y_p + height,
                 BACK_COLOR);
    else
        LCD_ShowChar(pos.x_p, pos.y_p, this->str[real_id], this->font_size, 1);
}

void cvtext::update_char(int begin, int end) {
    if (begin > end) return;
    end = min(end, this->MAX_CALC_LEN);
    begin = max(0, begin);

    for (int i = begin; i <= end; i++) {
        render_char(i, this->my_axis, 1);
        render_char(i, this->my_axis, 0);
    }
}

void cvtext::render_cursor(int real_id, pii axis, uint16_t color) {
    uint16_t saved_COLOR = POINT_COLOR;

    POINT_COLOR = (cursor == 0 || cursor == MAX_CALC_LEN) ? RED : color;
    int pos_id = real_id - this->v_begin_id;
    int width = this->font_size / 2;
    int height = this->font_size;
    pos_id = min((int)this->max_v_len - 1, pos_id);
    // Current Place
    printf("render curser pos:%d", pos_id);
    pii target;
    if (pos_id != -1)
        target = this->get_pos(pos_id, axis);
    else
        target = this->get_pos(pos_id, axis);

    printf("tar_curser pos:%d, %d", target.x_p, target.y_p);
    LCD_DrawRectangle(target.x_p, target.y_p, target.x_p + width,
                      target.y_p + height);

    POINT_COLOR = saved_COLOR;
}

void cvtext::move_cursor_update_char(int real_id, pii axis) {
    real_id = max(real_id, 0);
    real_id = min(real_id, (int)this->len);
    this->cursor = real_id;

    if (real_id == this->len) {
        this->v_begin_id = max(real_id - (this->max_v_len - 1), 0);
    } else if (real_id > this->v_begin_id + (this->max_v_len - 1)) {
        this->v_begin_id = real_id - (this->max_v_len - 1);
    } else if (real_id < this->v_begin_id) {
        this->v_begin_id = real_id;
    }

    int pos_id = real_id - this->v_begin_id;

    // this->render_cursor(-1, axis, this->backgroud);
    this->update_char(0, this->len);
    this->render_cursor(this->cursor, axis, BLACK);
}
// complete
void cvtext::add_on_cursor(char c) {
    if (this->len >= this->MAX_CALC_LEN) return;
    for (int i = this->MAX_CALC_LEN - 1; i > this->cursor; i--) {
        this->str[i] = this->str[i - 1];
    }
    this->str[this->cursor] = c;
    this->len++;
    this->move_cursor_update_char(this->cursor + 1, this->my_axis);
}
// complete
void cvtext::delete_on_cursor() {
    if (this->len == 0 || this->cursor == this->len) return;
    for (int i = this->cursor; i < this->MAX_CALC_LEN - 1; i++) {
        this->str[i] = this->str[i + 1];
    }
    this->str[this->MAX_CALC_LEN - 1] = '\0';
    this->str[this->len - 1] = '\0';
    this->len--;
    this->move_cursor_update_char(this->cursor, this->my_axis);
}
bool cvtext::isClicked() { return this->click; }

void cvtext::clear() {
    memset(this->str, 0, 256);
    this->len = 0;
    this->v_begin_id = 0;

    this->need_render = true;
}

void cvtext::update(dpo *father, pii axis) {
    this->my_axis = adding(axis, this->pos);
    if (father != nullptr) {
    }

    pii p1 = {this->my_axis.x_p - this->shape.x_p / 2,
              this->my_axis.y_p - this->shape.y_p / 2};
    pii p2 = {this->my_axis.x_p + this->shape.x_p / 2,
              this->my_axis.y_p + this->shape.y_p / 2};

    bool fly = equal_pii(touch, {65535, 65535});

    // Full Render
    if (this->need_render && this->isVisible) {
        // Draw BG

        LCD_Fill(p1.x_p, p1.y_p, p2.x_p, p2.y_p, this->backgroud);
        POINT_COLOR = BLACK;
        LCD_DrawRectangle(p1.x_p, p1.y_p, p2.x_p, p2.y_p);
        POINT_COLOR = this->font_color;
        // Draw Letters
        update_char(0, this->len);

        if (this->choosing) {
            this->cursor =
                min((int)this->len, this->v_begin_id + (int)this->max_v_len);
            this->render_cursor(this->cursor, my_axis, BLACK);
        }
    }

    // Click
    if (this->isVisible) {
        this->click = false;
        if (this->touching && fly) this->click = true;
        if (IN(p1, p2, touch))
            this->touching = true;
        else
            this->touching = false;
    }
    if (this->click) {
        this->click_cnt++;
        if (this->choosing) {
            printf("[unchoosed]\"%s\"\n", this->name.c_str());
            this->choosing = false;
            cv_choosed = nullptr;
            this->render_cursor(this->cursor, my_axis, this->backgroud);
        } else {
            printf("[choosed] \"%s\"\n", this->name.c_str());
            cv_choosed = this;
            this->choosing = true;
            this->render_cursor(this->len, my_axis, BLACK);
        }
        this->cursor = this->len;
        if (this->cursor == this->max_v_len) this->cursor--;
    }

    dpo::update(father, axis);
}