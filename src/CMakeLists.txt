file(GLOB SOURCES
    "*.cpp"
)

add_library (hplotlib ${SOURCES})
target_link_libraries (hplotlib glld ${CMAKE_THREAD_LIBS_INIT} -lopengl32)
