cmake_minimum_required(VERSION 3.18.1)
project(MW_Injector)

# تجميع أداة الحقن
add_executable(injector injector.cpp)
target_link_libraries(injector log dl)

# تجميع الحمولة مع ImGui
add_library(mod SHARED 
    libmod.cpp 
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_android.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)
target_link_libraries(mod log android EGL GLESv3)
