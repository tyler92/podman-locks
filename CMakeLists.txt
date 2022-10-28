project(podman-locks LANGUAGES C)

add_executable(${PROJECT_NAME} main.c shm_lock.c)
target_link_libraries(${PROJECT_NAME} PUBLIC rt pthread)
