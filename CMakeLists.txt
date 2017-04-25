cmake_minimum_required(VERSION 2.8)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#include_directories(${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})

#uncomment if you have defined messages
rosbuild_genmsg()
#uncomment if you have defined services
rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})

#link_directories(/home/raul/fuerte_workspace/dynamixel/src)
#find_library(DYNXL dxl)


#add_library(ldxl STATIC IMPORTED)
#set_property(TARGET ldxl PROPERTY IMPORTED_LOCATION home/raul/fuerte_workspace/dynamixel/src/libdxl.a)


#FIND_LIBRARY(TCL_LIB NAMES dxl PATHS /home/raul/fuerte_workspace/dynamixel/src)
#rosbuild_add_executable(nodo_dynamixel src/nodo_dynamixel.cpp src/motordynamixel.cpp)
#LINK_DIRECTORIES(home/raul/fuerte_workspace/dynamixel/src)
#target_link_libraries(nodo_dynamixel TCL_LIB)
#rosbuild_add_executable(nodo_dynamixel src/nodo_dynamixel.cpp src/motordynamixel.cpp)
#target_link_libraries(${PROJECT_NAME} libdxl.a)

#SET(libs liblibdxl.a)

#target_link_libraries(${PROJECT_NAME} ldxl)



set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}  CACHE PATH "CMAKE_MODULE_PATH")
#MESSAGE(STATUS "CMAKE_PATH: " ${CMAKE_MODULE_PATH})
#MESSAGE(STATUS "CMAKE_DIR: " ${CMAKE_SOURCE_DIR})
find_package(libdxl REQUIRED)

find_package ( SDL REQUIRED )
find_package( PCL 1.2 REQUIRED)
find_package( Boost 1.40 COMPONENTS regex )
#if ( NOT SDL_FOUND )
#   message ( FATAL_ERROR "SDL not found!" )
#endif ( NOT SDL_FOUND )


link_directories ( ${Boost_LIBRARY_DIRS} 
					/usr/local/include/urg
					/usr/include
				 )
link_directories(${PCL_LIBRARY_DIRS})
include_directories ( ${Boost_INCLUDE_DIRS} 
						/usr/local/include/urg
						/usr/include
					)
include_directories(${PCL_INCLUDE_DIRS})

#include_directories(${OpenMRL_INCLUDE_DIRS})

add_definitions(${PCL_DEFINITIONS})

rosbuild_add_executable(bin_nodo_hokuyo src/nodo_hokuyo.cpp)
rosbuild_add_executable(bin_nodo_dynamixel src/nodo_dynamixel.cpp src/motordynamixel.cpp)
rosbuild_add_executable(bin_nodo_laser3D src/nodo_laser3D.cpp)
rosbuild_add_executable(bin_cliente src/cliente.cpp)
#rosbuild_add_executable(bin_client_dynamixel src/client_dynamixel.cpp)
rosbuild_add_executable(bin_cliente_hokuyo src/cliente_hokuyo.cpp)
rosbuild_add_executable(bin_sus_hokuyo src/sus_hokuyo.cpp)

target_link_libraries(bin_nodo_dynamixel 
	${LIBDXL_LIBRARIES}
	#tinyxml
)
target_link_libraries(bin_cliente
	${PCL_LIBRARIES}
	tinyxml
)
target_link_libraries(bin_nodo_laser3D ${PCL_LIBRARIES})
target_link_libraries (bin_nodo_hokuyo
   ${Boost_LIBRARIES}
   ${SDL_LIBRARY}
   #${SDLIMAGE_LIBRARY} # if using SDL_image, obviously
   SDLmain # Sadly not included in SDL_LIBRARY variable
   urg
   urg_common
   urg_connection
   urg_connection_sdl
   urg_coordinate
   urg_geometry
   urg_monitor
   urg_system
	#tinyxml
)










