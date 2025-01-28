target_sources(GViewCore PRIVATE KaitaiViewer.hpp Config.cpp GoToDialog.cpp Instance.cpp Settings.cpp)

add_subdirectory(../../../../Types/Kaitai KaitaiLib)

target_link_libraries(GViewCore PRIVATE KaitaiStruct)
target_include_directories(GViewCore PRIVATE ../../../../Types/Kaitai/include)