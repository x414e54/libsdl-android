libsdl-android
==============

This is a custom Android project template for libSDL 2 with libSDL 2 patches to enable static linking and fix or workaround some bugs.

Just copy the source folder on to of the SDL source code.

Implementation details: http://en.wildservices.net/2013/10/making-libsdl-2-apps-on-android.html

TODO:

- The template code needs to be cleaned up: use `return` instead of `exit()`, make proper initialisation/finalisation calls, reduce code size a bit.
- The main native thread does not receive SDL_QUIT when the OS wants to kill the app ([bug 2041][11]).
- If we exit from the main native thread, the app does not terminate.
- Port the iOS SDL tests to Android.
