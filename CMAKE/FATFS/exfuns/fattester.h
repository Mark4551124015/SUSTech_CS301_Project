#ifndef __FATTESTER_H__
#define __FATTESTER_H__
#include "ff.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////
// ������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
// ALIENTEK STM32������
// FATFS ���Դ���
// ����ԭ��@ALIENTEK
// ������̳:www.openedv.com
// ��������:2016/1/7
// �汾��V1.0
// ��Ȩ���У�����ؾ���
// Copyright(C) �������������ӿƼ����޹�˾
// 2014-2024 All rights reserved
//////////////////////////////////////////////////////////////////////////////////

u8 mf_mount(u8 *path, u8 mt);
u8 mf_open(u8 *path, u8 mode);
u8 mf_close(void);
u8 mf_read(u16 len);
u8 mf_write(u8 *dat, u16 len);
u8 mf_opendir(u8 *path);
u8 mf_closedir(void);
u8 mf_readdir(void);
u8 mf_scan_files(u8 *path);
u32 mf_showfree(u8 *drv);
u8 mf_lseek(u32 offset);
u32 mf_tell(void);
u32 mf_size(void);
u8 mf_mkdir(u8 *pname);
u8 mf_fmkfs(u8 *path, u8 mode, u16 au);
u8 mf_unlink(u8 *pname);
u8 mf_rename(u8 *oldname, u8 *newname);
void mf_getlabel(u8 *path);
void mf_setlabel(u8 *path);
void mf_gets(u16 size);
u8 mf_putc(u8 c);
u8 mf_puts(u8 *c);

#endif
#ifdef __cplusplus
}
#endif