#include "OnCommand.h"
extern _u8 *P;
extern _u8 *Q;
extern _u8 *fai;
extern _u8 **PublicKey;
extern _u8 **PrivateKey;
extern HWND hwnd;
extern HANDLE Myhandle;
extern DWORD MyThreadId;
void OnCommand(WPARAM wParam, LPARAM lParam, HWND hwnd)
{
    DWORD exitcode;
    GetExitCodeThread(Myhandle, &exitcode);
    switch(LOWORD(wParam))
    {
    //选择界面
    case MYRADIOBUTTON_MODE_ENCRYPTION:
    case MYRADIOBUTTON_MODE_DECRYPTION:
    case MYRADIOBUTTON_MODE_GETRSAKEYS:
        if(exitcode == STILL_ACTIVE)
        {
            MessageBox(hwnd, "系统正在运行中，请稍后。", "提示", MB_OK);
            return;
        }
    OnCase_SELECT(wParam, lParam, hwnd);
        break;
    //加密模式界面按钮
    case MYPUSHBUTTON_ENCRYPTION_OPENFILE:
    case MYRADIOBUTTON_ENCRYPTION_SAVETOTXT:
    case MYRADIOBUTTON_ENCRYPTION_SAVETOBMP:
    case MYPUSHBUTTON_ENCRYPTION_SAVETO:
    case MYPUSHBUTTON_ENCRYPTION_ENCRYPT:
    case MYPUSHBUTTON_ENCRYPTION_PUBLICKEY_OPEN:
        if(exitcode == STILL_ACTIVE)
        {
            MessageBox(hwnd, "正在加密中，请稍后。", "提示", MB_OK);
            return;
        }
        OnCase_ENCRYPTION(wParam,lParam,hwnd);
        break;
    //解密模式界面按钮
    case MYPUSHBUTTON_DECRYPTION_OPENFILE_CIPHERTEXT:
    case MYPUSHBUTTON_DECRYPTION_OPENFILE_PRIVATEKEY:
    case MYPUSHBUTTON_DECRYPTION_SAVETO:
    case MYPUSHBUTTON_DECRYPTION_DECRYPT:
        if(exitcode == STILL_ACTIVE)
        {
            MessageBox(hwnd, "正在解密中，密文越长画的时间就越长，请稍后。", "提示", MB_OK);
            return;
        }
        OnCase_DECRYPTION(wParam,lParam,hwnd);
        break;
    //生成公钥与私钥界面按钮
    case MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOTXT:
    case MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOBMP:
    case MYPUSHBUTTON_GETRSAKEYS_PRVKEY_SAVETO:
    case MYPUSHBUTTON_GETRSAKEYS_PUBKEY_SAVETO:
    case MYPUSHBUTTON_GETRSAKEYS_GETKEY:
        if(exitcode == STILL_ACTIVE)
        {
            MessageBox(hwnd, "正在生成中，根据机器性能和运气，可能需要0到30分钟不等，请稍后。", "提示", MB_OK);
            return;
        }
        OnCase_GETRSAKEYS(wParam,lParam,hwnd);
        break;
    }

}

void OnCase_SELECT(WPARAM wParam, LPARAM lParam, HWND hwnd)
{
    switch(LOWORD(wParam))
    {
    case MYRADIOBUTTON_MODE_ENCRYPTION:
    {
        Show_Subwindows(MYMODE_ENCRYPTION);
        CheckDlgButton(hwnd, MYRADIOBUTTON_MODE_ENCRYPTION, TRUE);
        CheckDlgButton(hwnd, MYRADIOBUTTON_MODE_DECRYPTION, FALSE);
        CheckDlgButton(hwnd, MYRADIOBUTTON_MODE_GETRSAKEYS, FALSE);
        SetModeStatus(MYMODE_ENCRYPTION);
        InvalidateRect(hwnd, NULL, 1);
        break;
    }
    case MYRADIOBUTTON_MODE_DECRYPTION:
    {
        Show_Subwindows(MYMODE_DECRYPTION);
        CheckDlgButton(hwnd, MYRADIOBUTTON_MODE_ENCRYPTION, FALSE);
        CheckDlgButton(hwnd, MYRADIOBUTTON_MODE_DECRYPTION, TRUE);
        CheckDlgButton(hwnd, MYRADIOBUTTON_MODE_GETRSAKEYS, FALSE);
        SetModeStatus(MYMODE_DECRYPTION);
        InvalidateRect(hwnd, NULL, 1);
        break;
    }
    case MYRADIOBUTTON_MODE_GETRSAKEYS:
    {
        Show_Subwindows(MYMODE_GETRSAKEYS);
        CheckDlgButton(hwnd, MYRADIOBUTTON_MODE_ENCRYPTION, FALSE);
        CheckDlgButton(hwnd, MYRADIOBUTTON_MODE_DECRYPTION, FALSE);
        CheckDlgButton(hwnd, MYRADIOBUTTON_MODE_GETRSAKEYS, TRUE);
        SetModeStatus(MYMODE_GETRSAKEYS);
        InvalidateRect(hwnd, NULL, 1);
        break;
    }
    }
}

