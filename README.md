binfmt
======

Simple message serialization.

Examples
--------


    #include <binfmt/binfmt.hpp>
    
    int main() {

        binfmt::Buffer buffer(1024);
        
        buffer.write("hello world");
        buffer.write(uint32_t(1));
        
        std::string msg;
        uint32_t num;

        buffer.read(msg);
        buffer.read(num);

        return 0;
    }
