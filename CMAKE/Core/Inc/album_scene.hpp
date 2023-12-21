#ifndef __ALBUM_SCENE_HPP__
#define __ALBUM_SCENE_HPP__

#include "framework.h"
#include "piclib.h"

#ifdef __cplusplus
extern "C" {
#endif

class album_scene_main : public dpo {
   public:
    stext title = stext("title", {0, -120}, {240, 40}, "Album", false, 24);
    image photo = image("photo", {0, 20}, {180, 180}, NULL, "");
    stext photo_info = stext("photo_info", {0, -90}, {240, 40}, "0/0", false, 16);
    stext photo_name = stext("photo_name", {0, 120}, {240, 40}, "No Photo", false, 16);
    string photo_index[10];
    int photo_cnt;
    DIR picdir;
    FILINFO *picfileinfo;
    int cur_photo;

   public:
    album_scene_main(string name, pii pos, pii shape);
    ~album_scene_main();
    void photo_init();
    void update(display_object *father, pii axis) override;
};

#ifdef __cplusplus
}
#endif

#endif