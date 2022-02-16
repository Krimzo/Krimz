#include "Engine/Engine.h"


int main() {
	win.start = Start;
	win.update = Update;
	win.end = End;
	win.startNew(kl::ivec2(1600, 900), "Krimz Editor", true, false);
}
