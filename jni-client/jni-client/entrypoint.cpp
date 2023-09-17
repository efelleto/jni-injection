#include <thread>
#include <cstdio>
#include <iostream>

#include <Windows.h>
#include <jni.h>

void MainThread(HMODULE instance)
{
    JavaVM* p_jvm{ nullptr };
    jint result = JNI_GetCreatedJavaVMs(&p_jvm, 1, nullptr);

    printf("[debug] process hooked\n");
    printf("[debug] jvm address -> "); std::cout << p_jvm;

    void* p_env{ nullptr }; 
    p_jvm->AttachCurrentThread(&p_env, nullptr);

    while (!GetAsyncKeyState(VK_RSHIFT))
    {

    }

    FreeLibrary(instance);
}

bool __stdcall DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    static FILE* p_file{ nullptr };
    static std::thread main_thread;

    if (reason == DLL_PROCESS_ATTACH)
    {
        AllocConsole();
        freopen_s(&p_file, "CONOUT$", "w", stdout);

        main_thread = std::thread([instance] { MainThread(instance); });
        if (main_thread.joinable())
            main_thread.detach();
    }
    else if (reason == DLL_PROCESS_DETACH)
    {
        fclose(p_file);
        FreeConsole();
    }

    return true;
}