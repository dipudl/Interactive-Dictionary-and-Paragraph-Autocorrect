#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <wchar.h>

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
//#define CHANGE_TITLE 20
#define GENERATE_BUTTON 4
#define BLIND_BUTTON 5
#define OPEN_FILE_BUTTON 6
#define AUTO_CORRECT_BUTTON 7
#define SAVE_FILE_BUTTON 8
#define CHECK_BUTTON 9
#define CANCEL_BUTTON 10
#define HELP_MENU 11
#define ABOUT_US 12

struct spellStore{
    char word[100];
    int position;
}SS[50];

LRESULT CALLBACK WindowProcedure(HWND,UINT,WPARAM,LPARAM);

void AddMenus(HWND);
void AddControls(HWND);
void loadImages();
void tts(char*);
void tts2(char*);
void tts3(char*);
void dictionary(HWND);
void open_file(HWND);
void display_file(char*);
void save_file(HWND);
void Auto_Correct(HWND);
void write_file(char* path);
void ExtChar(char*);
void UnformTxt(char*);
void ImproCase(char*);
void spellCorr(char*,HWND);
int checkNum(char*);
void dialogBoxClass(HINSTANCE);
void displayDialogBox(HWND);
int CheckWord(char*);
void displayStaticText(HWND);

HWND hName,hAge,hOut,hLogo,hBack,hTest,hTTS,hAutoText,hWord,hCancelButton,hStaticDialog;
HMENU hMenu,testMenu;
HBITMAP hLogoImage,hBackImage;
HWND hEdit;

void hideConsole()
{
    HWND hide;
    AllocConsole();
    hide = FindWindowA("ConsoleWindowClass",NULL);
    ShowWindow(hide,0);
}
int blindButton = 0;
int structCount=0,structIndex=0;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    //hideConsole();

    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = WindowProcedure;
    wc.hIcon = LoadIcon(NULL,IDI_INFORMATION);

    if (!RegisterClassW(&wc))
        return -1;

    dialogBoxClass(hInst);

    CreateWindowW(L"myWindowClass",L"Interactive Dictionary & Paragraph Auto-Correct",WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                  100,100,955,500,NULL,NULL,NULL,NULL);
    MSG msg = {0};
    while(GetMessage(&msg,NULL,NULL,NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }

    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)
{
    int val;
    wchar_t helpMsg[300] = L"\"Interactive Dictionary & Paragraph Auto-correct\" allows you to explore through English words using Dictionary(on the left) and correct your writing mistakes using Paragraph Auto-correct(at the right).\n\nYou can go deep into its features to become more familiar with it.\nThank You!";
    wchar_t aboutMsg[500] = L"\"Interactive Dictionary & Paragraph Auto-correct\" was made by Dipu Dahal, Anupam Bhattarai & Jagadish Shrestha and submitted to Department of Electronics and Computer Engineering, Thapathali Campus, Kathmandu, Nepal as the C programming project of first semester on March, 2020.\n\nFor any query, email at:\ndipu234dahal@gmail.com";

    switch(msg)
    {
    case WM_COMMAND:

        switch(wp)
        {
        case OPEN_FILE_BUTTON:
            open_file(hWnd);
            break;
        case SAVE_FILE_BUTTON:
            save_file(hWnd);
            break;

        case FILE_MENU_EXIT:
            val = MessageBoxW(hWnd,L"Are you sure?",L"Wait!",MB_YESNO | MB_ICONEXCLAMATION);

            if(val==IDYES)
            {
                DestroyWindow(hWnd);
                //system("mshta vbscript:Execute(\"CreateObject(\"\"SAPI.SpVoice\"\").Speak(\"\"Hello\"\")(window.close)\")");
                //system("cscript say.vbs \"Thanks for operating our program.\"");
                //system("PowerShell -Command \"Add-Type –AssemblyName System.Speech; (New-Object System.Speech.Synthesis.SpeechSynthesizer).Speak('Thanks for operating our program.');\"");
            }
            break;

        case FILE_MENU_NEW:
            SetWindowTextW(hName,L"");
            SetWindowTextW(hOut,L"");
            SetWindowTextW(hAutoText,L"");
            MessageBeep(MB_ICONSTOP);
            break;

        case HELP_MENU:
            MessageBoxW(hWnd,helpMsg,L"Help",MB_OK | MB_ICONINFORMATION);
            break;

        case ABOUT_US:
            MessageBoxW(hWnd,aboutMsg,L"About Us",MB_OK | MB_ICONINFORMATION);
            break;
        /*case CHANGE_TITLE:
            wchar_t text[100];
            GetWindowTextW(hEdit,text,100);
            SetWindowTextW(hWnd,text);
            break;*/
        case BLIND_BUTTON:
            blindButton++;

            if(blindButton%2 == 1)
            {
                SetWindowTextW(hTTS,L"TTS : on");
                tts("Text-to-speech ON");
            }
            else
            {
               SetWindowTextW(hTTS,L"TTS : off");
            }

            break;


        case GENERATE_BUTTON:
            dictionary(hWnd);
            break;

        case AUTO_CORRECT_BUTTON:
            Auto_Correct(hWnd);
            break;
        }
        break;

    case WM_CREATE:
        loadImages();
        AddMenus(hWnd);
        AddControls(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hWnd,msg,wp,lp);
    }
}

