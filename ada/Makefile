ADAFLAGS := -gnat05 -D obj
MKDIR_P := mkdir -p

hello: hello.adb
	$(MKDIR_P) obj
	$(MKDIR_P) bin
	gnat make $(ADAFLAGS) -o bin/hello $<

arguments: arguments.adb
	$(MKDIR_P) obj
	$(MKDIR_P) bin
	gnat make $(ADAFLAGS) -o bin/arguments $<

int: int.adb
	$(MKDIR_P) obj
	$(MKDIR_P) bin
	gnat make $(ADAFLAGS) -o bin/int $<

chargender: chargender.adb
	$(MKDIR_P) obj
	$(MKDIR_P) bin
	gnat make $(ADAFLAGS) -o bin/chargender $<

home: home.adb
	$(MKDIR_P) obj
	$(MKDIR_P) bin
	gnat make $(ADAFLAGS) -o bin/home $<

future_value: finance/future_value_example.adb
	$(MKDIR_P) obj
	$(MKDIR_P) bin
	gnat make $(ADAFLAGS) -o bin/future_value $<

clean:
	gnat clean -D obj hello.adb arguments.adb int.adb chargender.adb home.adb future_value.adb
	cd bin && gnat clean hello.adb arguments.adb int.adb chargender.adb home.adb future_value.adb
