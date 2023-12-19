
#ifndef EMOJI_SCENE_HPP
#define EMOJI_SCENE_HPP

#include "framework.h"
#include "emoji.hpp"
#include <cstring>
#include <string>

#ifdef __cplusplus
extern "C"{
#endif


extern pii touch;

class emoji_scene_main : public dpo {
   public:
      image emoji1 = image("emoji1", {-90, -120}, {30, 30}, NULL, "");
      image emoji2 = image("emoji2", {-30, -120}, {30, 30}, NULL, "");
      image emoji3 = image("emoji3", {30, -120}, {30, 30}, NULL, "");
      image emoji4 = image("emoji4", {90, -120}, {30, 30}, NULL, "");
      button *emoji = new button("emoji", {90, 40}, {60, 30}, "Emoji");
      int emoji_num = 0;
   public:
    emoji_scene_main(string name, pii pos, pii shape);
    void update(display_object *father, pii axis) override;
};

#ifdef __cplusplus
}
#endif
#endif
