configure_file(sab.desktop.in
  sab.desktop
  @ONLY
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sab.desktop
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications
)
