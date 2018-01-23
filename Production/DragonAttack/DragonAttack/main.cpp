#include "AEEngine.h"

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	UNREFERENCED_PARAMETER(instanceH);
	UNREFERENCED_PARAMETER(prevInstanceH);
	UNREFERENCED_PARAMETER(command_line);
	UNREFERENCED_PARAMETER(show);

	AESysInit(instanceH, show, 800, 600, 1, 60, NULL);
}