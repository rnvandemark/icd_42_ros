/** @file IcdNode.cpp
 *  @author R. Nick Vandemark
 *  @brief The core interface between a 42 simulation, outputting data on a
 *  socket, and the ROS network.
 *  @version 0.0.1
 *  @date 2021-11-13
 */

#include <string>
#include <array>
#include <vector>
#include <algorithm>

#include <ros/ros.h>

#include <boost/asio.hpp>

namespace ba = boost::asio;
namespace bs = boost::system;

int main(int argc, char** argv) {
    ros::init(argc, argv, "icd_42_ros_node");
    ros::NodeHandle n;

    // The node requires three ROS parameters, failure if not given
    std::string host;
    int port;
    int read_freq;
    if (n.getParam("host", host)
            && n.getParam("port", port)
            && n.getParam("read_freq", read_freq)) {
        ROS_INFO_STREAM("Using socket " << host
                                        << ":"
                                        << port
                                        << ", reading at "
                                        << read_freq
                                        << "Hz");
    } else {
        ROS_FATAL_STREAM("One or more socket parameters not set.");
        return -1;
    }

    // Our return code
    int rc = 0;

    // Create socket connection
    ba::io_service io_service;
    ba::ip::tcp::socket socket(io_service);
    socket.connect(ba::ip::tcp::endpoint(ba::ip::address::from_string(host),
                                         port));

    // ACK message
    const std::array<char, 4> msg_ack = { 'A', 'c', 'k', '\n' };
    // End packet
    const std::array<char, 6> msg_eof = { '\n', '[', 'E', 'O', 'F', ']' };

    // Reused socket data
    std::array<char, 512> buff;
    std::vector<char> data;
    bs::error_code cc;

    // Poll for available data and spin the node at the specified freq
    // Loop while 1. ROS master is available, 2. Connection code is unset,
    // 3. Connection code is set and was successful
    ros::Rate rate(read_freq);
    bool do_main_loop = ros::ok();
    while (do_main_loop) {
        // Check if there is data available, skip for now if none
        if (0 != socket.available()) {
            // Start reading a new chunk of data
            data.clear();
            bool done = false;
            while (do_main_loop && !done) {
                // Receive nb number of bytes, only continue on no error
                const std::size_t nb = socket.receive(ba::buffer(buff), 0, cc);
                do_main_loop = (cc == bs::errc::success);
                if (do_main_loop) {
                    // Insert the contents of buffer into the total data chunk
                    data.insert(data.end(), buff.begin(), buff.begin() + nb);
                    // Search for EOF, only bother searching in the buffer's
                    // contents as well as a small buffer to account for if EOF
                    // was split between two reads
                    const std::size_t search_idx = std::max(
                        0,
                        static_cast<int>(data.size() - nb - msg_eof.size()));
                    const auto eof_idx = std::search(data.begin() + search_idx,
                                                     data.end(),
                                                     msg_eof.begin(),
                                                     msg_eof.end());
                    // We're done with this message if we found EOF
                    done = (eof_idx != data.end());
                } else {
                    // Set an error return code
                    rc = 1;
                }
            }
            // Send back an ACK, only continue if there was no error and that
            // the ACK was sent as expected
            do_main_loop = ((4 == socket.write_some(ba::buffer(msg_ack), cc))
                            && (cc == bs::errc::success));
            if (do_main_loop) {
                // TODO: handle data
            } else if (1 != rc) {
                // Set an error return code
                rc = 2;
            }
        }

        // Spin node and sleep for previously specified period, even if there
        // was no data this loop
        if (do_main_loop) {
            ros::spinOnce();
            rate.sleep();
        }
    }

    // Capture any error closing the socket
    socket.close(cc);
    if (cc != bs::errc::success) {
        ROS_WARN_STREAM("Error received closing the socket: " << cc);
    }

    return rc;
}
