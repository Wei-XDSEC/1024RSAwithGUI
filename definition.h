#ifndef _DEFINITION_H_
#define _DEFINITION_H_

//头文件及支持声明
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif


#include<windows.h>
#include<Commdlg.h>
#include<tchar.h>
#include<wingdi.h>
#include<stdio.h>
#include<string.h>
#include "head.h"

/////////////////////////////////////////////////////////////////////////////////////////
///////模式状态代码
#define MYMODE_ENCRYPTION 1 //加密模式
#define MYMODE_DECRYPTION 2 //解密模式
#define MYMODE_GETRSAKEYS 3 //生成公钥和私钥模式
/////////////////////////////////////////////////////////////////////////////////////////
///////////子窗口ID
//加密模式子窗口ID
#define MYRADIOBUTTON_MODE_ENCRYPTION 1
#define MYPUSHBUTTON_ENCRYPTION_OPENFILE 100
#define MYEDIT_ENCRYPTION_OPENFILE 101
#define MYEDIT_ENCRYPTION_PUBLICKEY 102
#define MYRADIOBUTTON_ENCRYPTION_SAVETOTXT 103
#define MYRADIOBUTTON_ENCRYPTION_SAVETOBMP 104
#define MYEDIT_ENCRYPTION_PLAINTEXT 105
#define MYEDIT_ENCRYPTION_CIPHERTEXT 106
#define MYPUSHBUTTON_ENCRYPTION_ENCRYPT 107
#define MYPUSHBUTTON_ENCRYPTION_SAVETO 108
#define MYEDIT_ENCRYPTION_SAVETO 109
#define MYPUSHBUTTON_ENCRYPTION_PUBLICKEY_OPEN 110
//解密模式子窗口ID
#define MYRADIOBUTTON_MODE_DECRYPTION 2
#define MYPUSHBUTTON_DECRYPTION_OPENFILE_CIPHERTEXT 20
#define MYEDIT_DECRYPTION_OPENFILE_CIPHERTEXT 21
#define MYPUSHBUTTON_DECRYPTION_OPENFILE_PRIVATEKEY 22
#define MYEDIT_DECRYPTION_OPENFILE_PRIVATEKEY 23
#define MYEDIT_DECRYPTION_CIPHERTEXT 24
#define MYEDIT_DECRYPTION_PLAINTEXT 25
#define MYPUSHBUTTON_DECRYPTION_SAVETO 26
#define MYEDIT_DECRYPTION_SAVETO 27
#define MYPUSHBUTTON_DECRYPTION_DECRYPT 28
//生成公钥和私钥模式子窗口ID
#define MYRADIOBUTTON_MODE_GETRSAKEYS 3
#define MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOTXT 31
#define MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOBMP 32
#define MYEDIT_GETRSAKEYS_PRVKEY_SAVETO 33
#define MYPUSHBUTTON_GETRSAKEYS_PRVKEY_SAVETO 34
#define MYEDIT_GETRSAKEYS_PUBLICKEY_TEXT 35
#define MYEDIT_GETRSAKEYS_PRIVATEKEY_TEXT 36
#define MYEDIT_GETRSAKEYS_PUBKEY_SAVETO 37
#define MYPUSHBUTTON_GETRSAKEYS_PUBKEY_SAVETO 38
#define MYPUSHBUTTON_GETRSAKEYS_GETKEY 39
/////////////////////////////////////////////////////////////////////////////////////////
///////其他定义
//明文的最大大小（字节）
#define MY_MAX_SIZE_OF_PLAINTEXT 100
//公钥的最大大小（字节）
#define MY_MAX_SIZE_OF_PUBLICKEY 100
//私钥的最大大小（字节）
#define MY_MAX_SIZE_OF_PRIVATEKEY 1024
//打开文件的类型 File_Mode模式
#define OPEN_TXT 0x1
#define OPEN_BMP 0x10
#define OPEN_TXTORBMP (OPEN_TXT|OPEN_BMP)
//加密模式所需的信息
struct _ENCRYPTION_INFORMATION
{
    char szPlaintextPath[MAX_PATH];
    char szPublicKeyPath[MAX_PATH];
    char szOrgBMP_Path[MAX_PATH];
    char szSaveToPath[MAX_PATH];
    BOOL SaveTO_Bmp_Open;
};
//解密模式所需的信息
struct _DECRYPTION_INFORMATION
{
    char szCipherPath[MAX_PATH];
    char szPrivateKeyPath[MAX_PATH];
    char szSave[MAX_PATH];
    BOOL Cipher_Bmp_Open;
    BOOL PivKey_Bmp_Open;
};
//生成公钥与私钥所需的信息
struct _GETRSAKEYS_INFORMATION
{
    char szBmpPath[MAX_PATH];
    char szPrvKeySaveto_Path[MAX_PATH];
    char szPubKeySaveto_Path[MAX_PATH];
    BOOL Bmp_Open;
};

//全局变量声明
extern HWND hwnd;
extern int Mode_Status;
extern TCHAR szClassName[ ];
extern struct _ENCRYPTION_INFORMATION ENCRYPTION_INFORMATION;
extern struct _DECRYPTION_INFORMATION DECRYPTION_INFORMATION;
extern struct _GETRSAKEYS_INFORMATION GETRSAKEYS_INFORMATION;

//mian.c的函数原型

//获得当前模式的信息
int GetModeStatus(void);
//修改模式信息
void SetModeStatus(int Mode);
//显示BMP文件
void Show_BMP(int x, int y, int w, int h, const char* path, HDC hdc);
//响应WM_PAINT消息的重绘函数
void Re_Draw(void);
//初始化子窗口的函数
void Create_Subwindows(HINSTANCE hThisInstance);
//显示子窗口的函数
void Show_Subwindows(int MyMode);
//调出打开文件窗口的函数
BOOL GetFilePath(int File_Mode,char * path);
//将指定文字保存到指定BMP文件的函数
void SaveToBmp(char *SourPath, char *DestPath, char *buff, size_t buff_size);
//从BMP文件读取信息的函数
BOOL ReadfromBmp(char *SourPath, INT32 *start, INT *end);
//回调函数
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//专门响应WM_COMMAND的函数
void OnCommand(WPARAM wParam, LPARAM lParam, HWND hwnd);

#endif // _DEFINITION_H_

