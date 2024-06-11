ADAFLAGS := -gnat05 -D out

hello: hello.adb
	gnat make -o out/hello $(ADAFLAGS) $<

arguments: arguments.adb
	gnat make -o out/arguments $(ADAFLAGS) $<

int: int.adb
	gnat make -o out/int $(ADAFLAGS) $<

chargender: chargender.adb
	gnat make -o out/chargender $(ADAFLAGS) $<

home: home.adb
	gnat make -o out/home $(ADAFLAGS) $<

future_value: finance/future_value_example.adb
	gnat make -o out/future_value $(ADAFLAGS) $<

clean:
	cd out && gnat clean hello.adb arguments.adb int.adb chargender.adb home.adb future_value.adb
