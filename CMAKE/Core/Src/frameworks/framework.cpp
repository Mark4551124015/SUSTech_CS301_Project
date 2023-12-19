
#include "framework.h"
#include "scene.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "scene.h"

extern pii touch;
extern vtext* choosed;

pii rotate_pos(pii pos, pii axis, int angle) {
    float x = pos.x_p - axis.x_p;
    float y = pos.y_p - axis.y_p;
    float rad = angle * 3.1415926 / 180;
    float x1 = x * cos(rad) - y * sin(rad);
    float y1 = x * sin(rad) + y * cos(rad);
    return {x1 + axis.x_p, y1 + axis.y_p};
}
bool IN(pii p1, pii p2, pii p3) {
    int x1 = p1.x_p, x2 = p2.x_p, x = p3.x_p;
    int y1 = p1.y_p, y2 = p2.y_p, y = p3.y_p;
    bool f1 = (x >= x1 && x <= x2) || (x <= x1 && x >= x2);
    bool f2 = (y >= y1 && y <= y2) || (y <= y1 && y >= y2);
    return f1 && f2;
}
pii adding(pii a, pii b) { return {a.x_p + b.x_p, a.y_p + b.y_p}; }
bool equal_pii(pii a, pii b) { return (a.x_p == b.x_p) && (a.y_p == b.y_p); }

pii randPII(pii x_range, pii y_range) {
    srand(SysTick->VAL);
    int x = rand() % (x_range.y_p - x_range.x_p) + x_range.x_p;
    int y = rand() % (y_range.y_p - y_range.x_p) + y_range.x_p;
    return {x, y};
}

// Display object
unsigned int dpo::cnt = 0;

dpo::display_object(string name, pii pos, pii shape) {
    this->id = dpo::cnt;
    dpo::cnt++;
    this->parent = nullptr;
    this->pos = {pos.x_p, pos.y_p};
    this->shape = {shape.x_p, shape.y_p};
    this->isVisible = true;
    this->sub_object_cnt = 0;
    memset(this->sub_object, 0, sizeof(this->sub_object));
    this->need_render = true;
    this->name = name;
    this->type = DPO;
    this->my_axis = this->pos;
    printf("Added object \"%s %d\" pos: %d,%d\n", this->name.c_str(), this->id,
           this->pos.x_p, this->pos.y_p);
}

bool dpo::set_parent(dpo* parent) {
    if (this->get_parent() != nullptr) return false;
    this->parent = parent;
    return true;
}
dpo* dpo::get_parent() { return this->parent; }

bool dpo::add_son(dpo* son) {
    // Unique confirm

    if (son == nullptr) {
        printf("bad ptr\n");
        return false;
    }
    if (!son->set_parent(this)) {
        printf("bad parent\n");
        return false;
    }
    for (dpo* i : this->sub_object)
        if (i->id == son->id) {
            printf("bad unique\n");
            return false;
        };
    this->sub_object[this->sub_object_cnt++] = son;
    return true;
}
display_object** dpo::get_son() { return this->sub_object; }

void dpo::setAlpha(float alpha) {
    alpha = alpha > 1.0 ? 1.0 : alpha;
    alpha = alpha < 0.0 ? 0.0 : alpha;
    this->alpha = alpha;
}
float dpo::getAlpha() { return this->alpha; }

void dpo::setVisbility(bool flag) {
    this->isVisible = flag;
    if (flag) {
        this->need_render = true;
    } else {
        pii p1 = {this->my_axis.x_p - this->shape.x_p / 2,
                  this->my_axis.y_p - this->shape.y_p / 2};
        pii p2 = {this->my_axis.x_p + this->shape.x_p / 2,
                  this->my_axis.y_p + this->shape.y_p / 2};
        LCD_Fill(p1.x_p, p1.y_p, p2.x_p, p2.y_p, WHITE);
        this->parent->need_render = true;
    }
}
bool dpo::getVisbility() { return this->isVisible; }

