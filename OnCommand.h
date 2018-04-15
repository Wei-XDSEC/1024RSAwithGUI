#include "definition.h"
//分发消息的函数
void OnCommand(WPARAM wParam, LPARAM lParam, HWND hwnd);
//选择模式界面
void OnCase_SELECT(WPARAM wParam, LPARAM lParam, HWND hwnd);
//加密模式界面
void OnCase_ENCRYPTION(WPARAM wParam, LPARAM lParam, HWND hwnd);
//解密模式界面
void OnCase_ENCRYPTION(WPARAM wParam, LPARAM lParam, HWND hwnd);
//获得公钥和私钥的界面
void OnCase_GETRSAKEYS(WPARAM wParam, LPARAM lParam, HWND hwnd);

//加密前检查
BOOL CheckBeforeEncrypt();
//加密
BOOL Encrypt(HWND hwnd);
//解密前检查
BOOL CheckBeforeDecrypt();
//解密
BOOL Decrypt(HWND hwnd);
//生成前检查
BOOL CheckBeforeGetRSAKeys(HWND hwnd);
//生成
BOOL GetRSAKeys(HWND hwnd);
//将大数保存成16进制文本形式
void WriteToText(_u8* Number, char* String);
//将16进制文本保存成大数形式
void WriteToNumber(char* String, _u8* Number);
//子线程函数，为了防止在处理时主线程无法处理消息而造成的假死现象
DWORD WINAPI ThreadProc(LPVOID mode);
