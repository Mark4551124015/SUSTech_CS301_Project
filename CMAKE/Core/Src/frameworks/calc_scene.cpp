#include "calc_scene.hpp"

#include <algorithm>
#include <cstdio>

#include "calc_algo.hpp"

using std::max;
using std::min;

cvtext::calc_var_text(string name, pii pos, pii shape) : dpo(name, pos, shape) {
    memset(this->str, 0, 64);
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
    memset(this->str, 0, 64);
    this->len = 0;
    this->v_begin_id = 0;

    this->need_render = true;
}

void cvtext::update(dpo* father, pii axis) {
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
            this->render_cursor(this->cursor, my_axis, this->backgroud);
        } else {
            printf("[choosed] \"%s\"\n", this->name.c_str());
            this->choosing = true;
            this->render_cursor(this->len, my_axis, BLACK);
        }
        this->cursor = this->len;
        if (this->cursor == this->max_v_len) this->cursor--;
    }

    dpo::update(father, axis);
}

pii calc_main::get_key_pos(int index) {
    pii target;
    target = {(index % this->max_col) * (this->key_sz.x_p + 6) + 2,
              (index / this->max_col) * (this->key_sz.y_p + 2) + 2};
    target = adding(target, {-95, -50});
    return target;
}

calc_main::calc_main(string name, pii pos, pii shape) : dpo(name, pos, shape) {
    // initail for stext
    pii key_pos, temp_pos;
    key_pos = this->get_key_pos(2);
    temp_pos = this->get_key_pos(3);
    key_pos.x_p = (key_pos.x_p + temp_pos.x_p) / 2;
    key_pos.y_p = (key_pos.y_p + temp_pos.y_p) / 2;
    this->ex_mode_s =
        stext("ex", key_pos, {80, 30}, (char*)"EXPRESSION", false, 12);
    this->eq_mode_s =
        stext("eq", key_pos, {80, 30}, (char*)"EQUATION", false, 12);
    this->bin_mode_s =
        stext("eq", key_pos, {80, 30}, (char*)"BINARY", false, 12);

    for (int i = 0; i <= 9; ++i) {
        char c = '0' + i;
        string name = "key_";
        name += c;
        string show = "";
        show += c;
        this->num_keys.emplace_back(name, get_key_pos(i + 10), this->key_sz,
                                    show);
    }

    this->op_keys.emplace_back("key_add", get_key_pos(20), this->key_sz, "+");
    this->op_keys.emplace_back("key_sub", get_key_pos(21), this->key_sz, "-");
    this->op_keys.emplace_back("key_mul", get_key_pos(22), this->key_sz, "*");
    this->op_keys.emplace_back("key_div", get_key_pos(23), this->key_sz, "/");
    this->op_keys.emplace_back("key_exp", get_key_pos(24), this->key_sz, "^");
    this->op_keys.emplace_back("key_lpa", get_key_pos(25), this->key_sz, "(");
    this->op_keys.emplace_back("key_rpa", get_key_pos(26), this->key_sz, ")");

    this->eq_keys.emplace_back("key_x", get_key_pos(27), this->key_sz, "x");
    this->eq_keys.emplace_back("key_y", get_key_pos(28), this->key_sz, "y");
    this->eq_keys.emplace_back("confirm", get_key_pos(7), this->key_sz, "CFM");

    /*
        add all sons
    */
    // add buttons
    for (size_t i = 0; i <= 9; ++i) {
        this->add_son(&this->num_keys[i]);
    }
    for (size_t i = 0; i < 7; i++) {
        this->add_son(&this->op_keys[i]);
    }
    for (size_t i = 0; i < 3; i++) {
        this->add_son(&this->eq_keys[i]);
    }
    this->add_son(&this->equal);
    this->add_son(&this->mov_l);
    this->add_son(&this->mov_r);
    this->add_son(&this->clear);
    this->add_son(&this->del);
    this->add_son(&this->mode_btn);
    // add stexts

    this->add_son(&this->ex_mode_s);
    this->add_son(&this->bin_mode_s);
    this->add_son(&this->eq_mode_s);

    this->add_son(&this->input);
    this->add_son(&this->res);

    // set_mode(EX);
    this->cmode = EX;
}

calc_main::~calc_main() {}

