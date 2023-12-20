#include "album_scene.h"

#include <cstdint>
#include <cstdio>

#include "../../FATFS/src/ff.h"


album_scene_main::album_scene_main(string name, pii pos, pii shape)
    : dpo(name, pos, shape) {
    f_mount(fs[0], "0:", 1);  // 挂载SD卡
    f_mount(fs[1], "1:", 1);  // 挂载FLASH.
    printf("mounted partition\n");

    photo_cnt = 0;
    picfileinfo = (FILINFO *)mymalloc(sizeof(FILINFO));
    printf("New FILINFO\n");
    picdir = DIR();
    printf("New Dir\n");

    if (picfileinfo == NULL) {
        printf("malloc error\n");
    }
    mymemset(picfileinfo, 0, sizeof(FILINFO));
    u8 res;
    while ((res = f_opendir(&picdir, "0:/PICTURE")))  // 打开图片文件夹
    {
        printf("Wrong Dir 0:/PICTURE! %d\n", (int)res);
        delay_ms(200);
    }
    photo_cnt = pic_get_tnum((uint8_t *)"0:/PICTURE");
    while (photo_cnt == NULL) {
        printf("No Picture in 0:/PICTURE !");
        delay_ms(200);
    }
    while (!picfileinfo)  // 内存分配出错
    {
        printf("Memory Alloc Failed!");
        delay_ms(200);
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
                photo_index[curindex] = "0:/PICTURE/" + img_name;  // 记录索引
                printf("Adding %s\n", photo_index[curindex].c_str());
                curindex++;
            }
        }
        printf("Open Dir Success!\n");
    } else {
        printf("Open Dir Failed!\n");
    }
    printf("Done Photo Finding %d\n", photo_cnt);
    photo.set_image(photo_index[0]);
    this->add_son(&photo);
    photo.setVisbility(true);
}

album_scene_main::~album_scene_main() { free(picfileinfo); }

void album_scene_main::update(display_object *father, pii axis) {
    
    dpo::update(father, axis);
}