int dpo::get_id() { return this->id; }

void dpo::move(pair<int, int> pos) { this->pos = {pos.x_p, pos.y_p}; }

void dpo::update(display_object* father, pii axis) {
    this->my_axis = adding(axis, this->pos);
    // printf("updating %s\n", this->name.c_str());
    if (this->need_render) {
        for (int i = 0; i < this->sub_object_cnt; i++) {
            dpo* son = this->sub_object[i];
            if (!son->getVisbility()) continue;
            son->need_render = true;
        }
        this->need_render = false;
    }

    for (int i = 0; i < this->sub_object_cnt; i++) {
        dpo* son = this->sub_object[i];
        if (!son->getVisbility()) continue;
        // printf("son type: %d\n", (int)son->type);
        switch (son->type) {
            case (DPO): {
                son->update(this, this->my_axis);
                break;
            }
            case (BUTTON): {
                button* a = (button*)son;
                a->update(this, this->my_axis);
                break;
            }
            case (V_TEXT): {
                var_text* a = (var_text*)son;
                a->update(this, this->my_axis);
                break;
            }
            case (KEYBOARD): {
                keyboard* a = (keyboard*)son;
                a->update(this, this->my_axis);
                break;
            }
            case (S_TEXT): {
                stext* a = (stext*)son;
                a->update(this, this->my_axis);
                break;
            }
            case (IMAGE): {
                image* a = (image*)son;
                a->update(this, this->my_axis);
                break;
            }
            case (BAR): {
                bar* a = (bar*)son;
                a->update(this, this->my_axis);
                break;
            }
            case (MOVIMG): {
                mov_img* a = (mov_img*)son;
                a->update(this, this->my_axis);
                break;
            }
            case (RECT): {
                rect* a = (rect*)son;
                a->update(this, this->my_axis);
                break;
            }
            case (MARKER): {
                marker* a = (marker*)son;
                a->update(this, this->my_axis);
                break;
            }
            default:
                son->update(this, this->my_axis);
        }
    }
}

// v_text
vtext::var_text(string name, pii pos, pii shape) : dpo(name, pos, shape) {
    memset(this->str, 0, 256);
    this->len = 0;
    this->backgroud = WHITE;
    this->font_color = BLACK;
    this->font_size = 16;
    this->type = V_TEXT;
    this->choosing = false;
    this->touching = false;
    this->cursor = 0;
    this->start = {(-this->shape.x_p + 4) / 2, (-this->shape.y_p + 4) / 2};
    this->click_cnt = 0;

    // except the boarder
    this->max_col = (this->shape.x_p - 4) / (this->font_size / 2);
    this->max_row = (this->shape.y_p - 4) / this->font_size;
    this->max_len = this->max_row * this->max_col;
    this->max_len =
        this->max_len < sizeof(this->str) ? this->max_len : sizeof(this->str);
}

pii vtext::get_pos(int index, pii axis) {
    pii target;
    target = {(index % this->max_col) * (this->font_size / 2),
              (index / this->max_col) * this->font_size};
    target = adding(target, this->start);
    target = adding(target, axis);
    return target;
}

void vtext::render_char(int index, pii axis, bool clean) {
    if (!(index >= 0 && index < this->len)) return;
    pii pos = get_pos(index, axis);
    int width = this->font_size / 2;
    int height = this->font_size;
    POINT_COLOR = this->font_color;
    BACK_COLOR = this->backgroud;
    if (clean || !(this->str[index]))
        LCD_Fill(pos.x_p, pos.y_p, pos.x_p + width, pos.y_p + height,
                 BACK_COLOR);
    else
        LCD_ShowChar(pos.x_p, pos.y_p, this->str[index], this->font_size, 1);
}

