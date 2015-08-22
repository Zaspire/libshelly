#ifndef LIBSHELLY_STRING_UTILS_HPP_KKLJKJKLBBV
#define LIBSHELLY_STRING_UTILS_HPP_KKLJKJKLBBV

namespace shelly {

std::string ReplaceAll(const std::string& a_str, const std::string& from, const std::string& to) {
  std::string str(a_str);
  for (size_t start_pos = str.find(from); start_pos != std::string::npos; start_pos = str.find(from)) {
    str.replace(start_pos, from.length(), to);
  }
  return str;
}

std::vector<std::string> Split(const std::string &sentence, char separator) {
  std::vector<std::string> tokens;

  size_t pos = 0, start = 0;
  while ((pos = sentence.find(separator, start)) != std::string::npos) {
    if (pos >= 1 && start != pos) {
      tokens.push_back(sentence.substr(start, pos - start));
    }
    start = pos + 1;
  }
  if (start != sentence.size()) {
    tokens.push_back(sentence.substr(start, sentence.size() - start));
  }

  return tokens;
}

}

#endif
