#if ENABLE_TRACE
#include<cpptrace/cpptrace.hpp>
#endif
using namespace std;

void DumpTraceback() {
#if ENABLE_TRACE
	cpptrace::generate_trace().print();
#endif
}

void throwWarning(string text) {
	cerr << "[Warning] " << text << endl;
	cerr << "    Thrown in callback \"" << currentCallback << "\" of archetype \"" << currentArchetype << "\"" << endl;
}

void throwError(string text) {
	cerr << "\e[31m[Error]\e[0m " << text << endl;
	cerr << "    Thrown in callback \"\e[32m" << currentCallback << "\e[0m\" of archetype \"\e[32m" << currentArchetype << "\e[0m\"" << endl;
	cerr << endl;
	DumpTraceback();
	exit(3);
}
