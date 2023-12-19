#ifndef FAKE_FATFS_H
#define FAKE_FATFS_H

#ifdef __cplusplus  // 使用C语言的方式编译方法名。
extern "C" {
#endif

void MX_FATFS_Init();

#ifdef __cplusplus  // 使用C语言的方式编译方法名。
}
#endif
#endif