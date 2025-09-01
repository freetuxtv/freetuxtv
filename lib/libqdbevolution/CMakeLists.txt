SET (QDBEVOLUTION_SRC
	QDbEvolution.cpp
	QDbEvolution.h
)

if (NOT SINGLE_UNIT_COMPILATION)
	add_library (libqdbevolution ${QDBEVOLUTION_SRC})
else()
	generate_single_unit_sources(libqdbevolution "${QDBEVOLUTION_SRC}" SINGLE_UNIT_FILES)
	add_library (libqdbevolution ${SINGLE_UNIT_FILES})
endif()

if(WITH_INSTALL_LIB)
	install(TARGETS libqdbevolution DESTINATION ${INSTALL_PATH_LIB})
endif()
