set(HELP_ID ${PROJECT_NAME})

set(HELP_FIGURES_DIR figures)

set(HELP_MEDIA
)

set(HELP_FILES
  index.page
)

set(cinstalled)
foreach(_file ${HELP_FILES})
  list(APPEND cinstalled "${CMAKE_CURRENT_SOURCE_DIR}/C/${_file}")
endforeach(_file)

install(FILES ${cinstalled}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/help/C/${HELP_ID}/
)
