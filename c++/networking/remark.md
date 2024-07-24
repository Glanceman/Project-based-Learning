## Note

```cpp
        socket.wait(socket.wait_read);
        size_t bytes = socket.available(); //problem we dont know when the byte finish transfering
        std::cout<<"\nBytes Available: "<<bytes;
        if(bytes>0){
            std::vector<char> vBuffer(bytes);
            socket.read_some(asio::buffer(vBuffer.data(),vBuffer.size()));
            for(auto c:vBuffer){
                std::cout<<c;
            }
        }
```
Issue: we dont know when the byte finish
```cpp
std::vector<char> vBuffer (10*1024);
void  GrabSomeData(asio::ip::tcp::socket& socket){
    socket.async_read_some(asio::buffer(vBuffer.data(),vBuffer.size()),
        [&](std::error_code ec, std::size_t length){
            if(!ec){
                std::cout<<"\n\nRead "<<length << "bytes\n\n";
                for(int i=0; i<length;i++){
                    std::cout<<vBuffer[i];
                }
                GrabSomeData(socket);
            }
        }
    );
}
int main(){
    /*.....*/
    GrabSomeData(socket);
    /*....*/
}
``` 
