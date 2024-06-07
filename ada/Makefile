ADAFLAGS := -gnat05 -D out

hello: hello.adb
	gnat make -o out/hello $(ADAFLAGS) $<

arguments: arguments.adb
	gnat make -o out/arguments $(ADAFLAGS) $<

clean:
	gnat clean hello.adb arguments.adb