void vtext::update_char(int begin, int end) {
    if (begin >= end) return;
    end = end > this->max_len ? this->max_len : end;
    // end = this->max_len;
    begin = begin < 0 ? 0 : begin;
    for (int i = begin; i < end; i++) {
        render_char(i, this->my_axis, 1);
        render_char(i, this->my_axis, 0);
    }
}

void vtext::render_cursor(int index, pii axis, uint16_t color) {
    POINT_COLOR = color;
    int width = this->font_size / 2;
    int height = this->font_size;
    if (index >= this->max_len) index--;
    // Current Place
    pii target;
    if (index != -1)
        target = this->get_pos(index, axis);
    else
        target = this->get_pos(this->cursor, axis);
    LCD_DrawRectangle(target.x_p, target.y_p, target.x_p + width,
                      target.y_p + height);
}

void vtext::move_cursor(int index, pii axis) {
    index = index < 0 ? 0 : index;
    index = index > this->len ? this->len : index;
    if (index == this->max_len) index--;
    // this->render_cursor(-1, axis, this->backgroud);
    this->render_cursor(this->cursor, axis, this->backgroud);
    this->render_char(this->cursor - 1, axis, 0);
    this->render_char(this->cursor, axis, 0);
    this->render_char(this->cursor + 1, axis, 0);
    this->render_cursor(index, axis, BLACK);
    this->cursor = index;
}

void vtext::add_on_cursor(char c) {
    if (this->len >= this->max_len) return;
    for (int i = this->max_len - 1; i > this->cursor; i--) {
        this->str[i] = this->str[i - 1];
    }
    this->str[this->cursor] = c;
    this->len++;
    this->update_char(this->cursor, this->len);
    this->move_cursor(this->cursor + 1, this->my_axis);
}
void vtext::delete_on_cursor() {
    if (this->len == 0 || this->cursor == this->len) return;
    for (int i = this->cursor; i < this->max_len - 1; i++) {
        this->str[i] = this->str[i + 1];
    }
    this->str[this->max_len - 1] = 0;
    this->update_char(this->cursor - 1, this->len);
    this->len--;
    this->move_cursor(this->cursor, this->my_axis);
}
bool vtext::isClicked() { return this->click; }

void vtext::clear() {
    memset(this->str, 0, 64);
    this->len = 0;

    this->need_render = true;
}

void vtext::update(dpo* father, pii axis) {
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
        for (int i = 0; i < this->len; i++) {
            this->render_char(i, this->my_axis, 0);
        }

        if (this->choosing) this->render_cursor(this->len, my_axis, BLACK);
    }

    // Click
    this->click = false;
    if (this->isVisible) {
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
            choosed = nullptr;
            this->render_cursor(this->cursor, my_axis, this->backgroud);
        } else {
            printf("[choosed] \"%s\"\n", this->name.c_str());
            choosed = this;
            this->choosing = true;
            this->render_cursor(this->len, my_axis, BLACK);
        }
        this->cursor = this->len;
        if (this->cursor == this->max_len) this->cursor--;
    }

    dpo::update(father, axis);
}

button::button(string name, pii pos, pii shape, string str)
    : dpo(name, pos, shape) {
    this->str = str;
    this->font_color = BLACK;
    this->font_size = 16;
    this->backgroud = YELLOW;
    this->touching = false;
    this->type = BUTTON;
    this->click_cnt = 0;
    this->click = false;
}
void button::update(dpo* father, pii axis) {
    this->my_axis = adding(axis, this->pos);
    if (father != nullptr) {
    }
    pii p1 = {this->my_axis.x_p - this->shape.x_p / 2,
              this->my_axis.y_p - this->shape.y_p / 2};
    pii p2 = {this->my_axis.x_p + this->shape.x_p / 2,
              this->my_axis.y_p + this->shape.y_p / 2};

    if (this->need_render && this->isVisible) {
        POINT_COLOR = BLACK;
        LCD_Fill(p1.x_p, p1.y_p, p2.x_p, p2.y_p, this->backgroud);
        LCD_DrawRectangle(p1.x_p, p1.y_p, p2.x_p, p2.y_p);
        int width = this->font_size / 2 * this->str.length();
        int height = this->font_size;
        POINT_COLOR = this->font_color;
        BACK_COLOR = this->backgroud;
        LCD_ShowString(this->my_axis.x_p - width / 2,
                       this->my_axis.y_p - height / 2, width, height,
                       this->font_size, (uint8_t*)this->str.c_str());
    }
    bool fly = equal_pii(touch, {65535, 65535});
    // Click
    this->click = false;
    if (this->isVisible) {
        if (this->touching && fly) this->click = true;
        if (IN(p1, p2, touch))
            this->touching = true;
        else
            this->touching = false;
    }

    if (this->click) {
        this->click_cnt++;
        printf("[clicked] \"%s\"\n", this->name.c_str());
    }

    dpo::update(father, axis);
}
bool button::isClicked() { return this->click; }

