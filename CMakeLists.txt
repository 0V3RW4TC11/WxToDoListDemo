cmake_minimum_required(VERSION 3.21)
project(WxToDoListDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Conan's CMakeDeps generates the config files; this resolves to the
# wxWidgets::wxWidgets target (verified against the conan-center recipe).
find_package(wxWidgets REQUIRED)

# WIN32 => Windows GUI subsystem (no console window). wxIMPLEMENT_APP
# supplies WinMain, so this links cleanly as a GUI app.
# app.rc pulls in the wx manifest for proper theming + high-DPI.
add_executable(${PROJECT_NAME} WIN32
    "src/app.rc"
    "src/app.h"
    "src/app.cpp"
    "src/mainFrame.h"
    "src/mainFrame.cpp"
    "src/task.h"
    "src/task.cpp"
    "src/utils.h"
 )

target_link_libraries(${PROJECT_NAME} PRIVATE wxWidgets::wxWidgets)

# On MSVC the linker embeds its own DPI-unaware manifest by default, which
# overrides the one wx.rc provides -> app runs DPI-unaware (blurry).
# Disable the linker's manifest so wx's manifest from app.rc is the only one.
if(MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE "/MANIFEST:NO")
endif()