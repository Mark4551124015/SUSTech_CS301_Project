#ifndef __ALBUM_SCENE_HPP__
#define __ALBUM_SCENE_HPP__

#include "framework.h"
#include "piclib.h"

#ifdef __cplusplus
extern "C" {
#endif



class album_scene_main : public dpo {
   public:
    image photo = image("photo", {0, 0}, {180, 240}, NULL, "");
    string photo_index[10];
    int photo_cnt;
 	DIR picdir;
    FILINFO *picfileinfo;
    int cur_photo;

   public:
    album_scene_main(string name, pii pos, pii shape);
    ~album_scene_main();
    void update(display_object *father, pii axis) override;
};

#ifdef __cplusplus
}
#endif

#endif