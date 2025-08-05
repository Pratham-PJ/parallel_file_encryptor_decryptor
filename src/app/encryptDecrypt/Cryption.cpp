#include "Cryption.hpp"
#include "../processes/Task.hpp"
#include "../fileHandling/ReadEnv.cpp"
#include <ctime>
#include <iomanip>

int executeCryption(const std::string& taskData) {
<<<<<<< HEAD
    try {
        Task task = Task::fromString(taskData);
        ReadEnv env;
        std::string envKey = env.getenv();
        int key = std::stoi(envKey);
        
        if (task.action == Action::ENCRYPT) {
            task.f_stream.clear();
            task.f_stream.seekg(0, std::ios::beg);
            
            std::string content;
            std::string line;
            while (std::getline(task.f_stream, line)) {
                content += line + "\n";
            }
            if (!content.empty() && content.back() == '\n') {
                content.pop_back(); // Remove the extra newline
            }
            
            for (char& ch : content) {
                ch = (ch + key) % 256;
            }
            
            task.f_stream.clear();
            task.f_stream.seekp(0, std::ios::beg);
            task.f_stream << content;
            task.f_stream.flush();
            
        } else {
            task.f_stream.clear();
            task.f_stream.seekg(0, std::ios::beg);
            
            std::string content;
            std::string line;
            while (std::getline(task.f_stream, line)) {
                content += line + "\n";
            }
            if (!content.empty() && content.back() == '\n') {
                content.pop_back(); // Remove the extra newline
            }
            
            for (char& ch : content) {
                ch = (ch - key + 256) % 256;
            }
            
            task.f_stream.clear();
            task.f_stream.seekp(0, std::ios::beg);
            task.f_stream << content;
            task.f_stream.flush();
=======
    Task task = Task::fromString(taskData);
    ReadEnv env;
    std::string envKey = env.getenv();
    int key = std::stoi(envKey);
    if (task.action == Action::ENCRYPT) {
        char ch;
        while (task.f_stream.get(ch)) {
            ch = (ch + key) % 256;
            task.f_stream.seekp(-1, std::ios::cur);
            task.f_stream.put(ch);
        }
        task.f_stream.close();
    } else {
        char ch;
        while (task.f_stream.get(ch)) {
            ch = (ch - key + 256) % 256;
            task.f_stream.seekp(-1, std::ios::cur);
            task.f_stream.put(ch);
>>>>>>> 463f0fb66506374217f8c0edd6b3825fe311733f
        }
        
        task.f_stream.close();
        
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        std::cout << "Exiting the encryption/decryption at: " << std::put_time(now, "%Y-%m-%d %H:%M:%S") << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in executeCryption: " << e.what() << std::endl;
        return 1;
    }
<<<<<<< HEAD
=======
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    std::cout << "Exiting the encryption/decryption at: " << std::put_time(now, "%Y-%m-%d %H:%M:%S") << std::endl;
>>>>>>> 463f0fb66506374217f8c0edd6b3825fe311733f
    
    return 0;
}