find_package(Qt5 COMPONENTS WebEngineWidgets REQUIRED)

include_directories(SYSTEM "${AvogadroLibs_SOURCE_DIR}/thirdparty/jsoncpp")

# Extension
set(importpqr_srcs
  importpqr.cpp
  pqrwidget.cpp
  pqrrequest.cpp
)

avogadro_plugin(ImportPQR
  "Download molecules from the Pitt Quantum Repository"
  ExtensionPlugin
  importpqr.h
  ImportPQR
  "${importpqr_srcs}"
  pqrwidget.ui
  ""
)

target_link_libraries(ImportPQR LINK_PRIVATE ${Qt5Network_LIBRARIES} ${Qt5WebEngineWidgets_LIBRARIES})
