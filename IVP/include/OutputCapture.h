#ifndef OUTPUT_CAPTURE_H
#define OUTPUT_CAPTURE_H
#include <iostream>
#include <fstream>
#include <streambuf>
#include <filesystem>
#include <string>

// class OutputCapturer : public std::streambuf {
// public:
//     OutputCapturer(std::streambuf* buf, std::ostream& stream) : originalBuffer(buf), targetStream(stream) {}
// protected:
//     virtual std::streamsize xsputn(const char* s, std::streamsize n) {
//         originalBuffer->sputn(s, n);
//         targetStream.write(s, n);
//         return n;
//     }
//     virtual int overflow(int c = EOF) {
//         if (c != EOF) {
//             originalBuffer->sputc(c);
//             targetStream.put(c);
//         }
//         return c;
//     }
// private:
//     std::streambuf* originalBuffer;
//     std::ostream& targetStream;
// };

// void outputcapture(const std::string& outputpath,void (*func)()) {
//     std::filesystem::path filepath(outputpath);
//     std::filesystem::path dir = filepath.parent_path();
//     if (!dir.empty() && !std::filesystem::exists(dir)) {
//         if (!std::filesystem::create_directories(dir)) {
//             std::cerr << "Could not create directories: " << dir << std::endl;
//             return;
//         }
//     }
//     std::ofstream logFile(outputpath);
//     if (!logFile.is_open()) {
//         std::cerr << "Failed to open log file." << std::endl;
//         return;
//     }
//     std::streambuf* originalBuffer = std::cout.rdbuf();
//     OutputCapturer capturer(originalBuffer, logFile);
//     std::cout.rdbuf(&capturer);
//     func();
//     std::cout.rdbuf(originalBuffer);
//     logFile.close();
// }
// void outputcapture(const std::string& outputpath,const std::string& solver,const int& minorder,const int& maxorder,void (*func)(const std::string&,const int&,const int&)) {
//     std::filesystem::path filepath(outputpath);
//     std::filesystem::path dir = filepath.parent_path();
//     if (!dir.empty() && !std::filesystem::exists(dir)) {
//         if (!std::filesystem::create_directories(dir)) {
//             std::cerr << "Could not create directories: " << dir << std::endl;
//             return;
//         }
//     }
//     std::ofstream logFile(outputpath);
//     if (!logFile.is_open()) {
//         std::cerr << "Failed to open log file." << std::endl;
//         return;
//     }
//     std::streambuf* originalBuffer = std::cout.rdbuf();
//     OutputCapturer capturer(originalBuffer, logFile);
//     std::cout.rdbuf(&capturer);
//     func(solver,minorder,maxorder);
//     std::cout.rdbuf(originalBuffer);
//     logFile.close();
// }

class OutputCapturer : public std::streambuf {
public:
    OutputCapturer(std::streambuf* buf, std::ostream& stream) 
        : originalBuffer(buf), targetStream(stream) {}
protected:
    virtual std::streamsize xsputn(const char* s, std::streamsize n) override {
        originalBuffer->sputn(s, n);
        targetStream.write(s, n);
        return n;
    }
    virtual int overflow(int c = EOF) override {
        if (c != EOF) {
            originalBuffer->sputc(c);
            targetStream.put(c);
        }
        return c;
    }
private:
    std::streambuf* originalBuffer;
    std::ostream& targetStream;
};

template<typename Func, typename... Args>
void outputcapture(const std::string& outputpath, Func&& func, Args&&... args) {
    std::filesystem::path filepath(outputpath);
    std::filesystem::path dir = filepath.parent_path();
    if (!dir.empty() && !std::filesystem::exists(dir)) {
        if (!std::filesystem::create_directories(dir)) {
            std::cerr << "Could not create directories: " << dir << std::endl;
            return;
        }
    }
    std::ofstream logFile(outputpath);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file." << std::endl;
        return;
    }
    std::streambuf* originalBuffer = std::cout.rdbuf();
    OutputCapturer capturer(originalBuffer, logFile);
    std::cout.rdbuf(&capturer);

    std::forward<Func>(func)(std::forward<Args>(args)...);  // Forward the arguments to the function

    std::cout.rdbuf(originalBuffer);
    logFile.close();
}

#endif