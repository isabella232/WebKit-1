if (ENABLE_WEBKIT2)
    add_subdirectory(WebKitTestRunner)
endif ()

if ("${PORT}" STREQUAL "Efl")
    add_subdirectory(EWebLauncher/ControlTheme)

    if (ENABLE_WEBKIT)
        add_subdirectory(DumpRenderTree/efl)
        add_subdirectory(EWebLauncher)
    endif ()

    if (ENABLE_WEBKIT2 AND ELEMENTARY_FOUND)
        add_subdirectory(MiniBrowser/efl)
    endif ()
    add_subdirectory(DumpRenderTree/TestNetscapePlugIn)
elseif ("${PORT}" STREQUAL "WinCE")
    add_subdirectory(WinCELauncher)
endif ()

if (ENABLE_WEBKIT2 AND ENABLE_API_TESTS)
    add_subdirectory(TestWebKitAPI)
endif ()
