#include "definition.h"
#include "OnCommand.h"
//全局变量
//RSA加密所需的全局变量
extern _u8 *P;
extern _u8 *Q;
extern _u8 *fai;
extern _u8 **PublicKey;
extern _u8 **PrivateKey;
//图形界面所需的全局变量
HWND hwnd;
HANDLE Myhandle;
DWORD MyThreadId;
int Mode_Status = 1;
TCHAR szClassName[ ] = _T("RSA加密程序");
struct _ENCRYPTION_INFORMATION ENCRYPTION_INFORMATION = {{0},{0},{0},{0},{0},FALSE};
struct _DECRYPTION_INFORMATION DECRYPTION_INFORMATION = {{0},{0},{0},FALSE, FALSE};
struct _GETRSAKEYS_INFORMATION GETRSAKEYS_INFORMATION = {{0},{0},{0},FALSE};


int WINAPI WinMain (HINSTANCE hThisInstance,HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,int nCmdShow)
{

    MSG messages;
    WNDCLASSEX wincl;
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) COLOR_WINDOW;

    if (!RegisterClassEx (&wincl))
        return 0;

    hwnd = CreateWindowEx (
           0,
           szClassName,
           _T("RSA加密器"),
           WS_OVERLAPPEDWINDOW &  ~WS_SIZEBOX,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           1000,
           510,
           HWND_DESKTOP,
           NULL,
           hThisInstance,
           NULL
           );

    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);
    Create_Subwindows(hThisInstance);
    Show_Subwindows(MYMODE_ENCRYPTION);
    CheckDlgButton(hwnd, MYRADIOBUTTON_MODE_ENCRYPTION, TRUE);
    SetModeStatus(MYMODE_ENCRYPTION);

    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
            break;
        case WM_PAINT:
            Re_Draw();
            break;
        case WM_DESTROY:
            PostQuitMessage (0);
            break;
        case WM_COMMAND:
                OnCommand(wParam, lParam, hwnd);
            break;
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

int GetModeStatus(void)
{
    return Mode_Status;
}

void SetModeStatus(int Mode)
{
    Mode_Status = Mode;
}


void Show_BMP(int x, int y, int w, int h, const char* path, HDC hdc)
{
    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    SelectObject(hMemDC, hBitmap);
    SetStretchBltMode(hdc,STRETCH_HALFTONE);
    StretchBlt(hdc, x,y, w,h, hMemDC, 0,0,bmp.bmWidth, bmp.bmHeight, SRCCOPY);
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
}

