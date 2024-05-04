add_subdirectory(kcm)

set(forceblur_SOURCES
    blur.cpp
    blur.qrc
    main.cpp
    config/config.cpp
    windowrules/windowrule.cpp
    windowrules/windowrulelist.cpp
)

add_library(forceblur MODULE ${forceblur_SOURCES})
target_link_libraries(forceblur PRIVATE
    KWin::kwin

    KF6::ConfigGui

    KDecoration2::KDecoration
)

install(TARGETS forceblur DESTINATION ${KDE_INSTALL_PLUGINDIR}/kwin/effects/plugins)