void AddMenus(HWND hWnd)
{
    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    HMENU hSubMenu = CreateMenu();

    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFileMenu,"File");

    AppendMenu(hFileMenu,MF_STRING,FILE_MENU_NEW,"New");
    AppendMenu(hFileMenu,MF_SEPARATOR,NULL,NULL);

    //AppendMenu(hFileMenu,MF_POPUP,(UINT_PTR)hSubMenu,"Open");
    //AppendMenu(hSubMenu,MF_STRING,ABOUT_US,"Sub Menu");
    //AppendMenu(hFileMenu,MF_SEPARATOR,NULL,NULL);

    AppendMenu(hFileMenu,MF_STRING,FILE_MENU_EXIT,"Exit");


    AppendMenu(hMenu,MF_STRING,HELP_MENU,"Help");
    AppendMenu(hMenu,MF_STRING,ABOUT_US,"About");

    SetMenu(hWnd,hMenu);
}
void AddControls(HWND hWnd)
{
    hBack = CreateWindowW(L"Static",NULL,WS_VISIBLE | WS_CHILD | SS_BITMAP,0,0,700,300,hWnd,NULL,NULL,NULL);
    SendMessageW(hBack,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM) hBackImage);

    /*CreateWindowW(L"button",L"Change Title",WS_VISIBLE | WS_CHILD | WS_BORDER,200,190,100,40,
                  hWnd,(HMENU)CHANGE_TITLE,NULL,NULL);*/

    CreateWindowW(L"Static",L"Search:",WS_VISIBLE | WS_CHILD,50,50,50,17,hWnd,NULL,NULL,NULL);
    hName = CreateWindowW(L"Edit",L"",WS_VISIBLE | WS_CHILD | WS_BORDER,50,75,170,40,hWnd,NULL,NULL,NULL);

    CreateWindowW(L"Button",L"Generate",WS_VISIBLE | WS_CHILD | WS_BORDER,50,125,98,38,hWnd,(HMENU)GENERATE_BUTTON,NULL,NULL);
    hTTS = CreateWindowW(L"Button",L"TTS : off",WS_VISIBLE | WS_CHILD | WS_BORDER,315,25,75,32,hWnd,(HMENU)BLIND_BUTTON,NULL,NULL);

    hOut = CreateWindowW(L"Edit",L"",WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL,50,200,340,190,hWnd,NULL,NULL,NULL);

    hLogo = CreateWindowW(L"Static",NULL,WS_VISIBLE | WS_CHILD | SS_BITMAP,240,75,100,100,hWnd,NULL,NULL,NULL);
    SendMessageW(hLogo,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM) hLogoImage);

    CreateWindowW(L"Static",NULL,WS_VISIBLE | WS_CHILD | SS_ETCHEDVERT,445,10,0,420,hWnd,NULL,NULL,NULL);

    CreateWindowW(L"Button",L"Open File",WS_VISIBLE | WS_CHILD | WS_BORDER,500,25,95,34,hWnd,(HMENU)OPEN_FILE_BUTTON,NULL,NULL);

    CreateWindowW(L"Button",L"Auto-Correct",WS_VISIBLE | WS_CHILD | WS_BORDER,610,25,95,34,hWnd,(HMENU)AUTO_CORRECT_BUTTON,NULL,NULL);

    CreateWindowW(L"Button",L"Save File",WS_VISIBLE | WS_CHILD | WS_BORDER,720,25,95,34,hWnd,(HMENU)SAVE_FILE_BUTTON,NULL,NULL);

    hAutoText = CreateWindowW(L"Edit",L"",WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL ,500,75,392,315,hWnd,
                         NULL,NULL,NULL);
}