void OnCase_ENCRYPTION(WPARAM wParam, LPARAM lParam, HWND hwnd)
{
    switch(LOWORD(wParam))
    {
    case MYPUSHBUTTON_ENCRYPTION_OPENFILE:
    {
        char path[MAX_PATH] = "";
        if(GetFilePath(OPEN_TXT, path))
        {
            SetWindowText(GetDlgItem(hwnd,MYEDIT_ENCRYPTION_OPENFILE), path);
            FILE *fp = fopen(path, "rb");
            if(fp == NULL)
                MessageBox(hwnd, _T("无法打开这个文件"), _T("错误"), MB_OK);
            else
            {
                char buffer[1000+1];
                memset(buffer, '\0', 1000+1);
                fread(buffer, sizeof(char), 1000, fp);
                SetWindowText(GetDlgItem(hwnd,MYEDIT_ENCRYPTION_PLAINTEXT), buffer);
                fclose(fp);
            }
        }
        break;
    }
    case MYPUSHBUTTON_ENCRYPTION_PUBLICKEY_OPEN:
    {
        char path[MAX_PATH] = "";
        if(GetFilePath(OPEN_TXT, path))
        {
            SetWindowText(GetDlgItem(hwnd,MYEDIT_ENCRYPTION_PUBLICKEY), path);
            FILE *fp = fopen(path, "rb");
            if(fp == NULL)
                MessageBox(hwnd, _T("无法打开这个文件"), _T("错误"), MB_OK);
        }
        break;
    }
    case MYRADIOBUTTON_ENCRYPTION_SAVETOTXT:
    {
        CheckDlgButton(hwnd, MYRADIOBUTTON_ENCRYPTION_SAVETOBMP, FALSE);
        ENCRYPTION_INFORMATION.szOrgBMP_Path[0] = '\0';
        CheckDlgButton(hwnd, MYRADIOBUTTON_ENCRYPTION_SAVETOTXT, TRUE);
        SetWindowText(GetDlgItem(hwnd,MYEDIT_ENCRYPTION_SAVETO), "");
        ShowWindow(GetDlgItem(hwnd, MYEDIT_ENCRYPTION_CIPHERTEXT), SW_SHOW);
        ENCRYPTION_INFORMATION.SaveTO_Bmp_Open = FALSE;
        InvalidateRect(hwnd, NULL, 1);
        break;
    }
    case MYRADIOBUTTON_ENCRYPTION_SAVETOBMP:
    {
        CheckDlgButton(hwnd, MYRADIOBUTTON_ENCRYPTION_SAVETOTXT, FALSE);
        CheckDlgButton(hwnd, MYRADIOBUTTON_ENCRYPTION_SAVETOBMP, TRUE);
        ENCRYPTION_INFORMATION.SaveTO_Bmp_Open = TRUE;
        MessageBox(hwnd, _T("打开一个BMP文件作为保存"), _T("提示"), MB_OK);
        char path[MAX_PATH] = "";
        if(GetFilePath(OPEN_BMP, path))
            strcpy(ENCRYPTION_INFORMATION.szOrgBMP_Path, path);
        else
        {
            MessageBox(hwnd, _T("无法打开这个文件"), _T("错误"), MB_OK);
            CheckDlgButton(hwnd, MYRADIOBUTTON_ENCRYPTION_SAVETOBMP, FALSE);
            ENCRYPTION_INFORMATION.SaveTO_Bmp_Open = FALSE;
            break;
        }
        SetWindowText(GetDlgItem(hwnd,MYEDIT_ENCRYPTION_SAVETO), "");
        InvalidateRect(hwnd, NULL, 1);
        break;
    }
    case MYPUSHBUTTON_ENCRYPTION_SAVETO:
    {
        char szPath[MAX_PATH] = "ENCRYPTION";
        OPENFILENAME file = {0};
        file.lStructSize = sizeof(file);
        file.lpstrFile = szPath;
        file.nMaxFile = MAX_PATH;
        if(IsDlgButtonChecked(hwnd, MYRADIOBUTTON_ENCRYPTION_SAVETOTXT))
        {
            file.lpstrFilter = "Text Files(*.txt, *.TXT)\0*.txt\0\0";
            file.lpstrDefExt = "txt";
        }
        else if(IsDlgButtonChecked(hwnd, MYRADIOBUTTON_ENCRYPTION_SAVETOBMP))
        {
            file.lpstrFilter = "BitMap Files(*.bmp, *.BMP)\0*.bmp\0\0";
            file.lpstrDefExt = "bmp";
        }
        else
        {
            MessageBox(hwnd, "请先选择保存类型", "错误", MB_OK);
            break;
        }

        file.nFilterIndex = 1;

        if(GetSaveFileName(&file))
            SetWindowText(GetDlgItem(hwnd,MYEDIT_ENCRYPTION_SAVETO), szPath);
        break;
    }
    case MYPUSHBUTTON_ENCRYPTION_ENCRYPT:
    {
        //解密模式
        Myhandle = CreateThread(NULL, NULL, ThreadProc, (LPVOID)MYMODE_ENCRYPTION, 0, &MyThreadId);
        break;
    }
    }
}