void button::reset() {
    this->click_cnt = 0;
    this->click = false;
}
pii keyboard_size = {230, 320 / 2};
pii keyboard_pos = {0, 300 / 4};

keyboard::keyboard() : dpo("keyboard", keyboard_pos, keyboard_size) {
    this->backgroud = LIGHTGREEN;
    this->key_color = LIGHTBLUE;
    this->type = KEYBOARD;
    this->shape = keyboard_size;
    this->font_color = BLACK;
    this->shifting = false;
    this->key_shape = {17, 32};
    this->max_col = (this->shape.x_p - 2 - this->key_shape.x_p) /
                        (this->key_shape.x_p + 6) +
                    1;
    this->max_row = (this->shape.y_p - 4 - this->key_shape.y_p) /
                        (this->key_shape.y_p + 2) +
                    1;
    this->start = {(-this->shape.x_p / 2) + 2 + (this->key_shape.x_p / 2),
                   (-this->shape.y_p / 2) + 2 + (this->key_shape.y_p / 2)};
    printf("max col: %d, max row %d\n", this->max_col, this->max_row);
}
void keyboard::init_keys() {
    // this->my_axis = {lcddev.width / 2, lcddev.height / 2 + this->pos.y_p};
    printf("this axis: %d, %d\n", this->my_axis.first, this->my_axis.second);
    printf("this start: %d, %d\n", this->start.first, this->start.second);
    for (int i = 0; i < 26; i++) {
        pii pos = this->get_pos(i);
        char c = 97 + i;
        string name = "key_";
        name += c;
        string show = "";
        show += c;
        this->keys[i] = new button(name, pos, this->key_shape, show);
        this->add_son(this->keys[i]);
    }
    pii big_key = {this->key_shape.x_p * 2 + 6, this->key_shape.y_p};
    pii space_key = {this->key_shape.x_p * 4 + 18, this->key_shape.y_p};

    pii pos = this->get_pos(26);
    pos = {pos.x_p + this->key_shape.x_p / 2 + 3, pos.y_p};
    this->left = new button("key_left", pos, big_key, "<<");
    this->add_son(this->left);

    pos = this->get_pos(28);
    pos = {pos.x_p + this->key_shape.x_p / 2 + 3, pos.y_p};
    this->right = new button("key_right", pos, big_key, ">>");
    this->add_son(this->right);

    pos = this->get_pos(30);
    pos = {pos.x_p + this->key_shape.x_p / 2 + 3, pos.y_p};
    this->shift = new button("key_shift", pos, big_key, "sft");
    this->add_son(this->shift);

    pos = this->get_pos(38);
    pos = {pos.x_p + this->key_shape.x_p / 2 + 3, pos.y_p};
    this->del = new button("key_del", pos, big_key, "del");
    this->add_son(this->del);

    pos = this->get_pos(34);
    pos = {pos.x_p + this->key_shape.x_p / 2 + 3, pos.y_p};
    this->space = new button("key_space", pos, space_key, "space");
    this->add_son(this->space);
}

