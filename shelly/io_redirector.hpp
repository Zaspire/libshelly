#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

#ifndef SHELLY_IO_REDIRECTOR_HPP_
#define SHELLY_IO_REDIRECTOR_HPP_

class IORedirector {
 public:
  IORedirector(
      const std::string& input_path, const std::string& output_path)
      : _fin(input_path), _fout(output_path),
        _stream_buffer_cin(std::cin.rdbuf()),
        _stream_buffer_cout(std::cout.rdbuf()) {
    std::cin.rdbuf(_fin.rdbuf());
    std::cout.rdbuf(_fout.rdbuf());
  }
  ~IORedirector() {
    std::cin.rdbuf(_stream_buffer_cin);
    std::cout.rdbuf(_stream_buffer_cout);
  }
 private:
  std::ifstream _fin;
  std::ofstream _fout;

  std::streambuf* _stream_buffer_cin;
  std::streambuf* _stream_buffer_cout;
};

#endif
