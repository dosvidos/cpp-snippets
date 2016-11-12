/* vista1.c              Thomas R. Nicely          2007.04.08.2330
 *                    http://www.trnicely.net
 * GCC 4.12                 DJGPP 2.04                    GMP 4.12
 *
 * Freeware copyright (c) 2007 Thomas R. Nicely
 * <http://www.trnicely.net>. No warranties expressed or implied.
 * Distributed under the terms of the GNU GPL, GNU FDL, and DJGPP
 * licenses; see <http://www.gnu.org/licenses/licenses.html> and
 * <http://www.delorie.com/djgpp>. Source, binaries, and license
 * terms available upon request.
 *
 * SYNTAX: vista1 [max_mem_MB]
 *
 * Attempts to allocate a single chunk of memory, starting in size
 * at max_mem millions of bytes (default is 40 million bytes). If
 * the allocation fails, the size is reduced by one million bytes
 * per iteration, until allocation is successful or zero bytes
 * is reached. Results are reported to the console.
 *
 * When compiled with any version of GCC which does not link to
 * the Win32 API (as do CygWin and MinGW), this application appears
 * to fail when run under Windows Vista, allocating a maximum of
 * 32 or 33 million bytes of memory. Under other versions of
 * Windows (XP, 98SE, etc.) or extended DOS, the same executable
 * will allocate 40 million bytes (if present on the machine), and
 * can allocate several hundred million bytes if that much memory
 * is physically present and a command line such as "vista1 400" is
 * used to invoke the program. The reason for this incompatibility
 * in Vista is unknown, and no method for fixing it is known (other
 * than employing a compiler which links to the Win32 API).
 *
 * Allocating multiple arrays of smaller individual size will still
 * result in failure under Vista once the aggregate size exceeds
 * 32MB.
 *
 * Note that, depending on the compiler and environment, if more
 * than 32MB can be allocated, then it may also be possible to
 * allocate an amount exceeding the total physical memory (if the
 * environment allows for virtual memory).
 *
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

char signature[]=
  "\n __vista1.c__Version 2007.04.08.2330__Freeware copyright (c) 2007"
  "\n Thomas R. Nicely <http://www.trnicely.net>. No warranties expressed"
  "\n or implied. Distributed under the terms of the GNU GPL, GNU FDL, and"
  "\n DJGPP licenses; see <http://www.gnu.org/licenses/licenses.html> and"
  "\n <http://www.delorie.com/djgpp>. Source, binaries, and license terms"
  "\n available upon request.\n";

/**********************************************************************/
int main(int argc, char **argv)
{
unsigned char    *uchArray;
int              iArg, i;
long             lBytesMax, lBytes;

signal(SIGINT, exit);
signal(SIGTERM, exit);
#ifdef SIGQUIT
signal(SIGQUIT, exit);
#endif
setbuf(stdout, NULL);
setbuf(stderr, NULL);

if(argc > 1)
  {
  iArg=atoi(argv[1]);
  if(iArg < 1)
    {
    fprintf(stderr, "%s\n SYNTAX: %s [max_mem_MB]\n", signature, argv[0]);
    fprintf(stderr, "\n ...Press the ENTER key to exit: ");
    getchar();
    return(EXIT_FAILURE);
    }
  lBytesMax=iArg*1000000L;
  }
else
  lBytesMax=40000000L;
if(lBytesMax < 1000000L)lBytesMax=1000000L;

for(lBytes=lBytesMax; lBytes > 0; lBytes -= 1000000L)
  {
  printf("\n ...Allocation of %ld bytes being attempted...", lBytes);
  uchArray=(unsigned char *)malloc(lBytes);
  if(uchArray)  /* Test the accessibility and integrity of the array */
    {
    printf("\n   ...Allocation of %ld bytes being validated...", lBytes);
    printf("\n   ...Writing to array...");
    for(i=0; i < lBytes; i++)uchArray[i]=i%256;
    printf("\n   ...Verifying array...");
    for(i=0; i < lBytes; i++)
      {
      if(uchArray[i] != i%256)
	{
	printf("\n   ...Array check FAILED.");
	printf("\n   ...lBytes=%ld", lBytes);
	printf("\n uchArray(%ld)=%ld", i, uchArray[i]);
	printf("\n   ...i%%256=%ld\n", i%256);
	free(uchArray);
	goto NEXT;
	}
      }
    printf("\n ...Allocation of %ld bytes SUCCEEDED.\n", lBytes);
    free(uchArray);
    break;
    }
  else
    {
    printf("FAILED.");
    }
  NEXT: ;
  }

DONE: ;
fprintf(stderr, "\n ...Press the ENTER key to exit: ");
getchar();
return(EXIT_SUCCESS);
}
/**********************************************************************/