pii keyboard::get_pos(int index) {
    pii target;
    target = {(index % this->max_col) * (this->key_shape.x_p + 6) + 2,
              (index / this->max_col) * (this->key_shape.y_p + 2) + 2};
    target = adding(target, this->start);
    return target;
}

void keyboard::update(dpo* father, pii axis) {
    this->my_axis = adding(axis, this->pos);
    pii p1 = {this->my_axis.x_p - this->shape.x_p / 2,
              this->my_axis.y_p - this->shape.y_p / 2};
    pii p2 = {this->my_axis.x_p + this->shape.x_p / 2,
              this->my_axis.y_p + this->shape.y_p / 2};
    if (this->need_render) {
        LCD_Fill(p1.x_p, p1.y_p, p2.x_p, p2.y_p, this->backgroud);
        LCD_DrawRectangle(p1.x_p, p1.y_p, p2.x_p, p2.y_p);
    }

    if (this->shift->isClicked()) {
        if (this->shifting) {
            this->shift->backgroud = YELLOW;
            this->shift->need_render = true;
            this->shifting = false;
        } else {
            this->shift->backgroud = BLUE;
            this->shift->need_render = true;
            this->shifting = true;
        }
    }

    dpo::update(father, axis);
}

char keyboard::typing() {
    for (int i = 0; i < 26; i++) {
        if (this->keys[i]->isClicked()) {
            if (this->shifting)
                return i + 65;
            else
                return i + 97;
        }
    }
    if (this->space->isClicked()) {
        return 32;
    }
    return 0;
}

stext::static_text(string name, pii pos, pii shape, char* str,
                   bool text_aligned, uint8_t font_size)
    : dpo(name, pos, shape) {
    this->type = S_TEXT;
    this->need_render = true;
    this->backgroud = WHITE;
    this->isVisible = true;
    this->text_aligned = text_aligned;
    this->str = (char*)calloc(255, sizeof(char));
    strcpy(this->str, str);
    this->font_color = BLACK;
    this->len = strlen(str);
    this->font_size = font_size;
    this->max_col = (this->shape.x_p) / (this->font_size / 2);
    this->max_row = (this->shape.y_p) / this->font_size;
    this->max_len = this->max_row * this->max_col;

    pii font_shape = {
        font_size / 2 * (this->len > this->max_col ? this->max_col : this->len),
        font_size * ((this->len - 1) / this->max_col + 1)};
    if (this->len == 0) font_shape = {0, 0};
    this->start = {(-font_shape.x_p) / 2, (-font_shape.y_p) / 2};
    if(text_aligned) this->start = {(-this->shape.x_p)/2, (-this->shape.y_p)/2};
}
void stext::clear() {
    for (int i = 0; i < this->len; i++) {
        this->render_char(i, this->my_axis, 1);
    }
}
void stext::update_str(char* str, uint8_t font_size, uint16_t font_color,
                       uint16_t backgroud) {
    this->clear();
    memset(this->str, 0, 255);
    strcpy(this->str, str);
    this->font_color = BLACK;
    this->len = strlen(str);
    this->font_size = font_size;
    this->max_col = (this->shape.x_p) / (this->font_size / 2);
    this->max_row = (this->shape.y_p) / this->font_size;
    this->max_len = this->max_row * this->max_col;

    pii font_shape = {
        font_size / 2 * (this->len > this->max_col ? this->max_col : this->len),
        font_size * ((this->len - 1) / this->max_col + 1)};
    if (this->len == 0) font_shape = {0, 0};
    this->start = {(-font_shape.x_p) / 2, (-font_shape.y_p) / 2};
    if(this->text_aligned) this->start = {(-this->shape.x_p)/2, (-this->shape.y_p)/2};
    this->backgroud = backgroud;
    this->need_render = true;
}
pii stext::get_pos(int index, pii axis) {
    pii target;
    target = {(index % this->max_col) * (this->font_size / 2),
              (index / this->max_col) * this->font_size};
    target = adding(target, this->start);
    target = adding(target, axis);
    return target;
}

