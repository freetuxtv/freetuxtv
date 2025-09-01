SET (TOOLKIT_SRC
	SingletonManager.cpp
	SingletonManager.h
)

if (NOT SINGLE_UNIT_COMPILATION)
	add_library (toolkit ${TOOLKIT_SRC})
else()
	generate_single_unit_sources(toolkit "${TOOLKIT_SRC}" SINGLE_UNIT_FILES)
	add_library (toolkit ${SINGLE_UNIT_FILES})
endif()

if(WITH_INSTALL_LIB)
	install(TARGETS toolkit DESTINATION ${INSTALL_PATH_LIB})
endif()