void OnCase_DECRYPTION(WPARAM wParam, LPARAM lParam, HWND hwnd)
{
    switch(LOWORD(wParam))
    {
    case MYPUSHBUTTON_DECRYPTION_OPENFILE_CIPHERTEXT:
    {
        char path[MAX_PATH] = "";
        if(GetFilePath(OPEN_TXTORBMP, path))
        {

            FILE *fp = fopen(path, "rb");
            if(fp == NULL)
            {
                MessageBox(hwnd, _T("无法打开这个文件"), _T("错误"), MB_OK);
                fclose(fp);
                break;
            }
            SetWindowText(GetDlgItem(hwnd,MYEDIT_DECRYPTION_OPENFILE_CIPHERTEXT), path);
            if((path[strlen(path) -1] | 32) == 't')
            {
                ShowWindow(GetDlgItem(hwnd, MYEDIT_DECRYPTION_CIPHERTEXT), SW_SHOW);
                char buffer[MY_MAX_SIZE_OF_PLAINTEXT+1];
                memset(buffer, '\0', MY_MAX_SIZE_OF_PLAINTEXT+1);
                fread(buffer, sizeof(char), MY_MAX_SIZE_OF_PLAINTEXT, fp);
                SetWindowText(GetDlgItem(hwnd,MYEDIT_DECRYPTION_CIPHERTEXT), buffer);
                DECRYPTION_INFORMATION.Cipher_Bmp_Open = FALSE;
            }
            else if((path[strlen(path)-1] | 32) == 'p')
            {
                strcpy(DECRYPTION_INFORMATION.szCipherPath, path);
                DECRYPTION_INFORMATION.Cipher_Bmp_Open = TRUE;
            }
            fclose(fp);
        }
        InvalidateRect(hwnd, NULL, 1);
        break;
    }
    case MYPUSHBUTTON_DECRYPTION_OPENFILE_PRIVATEKEY:
    {
        char path[MAX_PATH] = "";
        if(GetFilePath(OPEN_TXTORBMP, path))
        {

            FILE *fp = fopen(path, "rb");
            if(fp == NULL)
            {
                MessageBox(hwnd, _T("无法打开这个文件"), _T("错误"), MB_OK);
                fclose(fp);
                break;
            }
            SetWindowText(GetDlgItem(hwnd,MYEDIT_DECRYPTION_OPENFILE_PRIVATEKEY), path);
            if((path[strlen(path) -1] | 32) == 't')
                DECRYPTION_INFORMATION.PivKey_Bmp_Open = FALSE;
            else if((path[strlen(path)-1] | 32) == 'p')
            {
                strcpy(DECRYPTION_INFORMATION.szCipherPath, path);
                DECRYPTION_INFORMATION.PivKey_Bmp_Open = TRUE;
            }
            fclose(fp);
        }
        InvalidateRect(hwnd, NULL, 1);
        break;
    }
    case MYPUSHBUTTON_DECRYPTION_SAVETO:
    {
        char szPath[MAX_PATH] = "DECRYPTION";
        OPENFILENAME file = {0};
        file.lStructSize = sizeof(file);
        file.lpstrFile = szPath;
        file.nMaxFile = MAX_PATH;
        file.lpstrFilter = "Text Files(*.txt, *.TXT)\0*.txt\0\0";
        file.lpstrDefExt = "txt";
        if(GetSaveFileName(&file))
            SetWindowText(GetDlgItem(hwnd,MYEDIT_DECRYPTION_SAVETO), szPath);
        break;
    }
    case MYPUSHBUTTON_DECRYPTION_DECRYPT:
    {
        Myhandle = CreateThread(NULL, NULL, ThreadProc, (LPVOID)MYMODE_DECRYPTION, 0, &MyThreadId);
        break;
    }
    }
}

