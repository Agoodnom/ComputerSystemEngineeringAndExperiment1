cc=g++
cflags = -W -g
target = main

objects = main.o Array.o RangeArray.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

%.o : %.c
	$(cc) $(cflags) -c -o $@ $<


.PHONY : clean
clean :
	rm $(target) $(objects)
