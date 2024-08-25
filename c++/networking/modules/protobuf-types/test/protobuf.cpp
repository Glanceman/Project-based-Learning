#include <iostream>
#include "protoMessage.pb.h"


int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    try {
        ProtoMessage::Message msg;
        msg.set_msg("Hello World");
        msg.set_val(689);
        std::cout << "protoMessage: " << msg.msg() << " " << msg.val() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "An exception occurred: " << e.what() << std::endl;
        google::protobuf::ShutdownProtobufLibrary();
        return 1;
    }

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}