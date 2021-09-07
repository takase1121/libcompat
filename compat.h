#include <iostream>

#if defined (_WIN32)
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <termios.h>
#endif

#define hex2num(c) (c >= 'A') ? (c - 'A' + 10) : (c - '0')

#if defined(__APPLE__)
#define OPEN_PROGRAM "open"
#else
#define OPEN_PROGRAM "xdg-open"
#endif


#if defined(_WIN32)
#define compat_sleep(ms) Sleep(ms)
#define compat_beep(frequency, duration) Beep(frequency, duration)
#else
#define compat_sleep(ms) usleep(ms * 1000)
#define compat_beep(frequency, duration) __compat_beep_ansi(frequency, duration)

static char MS_TO_ANSI[] = {
  30, // BLACK
  34, // BLUE
  32, // GREEN
  36, // CYAN
  31, // RED
  35, // PURPLE
  33, // YELLOW
  37, // WHITE
  90, // GRAY
  94,
  92,
  96,
  91,
  95,
  93,
  97
};
#endif

#if defined(_WIN32) && ! defined(COMPAT_FORCE_ANSI_ESCAPE)
#define compat_cls() system("cls")
#else
#define compat_cls() (std::cout << "\033[2J\033[H" << std::flush)
#endif


void compat_color(const char *color) {
#if defined(_WIN32) && ! defined(COMPAT_FORCE_ANSI_ESCAPE)
  std::string str = std::string("color ") + color;
  system(str.c_str());
#else
  if (color[0] == '\0') {
    std::cout << "\033[0m";
    return;
  }
  char bgcolor = hex2num(std::toupper(color[0]));
  std::cout << "\033[" << (short) MS_TO_ANSI[bgcolor] + 10 << "m";
  if (color[1] != '\0') {
    char fgcolor = hex2num(std::toupper(color[1]));
    std::cout << "\033[" << (short) MS_TO_ANSI[fgcolor] << "m";
  }
#endif
}


void compat_open(const char *filename) {
#if defined(_WIN32)
  std::string fname = std::string("\"") + filename + std::string("\"");
  system(fname.c_str());
#else
  // this double fork is equal to shell's &, but better
  pid_t pid = fork();
  if (pid == 0) {
    setsid();
    pid_t pid2 = fork();
    if (pid2 == 0) {
      execlp(OPEN_PROGRAM, OPEN_PROGRAM, filename, NULL);
      perror("cannot execlp()");
    } else if (pid2 > 0) {
      exit(0);
    } else {
      perror("cannot fork()");
    }
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  } else {
    perror("cannot fork()");
  }
#endif
}


void compat_pause() {
  // the choice is between a 20 year old API
  // or lots of code
#if defined(_WIN32)
  _getch();
#else
  char buf = 0;
  struct termios old = { 0 };
  if (tcgetattr(0, &old) < 0)
    perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0)
    perror("cannot tcsetattr() ICANON");
  if (read(0, &buf, 1) < 0)
    perror("cannot read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0)
    perror("cannot tcsetattr() ~ICANON");
#endif
}


void __compat_beep_ansi(int frequency, int length) {
  //we'll at least respect the duration
  std::cout << '\x07' << std::flush;
  compat_sleep(length);
}


