# win10获取进程的相关信息并进行相关操作工具

## 1.查找当前所有进程
```
 ./fp.exe 
```
## 2.根据指令来搜索含有chrome字符串的进程 
```
 ./fp.exe -f chrome
```
## 3.根据指令来搜索含有chrome字符串的进程,是否来进行杀死
```
 ./fp.exe -f chrome -kill
```