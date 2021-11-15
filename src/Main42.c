/** @file 42Main.c
 *  @author R. Nick Vandemark
 *  @brief A simple main to handle starting a 42 program in a ROS environment.
 *  @version 0.0.1
 *  @date 2021-11-13
 */

#include <stdio.h>

int exec(int argc, char** argv);

int main(int argc, char** argv) {
    if (2 < argc) {
        // Roslaunch will add additional args, play it safe and remove them
        char* fixed_argv[2];
        fixed_argv[0] = argv[0];
        fixed_argv[1] = argv[1];
        return exec(2, fixed_argv);
    } else {
        // Otherwise, let 42 try to run with what we have
        return exec(argc, argv);
    }
}
