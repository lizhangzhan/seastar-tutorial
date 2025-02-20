#include "json/json.h"
#include <iostream>

int main() {
  Json::Value root;
  root["name"] = "Bazel";
  root["version"] = 6.0;
  std::cout << root.toStyledString() << std::endl;
  return 0;
}