void OnCase_GETRSAKEYS(WPARAM wParam, LPARAM lParam, HWND hwnd)
{
    switch(LOWORD(wParam))
    {
    case MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOTXT:
    {
        //更新按钮状态并清空之前输入的路径
        CheckDlgButton(hwnd, MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOBMP, FALSE);
        GETRSAKEYS_INFORMATION.szBmpPath[0] = '\0';
        CheckDlgButton(hwnd, MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOTXT, TRUE);
        SetWindowText(GetDlgItem(hwnd,MYEDIT_GETRSAKEYS_PRVKEY_SAVETO), "");

        //ShowWindow(GetDlgItem(hwnd, MYEDIT_GETRSAKEYS_PRIVATEKEY_TEXT), SW_SHOW);
        GETRSAKEYS_INFORMATION.Bmp_Open = FALSE;
        //使窗口无效，触发WM_PAINT消息，重绘窗口
        InvalidateRect(hwnd, NULL, 1);
        break;
    }
    case MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOBMP:
    {
        //更新按钮状态并清空之前输入的路径
        CheckDlgButton(hwnd, MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOTXT, FALSE);
        CheckDlgButton(hwnd, MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOBMP, TRUE);
        GETRSAKEYS_INFORMATION.Bmp_Open = TRUE;
        //要求打开一个模板BMP文件
        MessageBox(hwnd, _T("打开一个BMP文件作为保存"), _T("提示"), MB_OK);
        char path[MAX_PATH] = "";
        if(GetFilePath(OPEN_BMP, path))
            strcpy(GETRSAKEYS_INFORMATION.szBmpPath, path);
        else
        {
            MessageBox(hwnd, _T("无法打开这个文件"), _T("错误"), MB_OK);
            CheckDlgButton(hwnd, MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOBMP, FALSE);
            GETRSAKEYS_INFORMATION.Bmp_Open = FALSE;
            break;
        }
        SetWindowText(GetDlgItem(hwnd,MYEDIT_GETRSAKEYS_PRVKEY_SAVETO), "");
        //使窗口无效，触发WM_PAINT消息，重绘窗口
        InvalidateRect(hwnd, NULL, 1);
        break;
    }
    case MYPUSHBUTTON_GETRSAKEYS_PRVKEY_SAVETO:
    {
        //设置私钥保存路径
        char szPath[MAX_PATH] = "PRIVATE_KEY";
        OPENFILENAME file = {0};
        file.lStructSize = sizeof(file);
        file.lpstrFile = szPath;
        file.nMaxFile = MAX_PATH;
        if(IsDlgButtonChecked(hwnd, MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOTXT))
        {
            file.lpstrFilter = "Text Files(*.txt, *.TXT)\0*.txt\0\0";
            file.lpstrDefExt = "txt";
        }
        else if(IsDlgButtonChecked(hwnd, MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOBMP))
        {
            file.lpstrFilter = "BitMap Files(*.bmp, *.BMP)\0*.bmp\0\0";
            file.lpstrDefExt = "bmp";
        }
        else
        {
            MessageBox(hwnd, "请先选择私钥保存类型", "错误", MB_OK);
            break;
        }
        if(GetSaveFileName(&file))
            SetWindowText(GetDlgItem(hwnd,MYEDIT_GETRSAKEYS_PRVKEY_SAVETO), szPath);
        break;
    }
    case MYPUSHBUTTON_GETRSAKEYS_PUBKEY_SAVETO:
    {
        //设置公钥保存路径
        char szPath[MAX_PATH] = "PUBLIC_KEY";
        OPENFILENAME file = {0};
        file.lStructSize = sizeof(file);
        file.lpstrFile = szPath;
        file.nMaxFile = MAX_PATH;
        file.lpstrFilter = "Text Files(*.txt, *.TXT)\0*.txt\0\0";
        file.lpstrDefExt = "txt";
        if(GetSaveFileName(&file))
            SetWindowText(GetDlgItem(hwnd,MYEDIT_GETRSAKEYS_PUBKEY_SAVETO), szPath);
        break;
    }
    case MYPUSHBUTTON_GETRSAKEYS_GETKEY:
    {
        //生成公钥与私钥
        Myhandle = CreateThread(NULL, NULL, ThreadProc, (LPVOID)MYMODE_GETRSAKEYS, 0, &MyThreadId);
        break;
    }
    }
}


DWORD WINAPI ThreadProc(LPVOID mode)
{
    int MODE = (int)mode;
    switch(MODE)
    {
    case MYMODE_ENCRYPTION:
    {
        if(!CheckBeforeEncrypt())
            break;
        if(!Encrypt(hwnd))
        {
            MessageBox(hwnd, "无法加密该文件，请确认各文件的状态正常及符合要求", "错误",MB_OK);
            break;
        }
        MessageBox(hwnd, "完成！", "提示", MB_OK);
        break;
    }
    case MYMODE_DECRYPTION:
    {
        if(!CheckBeforeDecrypt())
        {
            MessageBox(hwnd, "请检查各文件是否正确！", "错误", MB_OK);
            break;
        }
        if(!Decrypt(hwnd))
        {
            MessageBox(hwnd, "解密失败，请检查各文件是否正确！", "错误", MB_OK);
            break;
        }
        MessageBox(hwnd, "完成！", "提示", MB_OK);
        break;
    }
    case MYMODE_GETRSAKEYS:
    {
        if(!CheckBeforeGetRSAKeys(hwnd))
            break;
        if(!GetRSAKeys(hwnd))
        {
            MessageBox(hwnd, "生成失败，请检查各路径是否正确", "错误", MB_OK);
            break;
        }
        MessageBox(hwnd, "完成！", "提示", MB_OK);
        break;
    }
    }
    return TRUE;
}

