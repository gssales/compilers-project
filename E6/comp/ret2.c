
int foo() { return 1; }
int bar() { return 2; }
int baz() { return 3; }
int moz() { return 4; }

int main() {
	if (bar() == 1) {
		return baz();
	}
	return foo();
}