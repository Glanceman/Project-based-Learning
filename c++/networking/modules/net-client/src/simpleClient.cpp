#include <iostream>

#include <olc_net.h>

enum class CustomMsgTypes : uint32_t
{
    FireBullet,
    MovePlayer
};

// class CustomClient : public Olc::Net::Client_Interface<CustomMsgTypes> {

// public:
//     CustomClient(){
//     };
// public:
//     void FireBullet(int x, int y) {
//         Olc::Net::Message<CustomMsgTypes> msg;
//         msg.header.id = CustomMsgTypes::FireBullet;
//         msg << x << y;
//         // Send(msg);
//     }
// };


int main(){
    // CustomClient c;
    // c.FireBullet(1, 1);
    // c.Connect("abc",80);
    std::cout<<"\ndone";
    return 0;
}