BOOL CheckBeforeEncrypt()
{
    FILE *fp_Plain, *fp_OrgBMP, *fp_Save, *fp_Public;
    char temp[MAX_PATH], *pstrHead, *pstrRear;
    int i = 0;
    //检查公钥文件
    GetWindowText(GetDlgItem(hwnd,MYEDIT_ENCRYPTION_PUBLICKEY),
                  ENCRYPTION_INFORMATION.szPublicKeyPath, MAX_PATH);

    if((fp_Public = fopen(ENCRYPTION_INFORMATION.szPublicKeyPath, "rb")) == NULL)
    {
        MessageBox(hwnd, "无法打开公钥文件于指定目录", "错误",MB_OK);
        fclose(fp_Public);
        return FALSE;
    }
    fclose(fp_Public);
    //检查明文文件
    GetWindowText(GetDlgItem(hwnd,MYEDIT_ENCRYPTION_OPENFILE),
                  ENCRYPTION_INFORMATION.szPlaintextPath, MAX_PATH);
    if((fp_Plain = fopen(ENCRYPTION_INFORMATION.szPlaintextPath, "rb")) == NULL)
    {
        MessageBox(hwnd, "无法打开明文文件，请检查路径是否正确，并确保文件存在并未被占用", "错误",MB_OK);
        fclose(fp_Plain);
        return FALSE;
    }
    fclose(fp_Plain);


    //检查密文保存路径
    GetWindowText(GetDlgItem(hwnd,MYEDIT_ENCRYPTION_SAVETO),
                  ENCRYPTION_INFORMATION.szSaveToPath, MAX_PATH);
    if((fp_Save = fopen(ENCRYPTION_INFORMATION.szSaveToPath, "wb")) == NULL)
    {
        MessageBox(hwnd, "无法保存密文文件于指定目录", "错误",MB_OK);
        fclose(fp_Save);
        return FALSE;
    }
    fclose(fp_Save);


    //检查BMP文件
    if(IsDlgButtonChecked(hwnd, MYRADIOBUTTON_ENCRYPTION_SAVETOBMP))
    {
        if((fp_OrgBMP = fopen(ENCRYPTION_INFORMATION.szOrgBMP_Path, "rb")) == NULL)
        {
            MessageBox(hwnd, "无法打开BMP文件，请检查路径是否正确，并确保文件存在并未被占用", "错误",MB_OK);
            fclose(fp_OrgBMP);
            return FALSE;
        }
        fclose(fp_OrgBMP);
        //检查原BMP文件和要保存的BMP文件是否在同一路径
            //去除前导空格和后导空格，方便比较
        pstrHead = pstrRear = ENCRYPTION_INFORMATION.szSaveToPath;
        while((*pstrRear++) != 0);
        pstrRear--;
        while((*--pstrRear) == ' ');
        pstrRear++;
        while((*pstrHead++) == ' ');
        pstrHead--;
        while(pstrHead != pstrRear)
            temp[i++] = *pstrHead++;
        temp[i] = 0;


        if(strcmp(ENCRYPTION_INFORMATION.szOrgBMP_Path,
              temp) == 0)
        {
            MessageBox(hwnd, "生成的密文BMP文件会覆盖原本的BMP文件！", "错误",MB_OK);
            return FALSE;
        }
    }
    else if(!IsDlgButtonChecked(hwnd, MYRADIOBUTTON_ENCRYPTION_SAVETOTXT))
    {
        MessageBox(hwnd, "请选择密文保存方式", "错误",MB_OK);
        return FALSE;
    }
    return TRUE;
}