void Re_Draw(void)
{
    HDC hdc;
    PAINTSTRUCT ps;

    hdc = BeginPaint(hwnd, &ps);
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, 50, 15 , _T("选择操作模式："), _tcslen(_T("选择操作模式：")));
    MoveToEx(hdc, 0,50, NULL);
    LineTo(hdc, 1000, 50);
    MoveToEx(hdc, 250,50, NULL);
    LineTo(hdc, 250, 500);
    switch(GetModeStatus())
    {
    case MYMODE_ENCRYPTION:
        TextOut(hdc, 15, 90, _T("1.打开明文文件："), _tcslen(_T("1.打开明文文件：")));
        TextOut(hdc, 15, 190, _T("2.打开公钥文件："), _tcslen(_T("2.打开公钥文件：")));
        TextOut(hdc, 15, 300, _T("3.生成的密文保存类型："), _tcslen(_T("3.生成的密文保存类型：")));
        TextOut(hdc, 15, 380, _T("4.生成的密文文件保存到："), _tcslen(_T("4.生成的密文文件保存到：")));
        break;
    case MYMODE_DECRYPTION:
        TextOut(hdc, 15, 90, _T("打开密文文件："), _tcslen(_T("打开密文文件：")));
        TextOut(hdc, 15, 230, _T("打开私钥文件："), _tcslen(_T("打开私钥文件：")));
        TextOut(hdc, 15, 370, _T("生成的明文文件保存到："), _tcslen(_T("生成的明文文件保存到：")));
        break;
    case MYMODE_GETRSAKEYS:
        TextOut(hdc, 15, 100, _T("私钥保存方式："), _tcslen(_T("私钥保存方式：")));
        TextOut(hdc, 15, 200, _T("私钥保存位置："), _tcslen(_T("私钥保存位置：")));
        TextOut(hdc, 15, 310, _T("公钥保存位置："), _tcslen(_T("公钥保存位置：")));
        break;
    }


    if(ENCRYPTION_INFORMATION.SaveTO_Bmp_Open
       && GetModeStatus() == MYMODE_ENCRYPTION)
    {
        ShowWindow(GetDlgItem(hwnd, MYEDIT_ENCRYPTION_CIPHERTEXT), SW_HIDE);
        Rectangle(hdc, 599,89,951,391);
        Show_BMP(600,90,350,300,ENCRYPTION_INFORMATION.szOrgBMP_Path, hdc);
    }
    else if(DECRYPTION_INFORMATION.Cipher_Bmp_Open
        && GetModeStatus() == MYMODE_DECRYPTION)
    {
        ShowWindow(GetDlgItem(hwnd, MYEDIT_DECRYPTION_CIPHERTEXT), SW_HIDE);
        Rectangle(hdc, 269,89,621,391);
        Show_BMP(270,90,350,300,DECRYPTION_INFORMATION.szCipherPath, hdc);

    }
    else if(GETRSAKEYS_INFORMATION.Bmp_Open
        && GetModeStatus() == MYMODE_GETRSAKEYS)
    {
        ShowWindow(GetDlgItem(hwnd, MYEDIT_GETRSAKEYS_PRIVATEKEY_TEXT), SW_HIDE);
        Rectangle(hdc, 599,89,951,391);
        Show_BMP(600,90,350,300,GETRSAKEYS_INFORMATION.szBmpPath, hdc);
    }

    EndPaint(hwnd, &ps);
}

