#include <iostream>

#include "../compat.h"

using namespace std;
int main() {
  cout << "Sleep for 2 seconds." << endl;
  compat_sleep(2000);
  cout << "Try beeping for 1 second" << endl;
  compat_beep(1000, 1000);
  
  compat_cls();
  cout << "Did you catch that? I cleared the screen!" << endl;
  compat_color("fc");
  cout << "BOO! Alright, I,ll turn it back" << endl;
  compat_color("");

  cout << "Finally," << endl;
  compat_open("test.jpg");
  cout << "I'm out of tricks. Bye!" << endl;
  
  compat_pause();
  return 0;
}
