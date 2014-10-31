

file (GLOB SOURCES
    "*.cpp"
)

if (WIN32)
	file (GLOB WIN32_SOURCES
		"win32/*.cpp"
	)	
	list (APPEND SOURCES ${WIN32_SOURCES})
endif ()

if (UNIX)
	file (GLOB X11_SOURCES
		"x11/*.cpp"
	)	
	list (APPEND SOURCES ${X11_SOURCES})
endif ()

list (APPEND SOURCES "GL/glld.c")

add_library (hplotlib ${SOURCES})
target_link_libraries (hplotlib ${CMAKE_THREAD_LIBS_INIT} ${OPENGL_LIBRARY})
install (TARGETS hplotlib DESTINATION lib)