void loadImages()
{
    hLogoImage = (HBITMAP)LoadImageW(NULL,L"Logo.bmp",IMAGE_BITMAP,150,100,LR_LOADFROMFILE);
    hBackImage = (HBITMAP)LoadImageW(NULL,L"Back.bmp",IMAGE_BITMAP,940,443,LR_LOADFROMFILE);
    /*if (hLogoImage == NULL)
    {
        MessageBoxW(NULL,L"LoadImage Failed",L"ERROR",MB_OK);
    }*/
}
void tts(char *out)
{
    char speakCmd1[1000] = "mshta vbscript:Execute(\"CreateObject(\"\"SAPI.SpVoice\"\").Speak(\"\"";
    char speakCmd2[200] = "\"\")(window.close)\")";

    strcat(speakCmd1,out);
    strcat(speakCmd1,speakCmd2);

    system(speakCmd1);
}
void tts2(char *out)
{
    char speakCmd1[500] = "\"cscript say.vbs \"";
    char speakCmd2[200] = "\"\"";

    strcat(speakCmd1,out);
    strcat(speakCmd1,speakCmd2);

    system(speakCmd1);
}
void tts3(char *out)
{
    char speakCmd1[500] = "\"PowerShell -Command \"Add-Type –AssemblyName System.Speech; (New-Object System.Speech.Synthesis.SpeechSynthesizer).Speak('";
    char speakCmd2[200] = "');\"\"";

    strcat(speakCmd1,out);
    strcat(speakCmd1,speakCmd2);

    system(speakCmd1);
}

