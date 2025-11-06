#main(server)

cmake_minimum_required(VERSION 3.15)


include_directories(
	${PROJECT_SOURCE_DIR}/lib
	${CMAKE_CURRENT_LIST_DIR}/Common
#	${PROJECT_SOURCE_DIR}/message
	${PROJECT_SOURCE_DIR}/tool/include
)
file(GLOB_RECURSE SRC_H_FILES ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB_RECURSE SRC_CPP_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#link_directories(${PROJECT_SOURCE_DIR}/tool)
#link_directories(${PROJECT_BINARY_DIR}/lib/Debug)
#link_directories(${PROJECT_BINARY_DIR}/tool)
#link_directories(${PROJECT_BINARY_DIR}/message/Debug)
add_executable(server ${SRC_CPP_FILES} ${SRC_H_FILES})
#target_link_libraries(server cjson libserverbase libprotobufd messagelib)
target_link_libraries(server pthread)
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gateserver)
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameserver)
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/loginserver)
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/dbserver)
file(GLOB_RECURSE DB_FILE ${CMAKE_CURRENT_LIST_DIR}/DB/*.*)
file(GLOB_RECURSE LOG_FILE ${CMAKE_CURRENT_LIST_DIR}/Log/*.*)
file(GLOB_RECURSE LOGIN_FILE ${CMAKE_CURRENT_LIST_DIR}/Login/*.*)
file(GLOB_RECURSE MAIL_FILE ${CMAKE_CURRENT_LIST_DIR}/Mail/*.*)
file(GLOB_RECURSE REDIS_FILE ${CMAKE_CURRENT_LIST_DIR}/Redis/*.*)
file(GLOB_RECURSE SCENE_FILE ${CMAKE_CURRENT_LIST_DIR}/Scene/*.*)
file(GLOB_RECURSE COMMON_FILE ${CMAKE_CURRENT_LIST_DIR}/Common/*.*)

source_group("db"	FILES ${DB_FILE})
source_group("log"	FILES ${LOG_FILE})
source_group("login" FILES ${LOGIN_FILE})
source_group("mail" FILES ${MAIL_FILE})
source_group("redis" FILES ${REDIS_FILE})
source_group("scene" FILES ${SCENE_FILE})
source_group("common" FILES ${COMMON_FILE})
source_group("main"	FILES main.cpp)
