ADAFLAGS := -gnat05 -D out
MKDIR_P := mkdir -p

hello: hello.adb
	$(MKDIR_P) out
	gnat make $(ADAFLAGS) -o out/hello $<

arguments: arguments.adb
	$(MKDIR_P) out
	gnat make $(ADAFLAGS) -o out/arguments $<

int: int.adb
	$(MKDIR_P) out
	gnat make $(ADAFLAGS) -o out/int $<

chargender: chargender.adb
	$(MKDIR_P) out
	gnat make $(ADAFLAGS) -o out/chargender $<

home: home.adb
	$(MKDIR_P) out
	gnat make $(ADAFLAGS) -o out/home $<

future_value: finance/future_value_example.adb
	$(MKDIR_P) out
	gnat make $(ADAFLAGS) -o out/future_value $<

clean:
	cd out && gnat clean hello.adb arguments.adb int.adb chargender.adb home.adb future_value.adb
