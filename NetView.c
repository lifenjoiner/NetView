/*  varient of 'net view' for script
    YX Hao
*/
#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include <stdio.h>
#include <lm.h>

#pragma comment(lib, "Netapi32.lib")
#pragma comment(lib, "Advapi32.lib")

int wmain( int argc, TCHAR *lpszArgv[])
{
   PSHARE_INFO_1 BufPtr, p;
   NET_API_STATUS res;
   LPTSTR   lpszServer = NULL;
   DWORD er=0, tr=0, resume=0, i;

   switch(argc)
   {
   case 2:
      lpszServer = lpszArgv[1];
      break;
   default:
      printf("Usage: %S <servername>\n", lpszArgv[0]);
      return 1;
   }
   // Print a report header.
   printf("Type\t\tShare\n");
   // Call the NetShareEnum function; specify level 502.
   do // begin do
   {
      res = NetShareEnum (lpszServer, 1, (LPBYTE *) &BufPtr, MAX_PREFERRED_LENGTH, &er, &tr, &resume);
      // If the call succeeds,
      if(res == ERROR_SUCCESS || res == ERROR_MORE_DATA)
      {
         p=BufPtr;
         // Loop through the entries, print retrieved data.
         for(i=1; i<=er; i++)
         {
            char *mbname;
            int nm_counter = WideCharToMultiByte(CP_OEMCP, 0, p->shi1_netname, -1, mbname, 0, NULL, NULL);
            mbname = (char *)calloc(nm_counter, 1);
            WideCharToMultiByte(CP_OEMCP, 0, p->shi1_netname, -1, mbname, nm_counter, NULL, NULL);
            printf("0x%08X\t%s\n", p->shi1_type, mbname);
            free(mbname);
            p++;
         }
         // Free the allocated buffer.
         NetApiBufferFree(BufPtr);
      }
      else 
         printf("Error: %ld\n",res);
   } while (res==ERROR_MORE_DATA); // end do
   return 0;
}