BOOL Encrypt(HWND hwnd)
{
    FILE *fp_Plain, *fp_Public, *fp_Save;
    unsigned char buffer[MY_MAX_SIZE_OF_PLAINTEXT] = {0};
    char bufferPublicKey[LEN*2];
    char N[LEN*2];
    char *CipherText;
    int count, i = 0;
    PublicKey = (_u8 **)malloc(2 * sizeof(_u8 *));
    PublicKey[0] = Ini();
    PublicKey[1] = Ini();

    fp_Plain = fopen(ENCRYPTION_INFORMATION.szPlaintextPath, "rb");
    fp_Public = fopen(ENCRYPTION_INFORMATION.szPublicKeyPath, "r");

    fread(bufferPublicKey, sizeof(char),LEN*2, fp_Public);
    fscanf(fp_Public, "%s ", N);
    fread(N, sizeof(char), LEN*2, fp_Public);


    WriteToNumber(bufferPublicKey, PublicKey[0]);
    WriteToNumber(N, PublicKey[1]);

    while((count = fread(buffer, sizeof(char), MY_MAX_SIZE_OF_PLAINTEXT, fp_Plain)) == MY_MAX_SIZE_OF_PLAINTEXT)
        i++;
    i = (count == MY_MAX_SIZE_OF_PLAINTEXT)?(i):(i+1);
    CipherText = (char*)malloc(sizeof(char)*(i*LEN*2));
    fclose(fp_Plain);

    i = 0;
    fp_Plain = fopen(ENCRYPTION_INFORMATION.szPlaintextPath, "rb");
    while((count = fread(buffer, sizeof(char), MY_MAX_SIZE_OF_PLAINTEXT, fp_Plain)) == MY_MAX_SIZE_OF_PLAINTEXT)
    {
        _u8 *tempu8 = PM(buffer);
        WriteToText(tempu8, CipherText+i*LEN*2);
        i++;
    }
    for(int i = count; i < MY_MAX_SIZE_OF_PLAINTEXT; i++)
        buffer[i] = 0;
    _u8 *temp = PM(buffer);

    WriteToText(temp, CipherText+i*LEN*2);
    i = (count == MY_MAX_SIZE_OF_PLAINTEXT)?(i):(i+1);

    if(IsDlgButtonChecked(hwnd, MYRADIOBUTTON_ENCRYPTION_SAVETOBMP))
    {
        //如果选择把密文保存到BMP文件中
        //则把私钥保存到BMP文件中
        SaveToBmp(ENCRYPTION_INFORMATION.szOrgBMP_Path, ENCRYPTION_INFORMATION.szSaveToPath,
        CipherText, i*LEN*2);
    }
    else
    {
        //密文保存到TXT文件中
        fp_Save = fopen(ENCRYPTION_INFORMATION.szSaveToPath, "wb");
        fwrite(CipherText, sizeof(char), i*LEN*2, fp_Save);
        fclose(fp_Save);
        CipherText[LEN*2] = 0;
        SetWindowText(GetDlgItem(hwnd,MYEDIT_ENCRYPTION_CIPHERTEXT), CipherText);
    }


    destroy(PublicKey[0]);
    destroy(PublicKey[1]);
    fclose(fp_Plain);
    fclose(fp_Public);
    return TRUE;
}


BOOL CheckBeforeDecrypt()
{
    FILE *fp_Cipher, *fp_PriKey, *fp_szSave;

    GetWindowText(GetDlgItem(hwnd,MYEDIT_DECRYPTION_OPENFILE_CIPHERTEXT),
                  DECRYPTION_INFORMATION.szCipherPath, MAX_PATH);
    if((fp_Cipher = fopen(DECRYPTION_INFORMATION.szCipherPath, "rb")) == NULL)
    {
        MessageBox(hwnd, "无法打开密文文件", "错误",MB_OK);
        fclose(fp_Cipher);
        return FALSE;
    }

    GetWindowText(GetDlgItem(hwnd,MYEDIT_DECRYPTION_OPENFILE_PRIVATEKEY),
                  DECRYPTION_INFORMATION.szPrivateKeyPath, MAX_PATH);
    if((fp_PriKey = fopen(DECRYPTION_INFORMATION.szPrivateKeyPath, "rb")) == NULL)
    {
        MessageBox(hwnd, "无法打开私钥文件", "错误",MB_OK);
        fclose(fp_PriKey);
        return FALSE;
    }
    GetWindowText(GetDlgItem(hwnd,MYEDIT_DECRYPTION_SAVETO),
                  DECRYPTION_INFORMATION.szSave, MAX_PATH);
    if((fp_szSave = fopen(DECRYPTION_INFORMATION.szSave, "wb")) == NULL)
    {
        MessageBox(hwnd, "无法保存解密后的文件于指定路径", "错误",MB_OK);
        fclose(fp_szSave);
        return FALSE;
    }
    return TRUE;

}

