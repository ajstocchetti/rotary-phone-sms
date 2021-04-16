#include <string>

std::string theMessage = "";

std::string charEntered(char c) {
  theMessage.push_back(c);
  return theMessage;
}

void textHungUp() {
  theMessage = "";
}
void textPickedUp() {
  theMessage = "";
}
