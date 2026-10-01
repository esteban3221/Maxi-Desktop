#include "controller/main_window.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
#ifdef _WIN32
    const wchar_t *MUTEX_NAME = L"Global\\org.desktop.maxicajero.single_instance";
    HANDLE hMutex = CreateMutexW(NULL, TRUE, MUTEX_NAME);

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        if (hMutex)
            CloseHandle(hMutex);

        return 0;
    }
#endif

    auto app = Gtk::Application::create("org.desktop.maxicajero");
    int result = app->make_window_and_run<MainWindow>(argc, argv, app);

#ifdef _WIN32
    if (hMutex)
    {
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
    }
#endif

    return result;
}