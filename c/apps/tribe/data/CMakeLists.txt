set(desktopdir ${DATADIR}/applications)

configure_file(tribe.desktop.in
  tribe.desktop
  @ONLY
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tribe.desktop
        DESTINATION ${desktopdir}
)
