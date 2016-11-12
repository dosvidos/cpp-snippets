#include <windows.h>
#include <stdio.h>
#define BUFF_MAX_LEN 4096
void GetInstalledSoft (char *ResultBuff,DWORD BuffLen);
void GetInstalledSoft (char *ResultBuff,DWORD BuffLen)
{
HKEY hKey,hItemKey;
char  ItemKey[BUFF_MAX_LEN],Data[BUFF_MAX_LEN],
      SoftName[BUFF_MAX_LEN],SoftVersion[BUFF_MAX_LEN];
      
DWORD cbItemKey=sizeof(cbItemKey),cbData=sizeof(Data),
      cbSoftName=sizeof(SoftName),cbSoftVersion=sizeof(SoftVersion),
      i = 0, type;
memset(ResultBuff, 0, BuffLen);
memset(Data, 0, cbData);
if (RegOpenKeyA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall", &hKey))
    {
    return;
    }
while (RegEnumKeyA(hKey, i++, Data, cbData) == 0) {
      cbSoftName=BUFF_MAX_LEN;
      cbSoftVersion=BUFF_MAX_LEN;
      memset(SoftName, 0, cbSoftName);
      memset(SoftVersion, 0, cbSoftVersion);
      memset(ItemKey, 0, cbItemKey);
      
      sprintf(ItemKey,"%s\\%s","Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall",Data);
      
      if (RegOpenKeyA(HKEY_LOCAL_MACHINE, ItemKey, &hItemKey)) {
         return;
      }
      
      RegQueryValueExA(hItemKey, "DisplayName", 0, &type, SoftName, &cbSoftName);
      RegQueryValueExA(hItemKey, "DisplayVersion", 0, &type, SoftVersion, &cbSoftVersion);
      RegCloseKey(hItemKey);
      if (SoftName[0]!=0) {
            sprintf(ResultBuff,"%s[IS] [Name: %s] [Vesion: %s]\n", ResultBuff, SoftName, SoftVersion);
              }
   }
RegCloseKey(hKey);
return;
}
void main( int argc, char **argv )
{
  char *InstalledSoftList;
  
  InstalledSoftList=malloc(BUFF_MAX_LEN);
  GetInstalledSoft(InstalledSoftList,sizeof(InstalledSoftList));
  printf("\nInstalled Software:\n%s\n",InstalledSoftList);
  return;
}