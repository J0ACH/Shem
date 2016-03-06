PROJECT(mySCIDE)

cmake_minimum_required( VERSION 2.8.11 )
SET(CMAKE_AUTOMOC OFF)
# This makes sclang/scide work with a Qt installation at a fixed, non-system location.
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5LinguistTools)
find_package(Qt5WebKitWidgets)
find_package(Qt5OpenGL)
find_package(Qt5PrintSupport)
find_package(Qt5Sensors)
find_package(Qt5Quick)
find_package(Qt5Qml)
find_package(Qt5Sql)
find_package(Qt5Positioning)


FIND_PACKAGE(Yaml)
FIND_PACKAGE(OscPack)
FIND_PACKAGE(Boost)

	

set(QT_IDE_LIBRARIES
    Qt5::Core Qt5::WebKitWidgets Qt5::OpenGL Qt5::PrintSupport Qt5::Sensors Qt5::Quick Qt5::Qml Qt5::Sql Qt5::Positioning)

if(${CMAKE_COMPILER_IS_GNUCXX})
    add_definitions(-Wreorder)
endif()

set ( ide_moc_hdr
    core/main.hpp
    core/sig_mux.hpp
    core/sc_process.hpp
    core/sc_server.hpp
    core/doc_manager.hpp
    core/settings/manager.hpp
    core/session_manager.hpp
    widgets/cmd_line.hpp
    widgets/doc_list.hpp
    widgets/documents_dialog.hpp
    widgets/editor_box.hpp
    widgets/find_replace_tool.hpp
    widgets/goto_line_tool.hpp
    widgets/lookup_dialog.hpp
    widgets/main_window.hpp
    widgets/multi_editor.hpp
    widgets/popup_text_input.hpp
    widgets/post_window.hpp
    widgets/session_switch_dialog.hpp
    widgets/sessions_dialog.hpp
    widgets/tool_box.hpp
    widgets/help_browser.hpp
    widgets/audio_status_box.hpp
    widgets/lang_status_box.hpp
    widgets/code_editor/editor.hpp
    widgets/code_editor/sc_editor.hpp
    widgets/code_editor/highlighter.hpp
    widgets/code_editor/line_indicator.hpp
    widgets/code_editor/overlay.hpp
    widgets/code_editor/autocompleter.hpp
    widgets/code_editor/completion_menu.hpp
    widgets/settings/dialog.hpp
    widgets/settings/general_page.hpp
    widgets/settings/sclang_page.hpp
    widgets/settings/editor_page.hpp
    widgets/settings/shortcuts_page.hpp
    widgets/util/path_chooser_widget.hpp
    widgets/util/text_format_list_widget.hpp
    widgets/util/key_sequence_edit.hpp
    widgets/util/popup_widget.hpp
    widgets/util/color_widget.hpp
    widgets/util/docklet.hpp
    widgets/util/volume_widget.hpp

    ${SC_SOURCE_DIR}/QtCollider/widgets/web_page.hpp
)

file (GLOB_RECURSE all_hdr *hpp)

