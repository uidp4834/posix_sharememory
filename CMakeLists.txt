CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_CXX_FLAGS " -std=c++11 ")
SET(example_shm_write test_shm_write)
SET(example_shm_read test_shm_read)

# Add the source in project root directory
aux_source_directory(./src DIR_SHMWRITE)
aux_source_directory(./write DIR_SHMWRITE)

aux_source_directory(./src DIR_SHMREAD)
aux_source_directory(./read DIR_SHMREAD)

#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib) 

# Add header file include directories
include_directories(
			.
			./include
	)
 

ADD_EXECUTABLE(${example_shm_write} ${DIR_SHMWRITE})
TARGET_LINK_LIBRARIES(${example_shm_write} pthread rt)

ADD_EXECUTABLE(${example_shm_read} ${DIR_SHMREAD})
TARGET_LINK_LIBRARIES(${example_shm_read} pthread rt)


