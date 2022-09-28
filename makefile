all:
	@windres.exe icon.rc icon.o
	@cc -o win_on_top *.c icon.o -mwindows