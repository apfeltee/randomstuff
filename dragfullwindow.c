
#include <stdio.h>
#include <windows.h>

#pragma comment(lib, "user32.lib") 

#define print(...) \
	fprintf(stdout, __VA_ARGS__); \
	fflush(stdout);

int main(int argc, char* argv[])
{
    BOOL enabled;
    SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, (UINT)NULL, &enabled, 0);
    print("Current 'dragfullwindow' option status: %d\n", enabled);
    if(!enabled)
    {
		print("enabling 'dragfullwindow' ... ");
        SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, 
                         TRUE,
                         NULL,
                         SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
		print("done!\n");
    }
	return 0;
}
