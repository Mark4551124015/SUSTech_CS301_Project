#ifndef __EXFUNS_H
#define __EXFUNS_H
// #include <sys.h>
#include "ff.h"
#include "main.h"
#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////
// ������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
// ALIENTEK STM32������
// FATFS ��չ����
// ����ԭ��@ALIENTEK
// ������̳:www.openedv.com
// ��������:2016/1/7
// �汾��V1.1
// ��Ȩ���У�����ؾ���
// Copyright(C) �������������ӿƼ����޹�˾
// 2014-2024 All rights reserved
//********************************************************************************
// ����˵��
// V1.1
// ����exf_copy����,�ļ�������ʾ�����bug
//////////////////////////////////////////////////////////////////////////////////

extern FATFS *fs[_VOLUMES];
extern FIL *file;
extern FIL *ftemp;
extern UINT br, bw;
extern FILINFO fileinfo;
extern DIR dir;
extern u8 *fatbuf;  // SD�����ݻ�����

// f_typetell���ص����Ͷ���
// ���ݱ�FILE_TYPE_TBL���.��exfuns.c���涨��
#define T_BIN 0X00  // bin�ļ�
#define T_LRC 0X10  // lrc�ļ�

#define T_NES 0X20  // nes�ļ�
#define T_SMS 0X21  // sms�ļ�

#define T_TEXT 0X30  //.txt�ļ�
#define T_C 0X31     //.c�ļ�
#define T_H 0X32     //.h�ļ�

#define T_WAV 0X40   // WAV�ļ�
#define T_MP3 0X41   // MP3�ļ�
#define T_APE 0X42   // APE�ļ�
#define T_FLAC 0X43  // FLAC�ļ�

#define T_BMP 0X50   // bmp�ļ�
#define T_JPG 0X51   // jpg�ļ�
#define T_JPEG 0X52  // jpeg�ļ�
#define T_GIF 0X53   // gif�ļ�

#define T_AVI 0X60  // avi�ļ�

// u8 exfuns_init(void);
u8 exfuns_init();
// u8 exfuns_init(void); //�����ڴ�
u8 f_typetell(u8 *fname);  // ʶ���ļ�����
// u8 exf_getfree(u8 *drv, u32 *total, u32 *free);
u8 exf_getfree(u8 *drv, u32 *total, u32 *free);
// u32 exf_fdsize(u8 *fdname);  // �õ��ļ��д�С
u8 *exf_get_src_dname(u8 *dpfn);
u8 exf_copy(u8 (*fcpymsg)(u8 *pname, u8 pct, u8 mode), u8 *psrc, u8 *pdst,
            u32 totsize, u32 cpdsize, u8 fwmode);  // �ļ�����
u8 exf_fdcopy(u8 (*fcpymsg)(u8 *pname, u8 pct, u8 mode), u8 *psrc, u8 *pdst,
              u32 *totsize, u32 *cpdsize, u8 fwmode);  // �ļ��и���
#endif
#ifdef __cplusplus
}
#endif
