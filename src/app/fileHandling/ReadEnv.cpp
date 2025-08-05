#include "IO.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class ReadEnv{
    public:
        std::string getenv(){
            std::string env_path = ".env";
            IO io(env_path);
            std::fstream f_stream = io.getFileStream();
            
            if (!f_stream.is_open()) {
                std::cerr << "Unable to open .env file" << std::endl;
                return "0"; // Return default value
            }
            
            std::stringstream buffer;
            buffer << f_stream.rdbuf();
            std::string content = buffer.str();
            
            // Remove any trailing newline or whitespace
            while (!content.empty() && (content.back() == '\n' || content.back() == '\r' || content.back() == ' ')) {
                content.pop_back();
            }
            
            f_stream.close();
            return content.empty() ? "0" : content;
        }    
};