void open_file(HWND hWnd)
{
    OPENFILENAME ofn;
    char file_name[200];

    ZeroMemory(&ofn,sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = file_name;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 200;
    ofn.lpstrFilter = "All files\0*.*\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;

    GetOpenFileName(&ofn);

    display_file(ofn.lpstrFile);
}

void display_file(char* path)
{
    FILE *file;
    file = fopen(path,"rb");

    fseek(file,0,SEEK_END);
    int size = ftell(file);
    rewind(file);

    char *data = malloc(size+1);
    fread(data,size,1,file);
    data[size] = '\0';

    SetWindowText(hAutoText,data);
    fclose(file);
    free(data);
}

void save_file(HWND hWnd)
{
    OPENFILENAME ofn;
    char file_name[200];

    ZeroMemory(&ofn,sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = file_name;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 200;
    ofn.lpstrFilter = "All files\0*.*\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;

    GetSaveFileName(&ofn);

    write_file(ofn.lpstrFile);
}

void write_file(char* path)
{
    FILE *file;
    file = fopen(path,"w");

    int size = GetWindowTextLength(hAutoText);
    char data[size+1];
    GetWindowText(hAutoText,data,size+1);

    fwrite(data,size+1,1,file);
    fclose(file);
}

void dictionary(HWND hWnd)
{
    char name[30],name2[30],search[32],out[5000],out2[5000],out3[5000];
    int i=0,flag=1,flag2=1,cL=430,val;

    GetWindowText(hName,name,30);
    GetWindowText(hName,out,500);
    strcpy(name2,name);
    strlwr(name);

    if(strcmp(name,"")==0)
    {
        val = MessageBoxW(hWnd,L"You did not enter anything !",NULL,MB_RETRYCANCEL | MB_ICONERROR);
    }

    switch(val)
    {
        case IDCANCEL:
            DestroyWindow(hWnd);
            break;

        case IDRETRY:
            return;
    }

    if(strcmp(name,"")==0)
        return;

    FILE *file = fopen("data.json","rb");

    fseek(file,0,SEEK_END);
    int size = ftell(file);
    rewind(file);

    char *str = malloc(size+1);
    fread(str,size,1,file);
    str[size] = '\0';

    x:
    strcpy(search,"\"");
    strcat(search,name);
    strcat(search,"\"");

    char *ptr = strstr(str,search);

    if(ptr==NULL)
    {
        if(strcmp(name,name2)==0)
        {
            if(name[0]>='a' && name[0] <='z' && flag!=0)
            {
                name[0] = name[0] - 32;
                flag=0;
                goto x;
            }
            else if(flag2!=0)
            {
                strupr(name);
                flag2=0;
                goto x;
            }
            strcpy(out2,"Word Not Found. Please enter the correct word.");
            strcpy(out3,out2);
        }
        else
        {
            strcpy(name,name2);
            goto x;
        }
    }
    else
    {
        ptr += strlen(search)+3;
        int co=0;
        while(*(ptr)!=']')
        {
            if(*ptr=='[')
                out[i] = ' ';
            else if(*ptr=='\"')
                out[i] = ' ';
            else
                out[i] = *ptr;

            if(*ptr=='\"')
            {
                //out[i] = ' ';
                if(co%2==0 && (*(ptr-2)== ',' || *(ptr-1)== '['))
                    out2[i] = '#';
                else
                    out2[i] = ' ';

                co++;
            }
            else if(*ptr==',' && out2[i-1]==' ')
            {
                out2[i-1] = 13;
                out2[i] = '\n';
            }
            else
            {
                //out[i] = *ptr;
                out2[i] = *ptr;
            }

            ptr++;
            i++;
        }
        out[i]='\0';
        out2[i]='\0';

        strcpy(out3,name);
        strcat(out3,":");
        strcat(out3,out);
    }

    free(str);
    fclose(file);

    SetWindowText(hOut,out2);

    if(blindButton%2 == 1)
    {
        if(strlen(out3)>cL)
        {
            strncpy(out,out3,cL);
            out[cL] = '\0';
        }
        else
        {
            strcpy(out,out3);
        }
        tts(out);
        //tts2(out);
        //tts3(out);
    }
}

void Auto_Correct(HWND hWnd)
{
    int size = GetWindowTextLength(hAutoText),val;
    char initial[size+100];
    GetWindowText(hAutoText,initial,size+1);

    if(strcmp(initial,"")==0)
    {
        val = MessageBoxW(hWnd,L"You did not enter anything!",NULL,MB_OK | MB_ICONERROR);
    }

    if(val==IDOK)
    {
        return;
    }

    ExtChar(initial);
    UnformTxt(initial);
    ImproCase(initial);
    spellCorr(initial,hWnd);

    SetWindowText(hAutoText,initial);
}
void ExtChar(char *initial)
{
    int flag=1,count=1,check,size=strlen(initial);
    char str[size+2];
    char before[] = {'.',']','}','>',')',';','!',',','%'};
    char after[] = {'$','#','@','[','{','<','('};

    while(flag)
    {
        count=0;
        check=0;

        for(int i=0;i<size;i++)
        {
        str[count]=initial[i];

        if(((initial[i]==initial[i+1])&&(initial[i]<48 || initial[i]>57)&&(initial[i]<65 || initial[i]>90)&&(initial[i]<97 || initial[i]>122))||(initial[i]=='\n' && initial[i+2]=='\n'))
        {
            if(initial[i]=='\n' && initial[i+2]=='\n')
            {
                i++;
            }
            i++;
            check++;
        }
        else if(initial[i+2]=='\n'&&initial[i]!='.')
        {
            initial[count+1]=' ';
            initial[count+2]=' ';
        }
        else
        {
            if(initial[i]==' ')
            {
                for(int j=0;j<(sizeof(before)/sizeof(char));j++)
                {
                    if(initial[i+1]==before[j])
                    {
                        str[count]=before[j];
                        check++;
                        break;
                    }
                }
                for(int k=0;k<(sizeof(after)/sizeof(char));k++)
                {
                    if(initial[i-1]==after[k])
                    {
                        str[count]=after[k];
                        check++;
                        break;
                    }
                }
            }

        }

        count++;
        }

        if(check==0)
        {
            str[count]='\0';
            flag=0;
        }
        strcpy(initial,str);
        initial[count]='\0';
        size=count;
    }
}
void UnformTxt(char *initial)
{
    int count=0,flag,size = strlen(initial);
    char str[size+1];

    char after[] = {'.',',',')','}',']','>','?','%',';'};

    if(initial[0]!='\t')
    {
        str[0]='\t';
        count=1;
    }

    for(int i=0;i<size;i++)
    {
        str[count] = initial[i];
        for(int j=0;j<sizeof(after);j++)
        {
                if(initial[i]==after[j] && initial[i+1]!=' ' && (!(initial[i]=='.' && initial[i+1]==13)))
                {
                    flag = 1;
                    for(int k=0;k<sizeof(after);k++)
                    {
                        if(initial[i+1]==after[k])
                        {
                            flag=0;
                            break;
                        }
                    }
                    if(flag)
                    {
                        str[count+1] = ' ';
                        count++;
                    }
                }
                break;
        }
        count++;
    }
    str[count]='\0';
    strcpy(initial,str);
    initial[count]='\0';
}

void ImproCase(char *initial)
{
    int size = strlen(initial);

    for(int i=1;i<size;i++)
    {
        if(initial[i-1]=='\t')
        {
            initial[i] = toupper(initial[i]);
        }

        if(i-2>=0)
        {
            if((initial[i-2]=='.' && initial[i-1]==' ') || initial[i-1]=='\n')
            {
                initial[i] = toupper(initial[i]);
            }
            else if((initial[i-1]<65 || initial[i-1]>90) && (!(initial[i-1]=='.' && initial[i-2]>=65 && initial[i-2]<=90)) && (initial[i-1]!=' ' && initial[i-1]!='\"' && initial[i-1]!='\'' && initial[i-1]!='('))
            {
                initial[i] = tolower(initial[i]);
            }
        }
    }
}

void spellCorr(char *initial,HWND hWnd)
{
    int i,j,k,position=0,posSize,count,flag,flag2,flag3,size = strlen(initial);
    structCount=0;
    char str[size+1],str2[size+1],myChar,strMsg[200],strPosition[10];
    //wchar_t message[50]=L"hello";
    wchar_t message[150];

    char sign[] = {' ','(','{','[','<','/','@','#','$','\"','\'','-','\n','\t','.',')',',','}',']','>','?','/','!','%',';',':','\"','\'',13,'\0'};

    for(i=0;i<size;i++)
    {
        for(j=0;j<sizeof(sign);j++)
        {
            if(initial[i]==sign[j])
            {
                count=i+1;
                flag=1;
                while(flag)
                {
                  myChar = initial[count];
                  for(k=0;k<sizeof(sign);k++)
                    {
                        if(myChar==sign[k])
                        {
                            flag = 0;
                            str[count-(i+1)] = '\0';
                            break;
                        }

                    }
                    if(flag)
                    {
                        str[count-(i+1)] = myChar;
                        count++;
                    }
                    if(count>i+100)
                    {
                        flag=0;
                        i=size;
                    }
                }
                if(i<size)
                {
                    i+=count-(i+1);
                }
                break;
            }
        }

       if(strcmp(str,"")!=0)
       {
           position++;
       }

        if(CheckWord(str)==0 && i<size)
       {
            strcpy(SS[structCount].word,str);
            SS[structCount].word[strlen(str)] = '\0';

            SS[structCount].position = position;

            structCount++;
       }

    }
    if(structCount>=1)
    {
        displayDialogBox(hWnd);
    }
}

int checkNum(char* str)
{
    for(int i=0;i<strlen(str);i++)
    {
        if(str[i]<48 || str[i]>57)
        {
            return 0;
        }
    }
    return 1;
}


LRESULT CALLBACK DialogProcedure(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)
{
    int check,size = GetWindowTextLength(hWord);
    char str[size+1],message[150];
    wchar_t displayMessage[150];

    switch(msg)
    {
    case WM_COMMAND:
        switch(wp)
        {
            case CHECK_BUTTON:

                GetWindowText(hWord,str,size+1);
                check = CheckWord(str);

                if(check==-1)
                {
                    MessageBoxW(hWnd,L"Please enter a word!",NULL,MB_OK | MB_ICONERROR);
                }
                else if(check==0)
                {
                    strcpy(message,"Incorrect Word : ");
                    strcat(message,"\"");
                    strcat(message,str);
                    strcat(message,"\"");

                    mbstowcs(displayMessage,message,strlen(message));
                    displayMessage[strlen(message)] = L'\0';

                    MessageBoxW(hWnd,displayMessage,L"Error",MB_OK | MB_ICONWARNING);
                }
                else if(check==1)
                {
                    strcpy(message,"Correct Word : ");
                    strcat(message,"\"");
                    strcat(message,str);
                    strcat(message,"\"");

                    mbstowcs(displayMessage,message,strlen(message));
                    displayMessage[strlen(message)] = L'\0';

                    MessageBoxW(hWnd,displayMessage,L"Success",MB_OK | MB_ICONINFORMATION);
                    SetWindowTextW(hCancelButton,L"OK");
                }

                break;

            case CANCEL_BUTTON:
                displayStaticText(hWnd);
                break;
        }
        break;
    case WM_CLOSE:
        //displayStaticText(hWnd);
        structIndex = 0;
        DestroyWindow(hWnd);
        break;
    default:
        return DefWindowProcW(hWnd,msg,wp,lp);
    }
}

void dialogBoxClass(HINSTANCE hInst)
{
    WNDCLASSW dialog = {0};

    dialog.hbrBackground = (HBRUSH)COLOR_MENU;
    dialog.hCursor = LoadCursor(NULL,IDC_HAND);
    dialog.hInstance = hInst;
    dialog.lpszClassName = L"myDialogClass";
    dialog.lpfnWndProc = DialogProcedure;
    dialog.hIcon = LoadIcon(NULL,IDI_ERROR);

    RegisterClassW(&dialog);
}

void displayDialogBox(HWND hWnd)
{
    wchar_t message[200];
    char strMsg[200],strPosition[10];
    int posSize;

    strcpy(strMsg,"Spelling Mistake: \"");
    strcat(strMsg,SS[structIndex].word);
    strcat(strMsg,"\"");
    strcat(strMsg,"\n(Position: ");

    posSize = log10(SS[structIndex].position)+1;
    //itoa(position,strPosition,posSize);
    sprintf(strPosition,"%d",SS[structIndex].position);
    strPosition[posSize] = '\0';

    strcat(strMsg,strPosition);
    strcat(strMsg,")");

    mbstowcs(message,strMsg,strlen(strMsg));
    message[strlen(strMsg)] = L'\0';
    //wcscat(message,L" is wrong. Please correct it.");

    structIndex++;

    MessageBeep(MB_ICONSTOP);

    HWND hDia = CreateWindowW(L"myDialogClass",L"Spelling Check",WS_VISIBLE | WS_OVERLAPPEDWINDOW,300,250,500,230,
                  hWnd,NULL,NULL,NULL);

    hStaticDialog = CreateWindowW(L"Static",message,WS_VISIBLE | WS_CHILD | ES_CENTER,100,30,300,35,hDia,NULL,NULL,NULL);
    hWord = CreateWindowW(L"Edit",L"",WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,175,80,150,35,hDia,NULL,NULL,NULL);

    CreateWindowW(L"Button",L"Check",WS_VISIBLE | WS_CHILD | WS_BORDER,150,130,90,35,hDia,(HMENU)CHECK_BUTTON,NULL,NULL);
    hCancelButton = CreateWindowW(L"Button",L"Cancel",WS_VISIBLE | WS_CHILD | WS_BORDER,260,130,90,35,hDia,(HMENU)CANCEL_BUTTON,NULL,NULL);
}

int CheckWord(char *str)
{
    int flag2, flag3, size = strlen(str);
    char str2[size+1],search[size+5];

    if(strcmp(str,"")==0)
    {
        return -1;
    }
    else if(checkNum(str)==1)
    {
        return 1;
    }
    else
    {
        FILE *file = fopen("words.txt","rb");

        fseek(file,0,SEEK_END);
        int sizeF = ftell(file);
        rewind(file);

        char *wordFile = malloc(sizeF+1);
        fread(wordFile,sizeF,1,file);
        wordFile[sizeF] = '\0';

        strcpy(str2,str);
        strlwr(str);

        flag2=1,flag3=1;

        x:
        strcpy(search,"\n");
        strcat(search,str);
        strcat(search,"\n");

        char *ptr = strstr(wordFile,search);

        if(ptr==NULL)
        {
            if(strcmp(str,str2)==0)
            {
                if(str[0]>='a' && str[0] <='z' && flag2!=0)
                {
                    str[0] = str[0] - 32;
                    flag2=0;
                    goto x;
                }
                else if(flag3!=0)
                {
                    strupr(str);
                    flag3=0;
                    goto x;
                }

                return 0;
            }
            else
            {
                strcpy(str,str2);
                goto x;
            }
        }
        else
        {
            return 1;
        }
    }
}
void displayStaticText(HWND hWnd)
{
    if(structIndex >= structCount)
    {
        structIndex = 0;
        DestroyWindow(hWnd);
        return;
    }
    wchar_t message[200];
    char strMsg[200],strPosition[10];
    int posSize;

    strcpy(strMsg,"Spelling Mistake: \"");
    strcat(strMsg,SS[structIndex].word);
    strcat(strMsg,"\"");
    strcat(strMsg,"\n(Position: ");

    posSize = log10(SS[structIndex].position)+1;
    //itoa(position,strPosition,posSize);
    sprintf(strPosition,"%d",SS[structIndex].position);
    strPosition[posSize] = '\0';

    strcat(strMsg,strPosition);
    strcat(strMsg,")");

    mbstowcs(message,strMsg,strlen(strMsg));
    message[strlen(strMsg)] = L'\0';

    SetWindowTextW(hStaticDialog,message);
    SetWindowTextW(hWord,L"");
    SetWindowTextW(hCancelButton,L"Cancel");
    structIndex++;
}














