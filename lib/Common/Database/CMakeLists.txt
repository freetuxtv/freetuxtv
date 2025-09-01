SET (COMMON_DATABASE_SRC
	DatabaseConnection.cpp
	DatabaseConnection.h
	DatabaseController.cpp
	DatabaseController.h
	DatabaseControllerFactory.cpp
	DatabaseControllerFactory.h
	DatabaseErrorHandler.h
	DatabaseEvolution.cpp
	DatabaseEvolution.h
	DatabaseInstance.cpp
	DatabaseInstance.h
	DatabaseManager.cpp
	DatabaseManager.h
)

if (NOT SINGLE_UNIT_COMPILATION)
	add_library (common_database ${COMMON_DATABASE_SRC})
else()
	generate_single_unit_sources(common_database "${COMMON_DATABASE_SRC}" SINGLE_UNIT_FILES)
	add_library (common_database ${SINGLE_UNIT_FILES})
endif()
