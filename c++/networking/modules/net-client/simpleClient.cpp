#include <iostream>

#include <olc_net.h>

enum class CustomMsgTypes : uint32_t
{
    FireBullet,
    MovePlayer
};

int main(){
    Olc::Net::Message<CustomMsgTypes> msg;
    msg.header.id=CustomMsgTypes::FireBullet;
    int a = 1;
    bool b =true;
    float c =3.141;

    struct{
        float x;
        float y;
    }d[5];
    std::cout<<a<<" "<<b<<" "<<c<<std::endl;
    msg<<a<<b<<c<<d;

    a=99;
    b=false;
    c=99.0f;

    msg>>d>>c>>b>>a;
    std::cout<<a<<" "<<b<<" "<<c;
    std::cout<<"\ndone";
    return 0;
}