ADAFLAGS := -gnat05

echoserver: echoserver.adb
	gnatmake -o echoserver $(ADAFLAGS) $<

clean:
	gnatclean echoserver