void stext::render_char(int index, pii axis, bool clean) {
    if (!(index >= 0 && index < this->len)) return;
    pii pos = get_pos(index, axis);
    int width = this->font_size / 2;
    int height = this->font_size;
    POINT_COLOR = this->font_color;
    BACK_COLOR = this->backgroud;
    if (clean || !(this->str[index]))
        LCD_Fill(pos.x_p, pos.y_p, pos.x_p + width, pos.y_p + height,
                 BACK_COLOR);
    else
        LCD_ShowChar(pos.x_p, pos.y_p, this->str[index], this->font_size, 1);
}

void stext::update(dpo* father, pii axis) {
    this->my_axis = adding(axis, this->pos);
    if (this->need_render && this->isVisible) {
        for (int i = 0; i < this->len; i++) {
            this->render_char(i, this->my_axis, 1);
            this->render_char(i, this->my_axis, 0);
        }
        printf("rendered %s len: %d\n", this->name.c_str(), this->len);
    }
    dpo::update(father, axis);
}

image::image(string name, pii pos, pii shape, const unsigned short* img,
             string str)
    : dpo(name, pos, shape) {
    this->str = str;
    this->font_color = BLACK;
    this->font_size = 16;
    this->backgroud = YELLOW;
    this->touching = false;
    this->type = BUTTON;
    this->click_cnt = 0;
}
void image::update(dpo* father, pii axis) {
    this->my_axis = adding(axis, this->pos);
    if (father != nullptr) {
    }
    pii p1 = {this->my_axis.x_p - this->shape.x_p / 2,
              this->my_axis.y_p - this->shape.y_p / 2};
    pii p2 = {this->my_axis.x_p + this->shape.x_p / 2,
              this->my_axis.y_p + this->shape.y_p / 2};

    if (this->need_render && this->isVisible) {
        POINT_COLOR = BLACK;
        LCD_ShowPicture(p1.x_p, p1.y_p, this->shape.x_p, this->shape.y_p,
                        this->img);
        int width = this->font_size / 2 * this->str.length();
        int height = this->font_size;
        POINT_COLOR = this->font_color;
        BACK_COLOR = this->backgroud;
        LCD_ShowString(this->my_axis.x_p - width / 2,
                       this->my_axis.y_p - height / 2, width, height,
                       this->font_size, (uint8_t*)this->str.c_str());
    }

    // Click
    bool fly = equal_pii(touch, {65535, 65535});
    this->click = false;
    if (this->isVisible) {
        if (this->touching && fly) this->click = true;
        if (IN(p1, p2, touch))
            this->touching = true;
        else
            this->touching = false;
    }

    if (this->click) {
        this->click_cnt++;
        printf("[clicked] \"%s\"\n", this->name.c_str());
    }

    dpo::update(father, axis);
}
bool image::isClicked() { return this->click; }

rect::rectangle(string name, pii pos, pii shape, uint16_t backgroud)
    : dpo(name, pos, shape) {
    this->backgroud = backgroud;
}

void rect::update(dpo* father, pii axis) {
    this->my_axis = adding(axis, this->pos);
    if (father != nullptr) {
    }
    pii p1 = {this->my_axis.x_p - this->shape.x_p / 2,
              this->my_axis.y_p - this->shape.y_p / 2};
    pii p2 = {this->my_axis.x_p + this->shape.x_p / 2,
              this->my_axis.y_p + this->shape.y_p / 2};

    if (this->need_render && this->isVisible) {
        POINT_COLOR = BLACK;
        LCD_Fill(p1.x_p, p1.y_p, p2.x_p, p2.y_p, this->backgroud);
        LCD_DrawRectangle(p1.x_p, p1.y_p, p2.x_p, p2.y_p);
    }
    dpo::update(father, axis);
}