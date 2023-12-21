#include "album_scene.hpp"

#include <cstdint>
#include <cstdio>
#include <string>

#include "../../FATFS/src/ff.h"

album_scene_main::album_scene_main(string name, pii pos, pii shape)
    : dpo(name, pos, shape) {
    this->photo_init();
    this->add_son(&title);
    
    title.setVisbility(true);
    this->add_son(&photo_info);

    photo_info.setVisbility(true);

    this->add_son(&photo_name);
    photo_name.setVisbility(true);

    this->add_son(&photo);
    photo.setVisbility(true);
}

album_scene_main::~album_scene_main() { myfree(picfileinfo); }

void album_scene_main::update(display_object *father, pii axis) {
    if (EVENT[REMOTE_KEY] && photo_cnt != 0) {
        if (remote_press == "RIGHT") {
            cur_photo = (cur_photo + 1) % photo_cnt;
            string temp = "Photo Display [" + to_string(cur_photo) + " / " +
                          to_string(photo_cnt) + "]";
            this->photo_info.update_str(temp, 16, BLACK, WHITE);
            this->photo_name.update_str(photo_index[cur_photo], 16, BLACK,
                                        WHITE);
            photo.set_image("0:/PICTURE/" + photo_index[cur_photo]);
        } else if (remote_press == "LEFT") {
            cur_photo = (cur_photo - 1 + photo_cnt) % photo_cnt;
            string temp = "Photo Display [" + to_string(cur_photo) + " / " +
                          to_string(photo_cnt) + "]";
            this->photo_info.update_str(temp, 16, BLACK, WHITE);
            photo.set_image("0:/PICTURE/" + photo_index[cur_photo]);
            this->photo_name.update_str(photo_index[cur_photo], 16, BLACK,
                                        WHITE);
        }
    }
    if (EVENT[REMOTE_KEY] && photo_cnt == 0) {
        this->photo_init();
    }
    dpo::update(father, axis);
}

void album_scene_main::photo_init() {
    f_mount(fs[0], "0:", 1);  // 挂载SD卡
    f_mount(fs[1], "1:", 1);  // 挂载FLASH.
    printf("mounted partition\n");

    photo_cnt = 0;
    cur_photo = 0;

    picfileinfo = (FILINFO *)mymalloc(sizeof(FILINFO));
    picdir = DIR();

    if (picfileinfo == NULL) {
        printf("malloc error\n");
        return;
    }
    mymemset(picfileinfo, 0, sizeof(FILINFO));
    u8 res;
    if ((res = f_opendir(&picdir, "0:/PICTURE"))) {
        printf("Wrong Dir 0:/PICTURE! %d\n", (int)res);
        return;
    }  // 打开图片文件夹

    photo_cnt = pic_get_tnum((uint8_t *)"0:/PICTURE");

    printf("No Picture in 0:/PICTURE !");

    if (!picfileinfo) {  // 内存分配出错
        printf("Memory Alloc Failed!");
        return;
    }
    res = f_opendir(&picdir, "0:/PICTURE");  // 打开目录

    if (res == FR_OK) {
        int curindex = 0;  // 当前索引为0
        string temp;
        while (1)  // 全部查询一遍
        {
            temp = picdir.dptr;                     // 记录当前dptr偏移
            res = f_readdir(&picdir, picfileinfo);  // 读取目录下的一个文件
            if (res != FR_OK || picfileinfo->fname[0] == 0)
                break;  // 错误了/到末尾了,退出
            res = f_typetell((u8 *)picfileinfo->fname);
            if ((res & 0XF0) == 0X50)  // 取高四位,看看是不是图片文件
            {
                string img_name = picfileinfo->fname;
                photo_index[curindex] = img_name;  // 记录索引
                printf("Adding %s\n", photo_index[curindex].c_str());
                curindex++;
            }
        }
        printf("Open Dir Success!\n");
    } else {
        printf("Open Dir Failed!\n");
    }
    printf("Done Photo Finding %d\n", photo_cnt);

    if (photo_cnt == 0) {
        photo_name.update_str("No Photo Found!", 16, BLACK, WHITE);
        photo_info.update_str("Nothing in your PICTURE/", 16, BLACK, WHITE);
    } else {
        photo_name.update_str(photo_index[0], 16, BLACK, WHITE);
        string temp = "Photo Display [" + to_string(cur_photo) + " / " +
                      to_string(photo_cnt) + "]";
        photo_info.update_str(temp, 16, BLACK, WHITE);
        photo.set_image("0:/PICTURE/" + photo_index[0]);
    }
}