void calc_main::set_mode(calc_mode mode) {
    printf("set to %d\n", (int)mode);
    this->input.clear();
    switch (mode) {
        case EX: {
            this->cmode = EX;
            this->ex_mode_s.setVisbility(true);
            this->eq_mode_s.setVisbility(false);
            this->bin_mode_s.setVisbility(false);

            for (size_t i = 0; i < 10; ++i) {
                this->num_keys[i].setVisbility(true);
            }
            for (size_t i = 0; i < 7; ++i) {
                this->op_keys[i].setVisbility(true);
            }
            for (size_t i = 0; i < 3; ++i) {
                this->eq_keys[i].setVisbility(false);
            }
            this->del.setVisbility(true);
            this->equal.setVisbility(true);
            this->clear.setVisbility(true);
            this->mov_l.setVisbility(true);
            this->mov_r.setVisbility(true);
            this->mode_btn.setVisbility(true);
            this->input.clear();
            this->res.clear();
            this->input.setVisbility(true);
            this->res.setVisbility(true);
            break;
        }
        case EQ: {
            this->cmode = EQ;
            this->ex_mode_s.setVisbility(false);
            this->eq_mode_s.setVisbility(true);
            this->bin_mode_s.setVisbility(false);

            for (size_t i = 0; i < 10; ++i) {
                this->num_keys[i].setVisbility(true);
            }
            for (size_t i = 0; i < 7; ++i) {
                this->op_keys[i].setVisbility(true);
            }
            for (size_t i = 0; i < 3; ++i) {
                this->eq_keys[i].setVisbility(true);
            }
            this->del.setVisbility(true);
            this->equal.setVisbility(true);
            this->clear.setVisbility(true);
            this->mov_l.setVisbility(true);
            this->mov_r.setVisbility(true);
            this->mode_btn.setVisbility(true);
            this->input.clear();
            this->res.clear();
            this->input.setVisbility(true);
            this->res.setVisbility(true);
            break;
        }
        case BIN: {
            this->cmode = BIN;
            this->ex_mode_s.setVisbility(false);
            this->eq_mode_s.setVisbility(false);
            this->bin_mode_s.setVisbility(true);

            for (size_t i = 0; i < 10; ++i) {
                this->num_keys[i].setVisbility((i <= 1 ? true : false));
            }
            for (size_t i = 0; i < 7; ++i) {
                this->op_keys[i].setVisbility(true);
            }
            for (size_t i = 0; i < 3; ++i) {
                this->eq_keys[i].setVisbility(false);
            }
            this->del.setVisbility(true);
            this->equal.setVisbility(true);
            this->clear.setVisbility(true);
            this->mov_l.setVisbility(true);
            this->mov_r.setVisbility(true);
            this->mode_btn.setVisbility(true);
            this->input.clear();
            this->res.clear();
            this->input.setVisbility(true);
            this->res.setVisbility(true);
            break;
        }
        default:
            break;
    }
}

void calc_main::update(display_object* father, pii axis) {
    // if (this->getVisbility() && this->cmode == IDLE) {
    //     this->need_render = true;
    //     set_mode(EX);
    // }
    if (this->mode_btn.isClicked()) {
        printf("set mode\n");
        this->need_render = true;
        if (this->cmode == IDLE) {
            set_mode(EX);
        } else if (this->cmode == EX) {
            set_mode(EQ);
        } else if (this->cmode == EQ) {
            set_mode(BIN);
        } else if (this->cmode == BIN) {
            set_mode(EX);
        }
    }
    if (this->equal.isClicked()) {
        if (this->cmode == EQ) {
            this->input.add_on_cursor('=');
            dpo::update(father, axis);
            return;
        }
        this->res.clear();
        string res_str;
        string ex_str(this->input.str, this->input.len);
        printf("%sEnd\n", ex_str.c_str());
        if (this->cmode == EX) {
            res_str = getExRes(ex_str);
        }
        if (this->cmode == BIN) {
            res_str = getBinRes(ex_str);
        }
        printf("res: %s\n", (char*)res_str.c_str());
        this->res.need_render = true;
        this->res.update_str((char*)res_str.c_str(), 24, BLACK, WHITE);
    }
    for (size_t i = 0; i < 10; i++) {
        if (this->num_keys[i].isClicked()) {
            this->input.add_on_cursor('0' + i);
        }
    }
    for (size_t i = 0; i < 7; i++) {
        if (this->op_keys[i].isClicked()) {
            this->input.add_on_cursor(this->op_keys[i].str[0]);
        }
    }
    if (this->mov_l.isClicked()) {
        this->input.move_cursor_update_char(this->input.cursor - 1,
                                            this->input.my_axis);
    }
    if (this->mov_r.isClicked()) {
        this->input.move_cursor_update_char(this->input.cursor + 1,
                                            this->input.my_axis);
    }
    if (this->del.isClicked()) {
        this->input.delete_on_cursor();
    }
    if (this->clear.isClicked()) {
        this->input.clear();
        this->res.clear();
    }
    if (this->cmode == EQ) {
        if (this->eq_keys[0].isClicked()) {
            this->input.add_on_cursor('x');
        }
        if (this->eq_keys[1].isClicked()) {
            this->input.add_on_cursor('y');
        }
        if (this->eq_keys[2].isClicked()) {
            string ex_str(this->input.str, this->input.len);
            string res_str = getEqRes(ex_str);
            printf("res: %s\n", (char*)res_str.c_str());
            this->res.need_render = true;
            this->res.update_str((char*)res_str.c_str(), 24, BLACK, WHITE);
        }
    }

    dpo::update(father, axis);
}