void Create_Subwindows(HINSTANCE hThisInstance)
{
    //选择模式按钮
    CreateWindowEx(0, "button", _T("加密"), WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
                     250,10,150,30, hwnd,(HMENU)MYRADIOBUTTON_MODE_ENCRYPTION, hThisInstance, NULL);
    CreateWindowEx(0, "button", _T("解密"), WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
                     500,10,150,30, hwnd,(HMENU)MYRADIOBUTTON_MODE_DECRYPTION, hThisInstance, NULL);
    CreateWindowEx(0, "button", _T("生成公钥与私钥"), WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
                     750,10,150,30, hwnd,(HMENU)MYRADIOBUTTON_MODE_GETRSAKEYS, hThisInstance, NULL);

    //加密模式界面
    CreateWindowEx(0, "button", _T("打开"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                    190,150,50,25, hwnd,(HMENU)MYPUSHBUTTON_ENCRYPTION_OPENFILE, hThisInstance, NULL);
    CreateWindowEx(WS_EX_CLIENTEDGE, "edit", _T(""), WS_CHILD|ES_MULTILINE|WS_VISIBLE|ES_AUTOHSCROLL,
                    15,115,225,25,hwnd,(HMENU)MYEDIT_ENCRYPTION_OPENFILE, hThisInstance, NULL);
    CreateWindowEx(WS_EX_CLIENTEDGE, "edit", _T(""), WS_CHILD|ES_MULTILINE|WS_VISIBLE|ES_AUTOHSCROLL,
                    15,215,225,25,hwnd,(HMENU)MYEDIT_ENCRYPTION_PUBLICKEY, hThisInstance, NULL);
    CreateWindowEx(0, "button", _T("打开"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                    190,250,50,25, hwnd,(HMENU)MYPUSHBUTTON_ENCRYPTION_PUBLICKEY_OPEN, hThisInstance, NULL);
    CreateWindowEx(0, "button", _T("文本文件（TXT）"), WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
                     15,320,150,30, hwnd,(HMENU)MYRADIOBUTTON_ENCRYPTION_SAVETOTXT, hThisInstance, NULL);
    CreateWindowEx(0, "button", _T("位图文件（BMP）"), WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
                     15,345,150,30, hwnd,(HMENU)MYRADIOBUTTON_ENCRYPTION_SAVETOBMP, hThisInstance, NULL);
    CreateWindowEx(WS_EX_CLIENTEDGE, "edit", _T(""), WS_CHILD|ES_MULTILINE|WS_VISIBLE|ES_AUTOHSCROLL,
                    15,400,225,25,hwnd,(HMENU)MYEDIT_ENCRYPTION_SAVETO, hThisInstance, NULL);
    CreateWindowEx(0, "button", _T("保存至"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                    165,435,75,25, hwnd,(HMENU)MYPUSHBUTTON_ENCRYPTION_SAVETO, hThisInstance, NULL);
    CreateWindowEx(WS_EX_CLIENTEDGE, "edit", _T("明文"), WS_CHILD|ES_MULTILINE|WS_VISIBLE|WS_VSCROLL|ES_READONLY,
                    270,90,300,300,hwnd,(HMENU)MYEDIT_ENCRYPTION_PLAINTEXT, hThisInstance, NULL);
    CreateWindowEx(WS_EX_CLIENTEDGE, "edit", _T("密文（只显示前两百字节）"), WS_CHILD|ES_MULTILINE|WS_VISIBLE|WS_VSCROLL|ES_READONLY,
                    600,90, 350,300,hwnd,(HMENU)MYEDIT_ENCRYPTION_CIPHERTEXT, hThisInstance, NULL);
    CreateWindowEx(0, "button", _T("加密"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                    540,410,90,30, hwnd,(HMENU)MYPUSHBUTTON_ENCRYPTION_ENCRYPT, hThisInstance, NULL);

    //解密模式界面
    CreateWindowEx(0, "button", _T("打开"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                    190,150,50,25, hwnd,(HMENU)MYPUSHBUTTON_DECRYPTION_OPENFILE_CIPHERTEXT, hThisInstance, NULL);
    CreateWindowEx(WS_EX_CLIENTEDGE, "edit", _T(""), WS_CHILD|ES_MULTILINE|WS_VISIBLE|ES_AUTOHSCROLL,
                    15,115,225,25,hwnd,(HMENU)MYEDIT_DECRYPTION_OPENFILE_CIPHERTEXT, hThisInstance, NULL);
    CreateWindowEx(0, "button", _T("打开"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                    190,290,50,25, hwnd,(HMENU)MYPUSHBUTTON_DECRYPTION_OPENFILE_PRIVATEKEY, hThisInstance, NULL);
    CreateWindowEx(WS_EX_CLIENTEDGE, "edit", _T(""), WS_CHILD|ES_MULTILINE|WS_VISIBLE|ES_AUTOHSCROLL,
                    15,255,225,25,hwnd,(HMENU)MYEDIT_DECRYPTION_OPENFILE_PRIVATEKEY, hThisInstance, NULL);
    CreateWindowEx(0, "button", _T("保存至"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                    165,430,75,25, hwnd,(HMENU)MYPUSHBUTTON_DECRYPTION_SAVETO, hThisInstance, NULL);
    CreateWindowEx(WS_EX_CLIENTEDGE, "edit", _T(""), WS_CHILD|ES_MULTILINE|WS_VISIBLE|ES_AUTOHSCROLL,
                    15,395,225,25,hwnd,(HMENU)MYEDIT_DECRYPTION_SAVETO, hThisInstance, NULL);
    CreateWindowEx(WS_EX_CLIENTEDGE, "edit", _T("密文"), WS_CHILD|ES_MULTILINE|WS_VISIBLE|WS_VSCROLL|ES_READONLY,
                    270,90, 350,300,hwnd,(HMENU)MYEDIT_DECRYPTION_CIPHERTEXT, hThisInstance, NULL);
    CreateWindowEx(WS_EX_CLIENTEDGE, "edit", _T("明文"), WS_CHILD|ES_MULTILINE|WS_VISIBLE|WS_VSCROLL|ES_READONLY,
                    650,90, 300,300,hwnd,(HMENU)MYEDIT_DECRYPTION_PLAINTEXT, hThisInstance, NULL);
    CreateWindowEx(0, "button", _T("解密"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                    540,410,90,30, hwnd,(HMENU)MYPUSHBUTTON_DECRYPTION_DECRYPT, hThisInstance, NULL);

    //生成公钥与私钥界面
    CreateWindowEx(0, "button", _T("文本文件（TXT）"), WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
                     15,120,150,30, hwnd,(HMENU)MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOTXT, hThisInstance, NULL);
    CreateWindowEx(0, "button", _T("位图文件（BMP）"), WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
                     15,145,150,30, hwnd,(HMENU)MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOBMP, hThisInstance, NULL);
    CreateWindowEx(WS_EX_CLIENTEDGE, "edit", _T(""), WS_CHILD|ES_MULTILINE|WS_VISIBLE|ES_AUTOHSCROLL,
                    15,225,225,25,hwnd,(HMENU)MYEDIT_GETRSAKEYS_PRVKEY_SAVETO, hThisInstance, NULL);
    CreateWindowEx(0, "button", _T("保存至"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                    165,260,75,25, hwnd,(HMENU)MYPUSHBUTTON_GETRSAKEYS_PRVKEY_SAVETO, hThisInstance, NULL);
    CreateWindowEx(WS_EX_CLIENTEDGE, "edit", _T(""), WS_CHILD|ES_MULTILINE|WS_VISIBLE|ES_AUTOHSCROLL,
                    15,330,225,25,hwnd,(HMENU)MYEDIT_GETRSAKEYS_PUBKEY_SAVETO, hThisInstance, NULL);
    CreateWindowEx(0, "button", _T("保存至"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                    165,365,75,25, hwnd,(HMENU)MYPUSHBUTTON_GETRSAKEYS_PUBKEY_SAVETO, hThisInstance, NULL);
    CreateWindowEx(WS_EX_CLIENTEDGE, "edit", _T("公钥"), WS_CHILD|ES_MULTILINE|WS_VISIBLE|WS_VSCROLL|ES_READONLY,
                    270,90, 300,300,hwnd,(HMENU)MYEDIT_GETRSAKEYS_PUBLICKEY_TEXT, hThisInstance, NULL);
    CreateWindowEx(WS_EX_CLIENTEDGE, "edit", _T("私钥"), WS_CHILD|ES_MULTILINE|WS_VISIBLE|WS_VSCROLL|ES_READONLY,
                    600,90, 350,300,hwnd,(HMENU)MYEDIT_GETRSAKEYS_PRIVATEKEY_TEXT, hThisInstance, NULL);
    CreateWindowEx(0, "button", _T("生成"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                    540,410,90,30, hwnd,(HMENU)MYPUSHBUTTON_GETRSAKEYS_GETKEY, hThisInstance, NULL);
}

void Show_Subwindows(int MyMode)
{
    int idc;
    for(idc = 100; idc <= 110; idc++)
        ShowWindow(GetDlgItem(hwnd, idc), SW_HIDE);
    for(idc = 20; idc <= 28; idc++)
        ShowWindow(GetDlgItem(hwnd, idc), SW_HIDE);
    for(idc = 31; idc <= 39; idc++)
        ShowWindow(GetDlgItem(hwnd, idc), SW_HIDE);
    switch(MyMode)
    {
    case MYMODE_ENCRYPTION:
        for(idc = 100; idc <= 110; idc++)
            ShowWindow(GetDlgItem(hwnd, idc), SW_SHOW);
        break;
    case MYMODE_DECRYPTION:
        for(idc = 20; idc <= 28; idc++)
            ShowWindow(GetDlgItem(hwnd, idc), SW_SHOW);
        break;
    case MYMODE_GETRSAKEYS:
        for(idc = 31; idc <= 39; idc++)
            ShowWindow(GetDlgItem(hwnd, idc), SW_SHOW);
        break;
    }
}

BOOL GetFilePath(int File_Mode,char * path)
{
    switch(File_Mode)
    {
        case OPEN_TXT:
        {
            OPENFILENAME file = {0};
            file.lStructSize = sizeof(file);
            file.lpstrFile = path;
            file.nMaxFile = MAX_PATH;
            file.lpstrFilter = "Text Files(*.txt, *.TXT)\0*.txt\0\0";
            file.nFilterIndex = 1;

            if(GetOpenFileName(&file))
                return TRUE;
            break;
        }
        case OPEN_BMP:
        {
            OPENFILENAME file = {0};
            file.lStructSize = sizeof(file);
            file.lpstrFile = path;
            file.nMaxFile = MAX_PATH;
            file.lpstrFilter = "BitMap Files(*.bmp, *.BMP)\0*.bmp\0\0";
            file.nFilterIndex = 1;
            if(GetOpenFileName(&file))
                return TRUE;
            break;
        }
        case OPEN_TXTORBMP:
        {
            OPENFILENAME file = {0};
            file.lStructSize = sizeof(file);
            file.lpstrFile = path;
            file.nMaxFile = MAX_PATH;
            file.lpstrFilter = "BitMap Files(*.bmp, *.BMP)\0*.bmp\0Text Files(*.txt, *.TXT)\0*.txt\0\0";
            file.nFilterIndex = 1;
            if(GetOpenFileName(&file))
                return TRUE;
            break;
        }
    }
    return FALSE;
}



void SaveToBmp(char *SourPath, char *DestPath, char *buff, size_t buff_size)
{
    char head[] = {'Q','L','|'};
    char rear[] = {'|','L','Q'};
    //考虑到BMP文件大小和操指针，决定以文件映射方式打开读取的BMP文件
    //打开文件
    HANDLE hFile = CreateFile(SourPath, GENERIC_READ,
    FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    //创建文件映射
    HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, NULL, NULL, NULL);
    //文件在内存中的头指针
    LPVOID lpBase = MapViewOfFile(hMap, FILE_MAP_READ, 0,0,0);
    //储存BMP文件的信息的结构体（在文件开头）
    BITMAPFILEHEADER *pFileHeader;
    pFileHeader = (BITMAPFILEHEADER *)lpBase;
    //获得文件大小
    size_t file_size = GetFileSize(hFile, NULL);
    FILE *fp;
    //获得写入内容后的新的BMP图像内容区域的开头偏移
    INT32 offset = pFileHeader->bfOffBits+6+buff_size;
    fp = fopen(DestPath, "wb");
    //写入储存偏移量内容之前的内容
    fwrite(lpBase, sizeof(char), 0xA, fp);
    //写入新的偏移
    fwrite((char*)&(offset), sizeof(char), 4, fp);
    //写入剩余的在图像内容区域之前的内容
    fwrite(lpBase+0xE, sizeof(char), pFileHeader->bfOffBits-0XE,fp);
    //写入信息
    fwrite(head, sizeof(char), 3, fp);
    fwrite(buff, sizeof(char), buff_size, fp);
    fwrite(rear, sizeof(char), 3, fp);
    //写入图像文件剩余内容
    fwrite(lpBase+pFileHeader->bfOffBits, sizeof(char), file_size-pFileHeader->bfOffBits, fp);

    fclose(fp);
    UnmapViewOfFile(lpBase);
    CloseHandle(hMap);
    CloseHandle(hFile);

}

BOOL ReadfromBmp(char *SourPath, INT32 *start, INT *end)
{
    //考虑到BMP文件大小和操指针，决定以文件映射方式打开读取的BMP文件
    //打开文件
    HANDLE hFile = CreateFile(SourPath, GENERIC_READ,
    FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    //创建文件映射
    HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, NULL, NULL, NULL);
    int i = 0;
    //获得文件大小
        size_t file_size = GetFileSize(hFile, NULL);
    //文件在内存中的头指针
    char *lpBase = (char*)MapViewOfFile(hMap, FILE_MAP_READ, 0,0,0);
    //储存BMP文件的信息的结构体（在文件开头）
    BITMAPFILEHEADER *pFileHeader;
    pFileHeader = (BITMAPFILEHEADER *)lpBase;
    if(lpBase[0] != 'B' || lpBase[1] != 'M')
    {
        UnmapViewOfFile(lpBase);
        CloseHandle(hMap);
        CloseHandle(hFile);
        return FALSE;
    }

    while(lpBase[i] != 'Q' || lpBase[i+1] != 'L' || lpBase[i+2] != '|')
        if(++i >= file_size-3)
        {
            UnmapViewOfFile(lpBase);
            CloseHandle(hMap);
            CloseHandle(hFile);
            return FALSE;
        }
    (*start) = i;
    (*end) = pFileHeader->bfOffBits;
    UnmapViewOfFile(lpBase);
    CloseHandle(hMap);
    CloseHandle(hFile);
    return TRUE;
}