set ( ide_src
    core/main.cpp
    core/sig_mux.cpp
    core/doc_manager.cpp
    core/sc_process.cpp
    core/sc_server.cpp
    core/sc_introspection.cpp
    core/sc_lexer.cpp
    core/settings/serialization.cpp
    core/settings/manager.cpp
    core/settings/theme.cpp
    core/session_manager.cpp
    core/util/standard_dirs.cpp
    core/util/scdoc_log.cpp
    widgets/cmd_line.cpp
    widgets/doc_list.cpp
    widgets/documents_dialog.cpp
    widgets/editor_box.cpp
    widgets/find_replace_tool.cpp
    widgets/lookup_dialog.cpp
    widgets/main_window.cpp
    widgets/multi_editor.cpp
    widgets/post_window.cpp
    widgets/session_switch_dialog.cpp
    widgets/help_browser.cpp
    widgets/audio_status_box.cpp
    widgets/lang_status_box.cpp
    widgets/code_editor/editor.cpp
    widgets/code_editor/sc_editor.cpp
    widgets/code_editor/highlighter.cpp
    widgets/code_editor/line_indicator.cpp
    widgets/code_editor/overlay.cpp
    widgets/code_editor/autocompleter.cpp
    widgets/code_editor/completion_menu.cpp
    widgets/settings/dialog.cpp
    widgets/settings/general_page.cpp
    widgets/settings/sclang_page.cpp
    widgets/settings/editor_page.cpp
    widgets/settings/shortcuts_page.cpp
    widgets/util/gui_utilities.cpp
    widgets/util/text_format_list_widget.cpp
    widgets/util/popup_widget.cpp
    widgets/util/docklet.cpp
    widgets/util/volume_widget.cpp
    widgets/util/status_box.cpp
    widgets/style/style.cpp

    ${SC_SOURCE_DIR}/QtCollider/widgets/web_page.cpp

    ${SC_SOURCE_DIR}/common/SC_TextUtils.cpp

    ${SC_SOURCE_DIR}/SCDoc/SCDoc.cpp
    ${SC_SOURCE_DIR}/SCDoc/SCDoc.tab.cpp
    ${SC_SOURCE_DIR}/SCDoc/lex.scdoc.cpp
)

set( ide_forms
    forms/settings_dialog.ui
    forms/settings_general.ui
    forms/settings_sclang.ui
    forms/settings_editor.ui
    forms/settings_shortcuts.ui
)

# QtCollider shared sources:
list(APPEND ide_moc_hdr
    ${SC_SOURCE_DIR}/QtCollider/layouts/stack_layout.hpp
)
list(APPEND ide_src
    ${SC_SOURCE_DIR}/QtCollider/layouts/stack_layout.cpp
)

list(APPEND ide_src ${SC_SOURCE_DIR}/common/SC_DirUtils.cpp)

if(WIN32)
    list(APPEND ide_src ${SC_SOURCE_DIR}/common/SC_Win32Utils.cpp)
endif()

include_directories(${SC_SOURCE_DIR}/include/common)
include_directories(${SC_SOURCE_DIR}/common)
include_directories(${SC_SOURCE_DIR}/include/plugin_interface)
#include_directories(${YAMLCPP_INCLUDE_DIR})
include_directories(${SC_SOURCE_DIR}/external_libraries/boost)
# For QtCollider headers:
include_directories(${SC_SOURCE_DIR})
# Needed for auto-generated forms headers:
include_directories(${SC_SOURCE_DIR}/editors/sc-ide/widgets/util)
include_directories(${SC_SOURCE_DIR}/editors/sc-ide/widgets)
include_directories(${SC_BUILD_DIR}/editors/sc-ide)

#INCLUDE_DIRECTORIES(${OSCPACK_INCLUDE_DIR})
#INCLUDE_DIRECTORIES(${YAML_INCLUDE_DIR})
include_directories(${SC_SOURCE_DIR}/external_libraries/oscpack_1_1_0)
include_directories(${SC_SOURCE_DIR}/external_libraries/yaml-cpp-0.3.0/include)
INCLUDE_DIRECTORIES(${SC_SOURCE_DIR}/external_libraries/boost)

include_directories(${SC_BUILD_DIR}/common)


#include(${CMAKE_SOURCE_DIR}/SCDoc/CMakeLists.txt)

# Translation files
set( translation_src
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/scide_de.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/scide_es.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/scide_fr.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/scide_ja.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/scide_pt.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/scide_ru.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/scide_sl.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/scide_sv.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/scide_zh.ts
)

# Translation file for source code native language, used only to handle singular/plural forms
set( native_translation_src ${CMAKE_CURRENT_SOURCE_DIR}/translations/scide.ts )

