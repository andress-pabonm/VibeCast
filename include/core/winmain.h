#ifndef VIBECAST_WINMAIN_H
#define VIBECAST_WINMAIN_H 1

#ifdef VIBECAST_WINMAIN

#include <windows.h>

int main(int argc, const char *argv[]);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    // Llama a tu función principal "real"
    return main(__argc, __argv); // solo en MSVC
}

#endif // VIBECAST_WINMAIN

#endif // VIBECAST_WINMAIN_H