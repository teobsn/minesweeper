// Game config
const bool doRevealNeighbors = true; // experimental
const bool winByRevealing = true; // whether or not to let the player win if they have discovered all squares
const bool winByMarking = false; // whether or not to let the player win if they have marked all bombs

// Input config
const int KeybindUp       =  119;    // W
const int KeybindLeft     =   97;    // A
const int KeybindDown     =  115;    // S
const int KeybindRight    =  100;    // D
const int KeybindMark     =   32;    // Space


// Windows specific bindings
#ifdef _WIN32
const int KeybindHit      =   13;    // Enter
#endif

// Linux specific bindings
#ifdef linux
const int KeybindHit	  =   10;    // Enter
#endif

// Android specific bindings
#ifdef __ANDROID__
const int KeybindHit      =   10;    // Enter
#endif
