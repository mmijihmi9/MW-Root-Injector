#include <jni.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <dlfcn.h>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_android.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "MW_Mod", __VA_ARGS__)

static bool g_Initialized = false;
static EGLint g_Width = 0, g_Height = 0;

typedef EGLBoolean (*eglSwapBuffers_t)(EGLDisplay dpy, EGLSurface surface);
static eglSwapBuffers_t orig_eglSwapBuffers = nullptr;

EGLBoolean hooked_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    if (!g_Initialized) {
        eglQuerySurface(dpy, surface, EGL_WIDTH, &g_Width);
        eglQuerySurface(dpy, surface, EGL_HEIGHT, &g_Height);
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)g_Width, (float)g_Height);
        ImGui::StyleColorsDark();
        ImGui_ImplOpenGL3_Init("#version 300 es");
        g_Initialized = true;
        LOGI("Menu Initialized");
    }

    ImGui_ImplAndroid_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Modern Warships - Root Injector");
    ImGui::Text("Injection Successful! Root Mode.");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return orig_eglSwapBuffers(dpy, surface);
}

// دالة تعمل تلقائياً عند حقن الملف
__attribute__((constructor))
void init_payload() {
    void *libegl = dlopen("libEGL.so", RTLD_NOW);
    if (libegl) {
        void *sym = dlsym(libegl, "eglSwapBuffers");
        orig_eglSwapBuffers = (eglSwapBuffers_t)sym;
        // في بيئة الإنتاج، يجب استخدام DobbyHook هنا لتغيير الدالة
        // هذا الكود يعمل كتوضيح للهيكل
        LOGI("Payload loaded and EGL found.");
    }
}
