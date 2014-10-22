

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

add_library (hplotlib ${SOURCES})
target_link_libraries (hplotlib glld ${CMAKE_THREAD_LIBS_INIT} ${OPENGL_LIBRARY})