BOOL Decrypt(HWND hwnd)
{
    FILE *fp_Cipher, *fp_PriKey, *fp_szSave;
    char bufferCipher[LEN*2] = {0};
    char bufferEdit[1000];
    char *tempSave;
    _u8 *temp = Ini();
    char bufferPrivateKey[LEN*2+1];
    char N[LEN*2+1];
    char ch, flag = 0;
    int count = 0;
    PrivateKey = (_u8 **)malloc(2 * sizeof(_u8 *));
    PrivateKey[0] = Ini();
    PrivateKey[1] = Ini();
    INT32 start, end;

    //读取私钥
    if((DECRYPTION_INFORMATION.szPrivateKeyPath[strlen(DECRYPTION_INFORMATION.szPrivateKeyPath) -1] | 32) == 'p')
    {
        if(!ReadfromBmp(DECRYPTION_INFORMATION.szPrivateKeyPath, &start, &end))
        {
            destroy(temp);
            destroy(PrivateKey[0]);
            destroy(PrivateKey[1]);
            MessageBox(hwnd, "私钥文件错误或者文件已损坏", "错误", MB_OK);
            return FALSE;
        }
        fp_PriKey = fopen(DECRYPTION_INFORMATION.szPrivateKeyPath, "rb");
        fseek(fp_PriKey, start+3, SEEK_SET);
    }
    else
        fp_PriKey = fopen(DECRYPTION_INFORMATION.szPrivateKeyPath, "rb");


    fread(bufferPrivateKey, sizeof(char), LEN*2, fp_PriKey);
    fscanf(fp_PriKey, "%c", &ch);
    fread(N, sizeof(char), LEN*2, fp_PriKey);
    fclose(fp_PriKey);
    WriteToNumber(bufferPrivateKey, PrivateKey[0]);
    WriteToNumber(N, PrivateKey[1]);


    if((DECRYPTION_INFORMATION.szCipherPath[strlen(DECRYPTION_INFORMATION.szCipherPath) -1] | 32) == 'p')
    {
        if(!ReadfromBmp(DECRYPTION_INFORMATION.szCipherPath, &start, &end))
        {
            destroy(temp);
            destroy(PrivateKey[0]);
            destroy(PrivateKey[1]);
            MessageBox(hwnd, "密文文件错误或者文件已损坏", "错误", MB_OK);
            return FALSE;
        }
        fp_Cipher = fopen(DECRYPTION_INFORMATION.szCipherPath, "rb");
        fseek(fp_Cipher, start+3, SEEK_SET);
        flag = 1;
    }
    else
        fp_Cipher = fopen(DECRYPTION_INFORMATION.szCipherPath, "rb");

    fp_szSave = fopen(DECRYPTION_INFORMATION.szSave, "wb");
    while(fread(bufferCipher, sizeof(char), LEN*2, fp_Cipher) == LEN*2)
    {
        if(flag == 1)
            count += LEN*2;
        WriteToNumber(bufferCipher, temp);
        tempSave = SC(temp);
        fwrite(tempSave, sizeof(char), MY_MAX_SIZE_OF_PLAINTEXT, fp_szSave);
        if(flag == 1 && count + start + 6 >= end)
            break;
    }
    fclose(fp_szSave);


    fp_szSave = fopen(DECRYPTION_INFORMATION.szSave, "rb");
    fread(bufferEdit, sizeof(char), 1000, fp_szSave);
    SetWindowText(GetDlgItem(hwnd,MYEDIT_DECRYPTION_PLAINTEXT), bufferEdit);

    fclose(fp_Cipher);
    fclose(fp_szSave);
    destroy(temp);
    destroy(PrivateKey[0]);
    destroy(PrivateKey[1]);
    return TRUE;
}

BOOL CheckBeforeGetRSAKeys(HWND hwnd)
{
     FILE *fp_OrgBMP, *fp_Public_Save, *fp_Private_save;
     char temp[MAX_PATH], *pstrHead, *pstrRear;
     int i = 0;
    //检查公钥的保存路径
    GetWindowText(GetDlgItem(hwnd,MYEDIT_GETRSAKEYS_PUBKEY_SAVETO),
                  GETRSAKEYS_INFORMATION.szPubKeySaveto_Path, MAX_PATH);
    if((fp_Public_Save = fopen(GETRSAKEYS_INFORMATION.szPubKeySaveto_Path, "wb")) == NULL)
    {
        MessageBox(hwnd, "无法保存公钥文件于指定目录", "错误",MB_OK);
        fclose(fp_Public_Save);
        return FALSE;
    }
    fclose(fp_Public_Save);

    //检查私钥的保存路径
    GetWindowText(GetDlgItem(hwnd,MYEDIT_GETRSAKEYS_PRVKEY_SAVETO),
                  GETRSAKEYS_INFORMATION.szPrvKeySaveto_Path, MAX_PATH);
    if((fp_Private_save = fopen(GETRSAKEYS_INFORMATION.szPrvKeySaveto_Path, "wb")) == NULL)
    {
        MessageBox(hwnd, "无法保存私钥文件于指定目录", "错误",MB_OK);
        fclose(fp_Private_save);
        return FALSE;
    }
    fclose(fp_Private_save);

    //检查BMP文件
        //如果选择保存在BMP文件中，检查BMP文件
    if(IsDlgButtonChecked(hwnd, MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOBMP))
    {
        if((fp_OrgBMP = fopen(GETRSAKEYS_INFORMATION.szBmpPath, "rb")) == NULL)
        {
            MessageBox(hwnd, "无法打开BMP文件，请检查路径是否正确，并确保文件存在并未被占用", "错误",MB_OK);
            fclose(fp_OrgBMP);
            return FALSE;
        }
        fclose(fp_OrgBMP);
        //检查原BMP文件和要保存的BMP文件是否在同一路径
            //去除前导空格和后导空格，方便比较
        pstrHead = pstrRear = GETRSAKEYS_INFORMATION.szPrvKeySaveto_Path;
        while((*pstrRear++) != 0);
        pstrRear--;
        while((*--pstrRear) == ' ');
        pstrRear++;
        while((*pstrHead++) == ' ');
        pstrHead--;
        while(pstrHead != pstrRear)
            temp[i++] = *pstrHead++;
        temp[i] = 0;
        if(strcmp(GETRSAKEYS_INFORMATION.szBmpPath,
              temp) == 0)
        {
            MessageBox(hwnd, "生成的密文BMP文件会覆盖原本的BMP文件！", "错误",MB_OK);
            return FALSE;
        }
    }
    else if(!IsDlgButtonChecked(hwnd, MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOTXT))
    {
        MessageBox(hwnd, "请选择密文保存方式", "错误",MB_OK);
        return FALSE;
    }

    return TRUE;

}