# A handy target to update translation source files
add_custom_target( update_ide_translations
    COMMAND lupdate ${CMAKE_CURRENT_SOURCE_DIR} -ts ${translation_src}
            -no-obsolete
    COMMAND lupdate ${CMAKE_CURRENT_SOURCE_DIR} -ts ${native_translation_src}
            -no-obsolete -pluralonly
)

qt5_wrap_cpp( ide_moc_src ${ide_moc_hdr} )
qt5_wrap_ui( ide_forms_src ${ide_forms} )
#qt5_add_resources( ide_rcc resources.qrc )
qt5_add_translation( translations ${translation_src} ${native_translation_src} )

set(ide_sources ${ide_src} ${all_hdr} )

set(ide_rc_sources ${ide_moc_src} ${ide_forms_src} ${ide_rcc} ${translations})

# This sets up the exe icon for windows.
if(WIN32)
 set(RES_FILES ${SC_SOURCE_DIR}/platform/windows/Resources/scide.rc)
 set(CMAKE_RC_COMPILER_INIT windres)
 ENABLE_LANGUAGE(RC)
 SET(CMAKE_RC_COMPILE_OBJECT
 "<CMAKE_RC_COMPILER> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif(WIN32)


add_executable( SuperCollider MACOSX_BUNDLE ${ide_sources} ${ide_rc_sources} ${RES_FILES} ${AdditionalBundleSources})

set_target_properties(SuperCollider PROPERTIES OUTPUT_NAME "scide")

target_link_libraries( SuperCollider
    ${QT_IDE_LIBRARIES}
    ${YAML_LIBRARY}
    ${OSCPACK_LIBRARY}
	${BOOST_LIBRARY}
)

#if(Boost_FOUND)
 #   target_link_libraries( SuperCollider ${Boost_SYSTEM_LIBRARY} )
#else()
    # in-house-built boost system
 #   target_link_libraries( SuperCollider boost_system )
#endif()

if(WIN32)
    target_link_libraries( SuperCollider wsock32 )
    # The following prevents a Windows console from showing up
    # when the executable is started:
    set_target_properties( SuperCollider PROPERTIES WIN32_EXECUTABLE TRUE )
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(SuperCollider rt)

    find_package(X11 REQUIRED)

    include_directories(${X11_INCLUDE_DIR})
    target_link_libraries(SuperCollider ${X11_X11_LIB})
endif()

if(PTHREADS_FOUND)
    target_link_libraries(SuperCollider ${PTHREADS_LIBRARIES})
endif()

if(LTO)
    set_property(TARGET SuperCollider
                 APPEND PROPERTY COMPILE_FLAGS "-flto -flto-report")

    set_property(TARGET SuperCollider
                 APPEND PROPERTY LINK_FLAGS "-flto -flto-report -fwhole-program")
endif()


# Installation

if(WIN32)
  install(TARGETS SuperCollider
      DESTINATION "SuperCollider"
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
  install(FILES ${translations}
      DESTINATION "SuperCollider/translations")

  SET(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "${CMAKE_INSTALL_PREFIX}/SuperCollider/platforms")
    include(InstallRequiredSystemLibraries)

  if(MSYS)
    SET(QT_PLATFORM_PLUGIN_PREFIX "${Qt5Core_DIR}/../../../share/qt5/plugins/platforms" CACHE STRING "Location of qt platform plugin for windows as provided by MSYS2!")
  else()
    SET(QT_PLATFORM_PLUGIN_PREFIX "${Qt5Core_DIR}/../../../plugins/platforms" CACHE STRING "Location of qt platform plugin for windows")
  endif()

  install(FILES "${QT_PLATFORM_PLUGIN_PREFIX}/qwindows$<$<CONFIG:DEBUG>:d>.dll"
    DESTINATION ${CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION}
  )

else()
  install(TARGETS SuperCollider
      RUNTIME DESTINATION "bin"
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
  install(FILES ${translations}
      DESTINATION "share/SuperCollider/translations")
endif()

if (LINUX)
  install( PROGRAMS SuperColliderIDE.desktop  DESTINATION "share/applications")
endif()
