## libcompat, the stupidest library ever written

> It can't get stupider than this.
 
 This library bridges some "inconsistencies" between systems.
 It has evil code in it, and that's it. I won't bother fixing anything.

 ### Install
 Just put `compat.h` into your project and include it.
 ```cpp
#include "compat.h"

int main() {
	compat_sleep(1000);
	return 0;
}
 ```

 ### API Docs

#### `#define COMPAT_FORCE_ANSI_ESCAPE`
By default, terminal stuff is managed in Windows via `system()` and the associated programs. This is the case because
Windows does not suport ANSI escape codes (or very limited) until Windows 10 1809.

If you're sure the code will run in newer versions of Windows, add `#define COMPAT_FORCE_ANSI_ESCAPE` before including `compat.h`.
This will make use of ANSI escape codes and fix a bug where colors can't be resetted properly.

#### `compat_sleep(ms)`
This is a macro to `Sleep()` on Windows and `usleep(ms * 1000)` on other platforms.

#### `compat_beep(frequency, duration)`
This is a macro to `Beep()` on Windows and triggers a terminal bell on other platforms.

In the future, a ALSA / Core Audio backend will be added for Linux and MacOS to simulate Windows `Beep()`

#### `compat_cls()`
This is a macro to `system("cls")` on Windows and uses ANSI escape code in other platforms.

> `COMPAT_FORCE_ANSI_ESCAPE` applies to this macro.

#### `compat_color(const char *color)`
This function calls `system("color")` on Windows and uses ANSI escape codes in other platforms.

The arguments follow Windows's [`color` command][1]: 2 characters each representing background and foreground color respectively. If an empty string is provided, the colors are reset.

> `COMPAT_FORCE_ANSI_ESCAPE` applies to this function.

#### `compat_open(const char *filename)`
This function uses [`start` semantics][2] to open a file with its default program on Windows. It uses `open` in MacOS and `xdg-open` in other platforms.

#### `compat_pause()`
This function uses `_getch()` on Windows.
On other platforms, it disables canonical and echo on current TTY to read raw input.

> Note: This is not guaranteed to work on platforms other than Windows, Linux and MacOS.


[1]: https://ss64.com/nt/color.html
[2]: https://ss64.com/nt/start.html
