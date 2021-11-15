cmake_minimum_required(VERSION 2.8.3)
project(icd_42_ros)

find_package(catkin REQUIRED
    roscpp
)
find_package(Boost REQUIRED COMPONENTS
    system
)

catkin_package(
    CATKIN_DEPENDS roscpp
)

# Capture the environment variable that says where the 42 library lives
SET(LOC42 "$ENV{LOCATION_42}")

SET(LIBS42
    42actuators.o
    42cmd.o
    42dynamics.o
    42environs.o
    42ephem.o
    42exec.o
    42fsw.o
    42gl.o
    42glut.o
    42gpgpu.o
    42init.o
    42ipc.o
    42nos3.o
    42perturb.o
    42report.o
    42sensors.o
    AcApp.o
    dcmkit.o
    envkit.o
    fswkit.o
    geomkit.o
    glkit.o
    iokit.o
    mathkit.o
    msis86kit.o
    nrlmsise00kit.o
    orbkit.o
    radbeltkit.o
    sigkit.o
    SimReadFromFile.o
    SimReadFromSocket.o
    SimWriteToFile.o
    SimWriteToSocket.o
    sphkit.o
    timekit.o
)
LIST(TRANSFORM LIBS42 PREPEND "${LOC42}/Object/")

include_directories(
    ${catkin_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

# Create a simple executable for the main 42 routine
add_executable(Main42 src/Main42.c)
target_link_libraries(Main42 ${LIBS42} glut GLU GL dl m pthread)

# Create our 42ToRos node
add_executable(IcdNode src/IcdNode.cpp)
target_link_libraries(IcdNode ${catkin_LIBRARIES} ${Boost_LIBRARIES})

# Install required resources from the 42 project's directory
install(
    DIRECTORY "${LOC42}/Model"
    DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
install(
    DIRECTORY "${LOC42}/Kit/Shaders"
    DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/Kit
)
install(
    DIRECTORY "${LOC42}/World"
    DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

# Install stuff from our project
install(
    TARGETS Main42 IcdNode
    RUNTIME DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
install(
    DIRECTORY launch
    DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