BOOL GetRSAKeys(HWND hwnd)
{
    FILE *fp_Public_Save, *fp_Private_save;
    char szPublicKey[LEN*2+1];
    char szPrivateKey[LEN*2+1];
    char szPublicKeyToEdit[LEN*2+1+LEN*2+1];
    char szPivateKeyToEdit[LEN*2+1+LEN*2+1];
    char szN[LEN*2 +1];
    char testTxtBefore[5] = "test";
    char *testTxtAfter;
    int i;

    //打开必要文件
    fp_Public_Save = fopen(GETRSAKEYS_INFORMATION.szPubKeySaveto_Path, "wb");

    //生成公钥和私钥
    P = MRFakePrime();
	Q = imirPefaFRM(P);
	CreatePublic();
	CreatePrivate();
	//将二进制内容转化为ASCII码十六进制表示（小端法）
    WriteToText(PublicKey[0], szPublicKey);
    WriteToText(PrivateKey[0], szPrivateKey);
    WriteToText(PublicKey[1], szN);

    //测试生成的公钥和私钥是否可用
    _u8 *temp = PM(testTxtBefore);
    testTxtAfter = SC(temp);
    //不可用（即加密后的内容与解密后的内容不同）则返回错误
    if(strcmp(testTxtAfter, testTxtBefore) != 0)
        return FALSE;

    //设置打印到EDIT控件里的字符串
    for( i = 0; i < LEN*2; i++)
        szPublicKeyToEdit[i] = szPublicKey[i];
    szPublicKeyToEdit[i] = '\n';
    for( i = 0; i < LEN*2; i++)
        szPublicKeyToEdit[i+LEN*2+1] = szN[i];
    szPublicKeyToEdit[i+LEN*2+1] = '\0';
    //将公钥打印到屏幕上
    SetWindowText(GetDlgItem(hwnd,MYEDIT_GETRSAKEYS_PUBLICKEY_TEXT), szPublicKeyToEdit);
    //将公钥保存到文件中
    fprintf(fp_Public_Save, "%s\n%s\n", szPublicKey, szN);
    //设置打印到EDIT控件或者BMP文件里的字符串
    for( i = 0; i < LEN*2; i++)
        szPivateKeyToEdit[i] = szPrivateKey[i];
        szPivateKeyToEdit[i] = '\n';
    for( i = 0; i < LEN*2; i++)
        szPivateKeyToEdit[i+LEN*2+1] = szN[i];
        szPivateKeyToEdit[i+LEN*2+1] = '\0';
    if(IsDlgButtonChecked(hwnd, MYRADIOBUTTON_GETRSAKEYS_PRVSAVETOBMP))
    {
        //如果选择把私钥保存到BMP文件中
        //则把私钥保存到BMP文件中
        SaveToBmp(GETRSAKEYS_INFORMATION.szBmpPath, GETRSAKEYS_INFORMATION.szPrvKeySaveto_Path,
        szPivateKeyToEdit, (LEN*2+1)*2);
    }
    else
    {
        //如果选择把私钥保存到TXT文件中
        fp_Private_save = fopen(GETRSAKEYS_INFORMATION.szPrvKeySaveto_Path, "wb");
        fprintf(fp_Private_save, "%s\n%s\n", szPrivateKey, szN);
        fclose(fp_Private_save);
        SetWindowText(GetDlgItem(hwnd,MYEDIT_GETRSAKEYS_PRIVATEKEY_TEXT), szPivateKeyToEdit);
    }
    Destory();
    fclose(fp_Public_Save);
    return TRUE;
}

void WriteToText(_u8* Number, char* String)
{
    int i ,j, num;
    for(i = 0, j = 0; i < Number->len; i++)
    {
        num = (Number->data[i] & 0xf0) >> 4;
        if(num < 10)
            String[j++] = num + '0';
        else
            String[j++] = num - 10 + 'A';
        num = (Number->data[i] & 0x0f);
        if(num < 10)
            String[j++] = num + '0';
        else
            String[j++] = num - 10 + 'A';
    }
    String[j] = 0;

}


void WriteToNumber(char* String, _u8* Number)
{
    int i, j;
    char ch;

    for(i = 0, j = 0; i < Number->len; i++)
    {
        Number->data[i] = 0;
        ch = String[j++];
        if(ch <= 'F' && ch >= 'A')
            Number->data[i] += (ch-'A'+10) << 4;
        else
            Number->data[i] += (ch-'0') << 4;
        ch = String[j++];
        if(ch <= 'F' && ch >= 'A')
            Number->data[i] += (ch-'A'+10);
        else
            Number->data[i] += (ch-'0');

    }
}
