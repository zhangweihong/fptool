#include <windows.h>
#include <tlhelp32.h> //进程快照函数头文件
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <stdlib.h>

struct ProcessStruct
{
    char *name;
    unsigned long id;
};

char *strupr(char *str)
{
    char *origin = (char *)malloc(200 * sizeof(char));
    int i = 0;
    while (str[i] != '\0')
    {
        origin[i] = tolower(str[i]);
        i++;
    }
    origin[i] = '\0';
    return origin;
}

BOOL arryIsHave(char *arry[], char *origin)
{
    int i = 0;
    while (arry[i] != NULL)
    {
        if (strstr(origin, arry[i]) != NULL)
        {
            return TRUE;
        }
        i++;
    }
    return FALSE;
}

int main(int argc, char *args[])
{
    int countProcess = 0;
    char _yKill = '\0';
    PROCESSENTRY32 currentProcess;
    currentProcess.dwSize = sizeof(currentProcess);
    HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcess == INVALID_HANDLE_VALUE)
    {
        printf("CreateToolhelp32Snapshot()调用失败!\n");
        return -1;
    }
    int i = 1;
    int maxSizeFind = 20;
    char **willFind = (char **)calloc(maxSizeFind, sizeof(char *));
    int maxSizePS = 200;
    struct ProcessStruct *allProcess = calloc(maxSizePS, sizeof(struct ProcessStruct));
    int startPushFindIndex = -1;
    while (args[i] != NULL)
    {
        if (startPushFindIndex > maxSizeFind - 1)
        {
            maxSizeFind = startPushFindIndex * 3 * sizeof(char *);
            willFind = (char **)realloc(willFind, maxSizeFind);
        }
        if (strstr(args[i], "-kill") != NULL)
        {
            printf("Are you sure which process you want to kill?(y/n)\n");
             _yKill = getchar();
            if (_yKill == 'y')
            {
                printf("\nYou typed yes\n\n");
            }
        }

        if (startPushFindIndex > -1)
        {
            if (strstr(args[i], "-"))
            {
                //如果出现下一个指令，停止加入剩余的查找
            }else{
                willFind[startPushFindIndex] = strupr(args[i]);
                startPushFindIndex++;
            }
            
        }

        if (strstr(args[i], "-f") != NULL)
        {
            startPushFindIndex = 0;
        }
        i++;
    }
    PROCESSENTRY32 pe = {sizeof(pe)};
    BOOL bMore = Process32First(hProcess, &pe);
    while (bMore)
    {

        if (countProcess > maxSizePS - 1)
        {
            maxSizePS = countProcess * 2 * sizeof(struct ProcessStruct);
            allProcess = realloc(allProcess, maxSizePS);
            printf("maxSizePS %d \n",maxSizePS);

        }
        
        if (startPushFindIndex > -1)
        {
            char *origin = strupr(pe.szExeFile);
            if (arryIsHave(willFind, origin))
            {
                allProcess[countProcess].id = pe.th32ProcessID;
                allProcess[countProcess].name = malloc(100 * sizeof(char));
                strcpy(allProcess[countProcess].name, pe.szExeFile);
                countProcess++;
            }
        }
        else
        {
            allProcess[countProcess].id = pe.th32ProcessID;
            printf("pe.szExeFile %d %s\n",countProcess, pe.szExeFile);
            allProcess[countProcess].name = malloc(100 * sizeof(char));
            strcpy(allProcess[countProcess].name, pe.szExeFile);
            countProcess++;
        }
        bMore = Process32Next(hProcess, &pe);
    }
    CloseHandle(hProcess);
    for (int j = 0; j < countProcess; j++)
    {
        if(_yKill == 'y'){
            char charIds[20] = {"\0"};
            int radix=10;
            itoa(allProcess[j].id,charIds,radix);
            char cmdHead[20] = {"taskkill /pid "};
            char *cmd = strcat(cmdHead,charIds);
            char *cmd2 = strcat(cmd," -t -f");
            system(cmd2);
        }else{
            printf("%d.PID = %5u PName = %s\n", (j + 1), allProcess[j].id, allProcess[j].name);
        }
    }

    printf("\n %d related processes were found \n", countProcess);
}