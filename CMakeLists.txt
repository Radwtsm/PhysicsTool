cmake_minimum_required(VERSION 3.31)
project(WorldSimulation)

set(CMAKE_CXX_STANDARD 20)

file(GLOB IMGUI_SRC
        imgui/*.cpp
        imgui/backends/imgui_impl_win32.cpp
        imgui/backends/imgui_impl_dx11.cpp
)

add_executable(WorldSimulation
        main.cpp
        Kinematics.cpp
        ${IMGUI_SRC}
)

target_include_directories(WorldSimulation PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

target_link_libraries(WorldSimulation
        d3d11
        dxgi
        d3dcompiler
        dwmapi          # ✅ <-- AGGIUNTO PER RISOLVERE L'ERRORE
        kernel32
        user32
        gdi32
        winspool
        shell32
        ole32
        oleaut32
        uuid
        comdlg32
        advapi32
)
