cc = gcc	#gcc로 컴파일
target = main	#최종적으로 main파일을 생성
cflags = -W -g	#컴파일 옵션으로 W, g 사용
objects = main.o print_arr.o cal.o	#object 파일로 사용할 파일들

$(target) : $(objects)		#위에서 작성한 명령어를 통한 컴파일 명령
	$(cc) $(cflags) -o $(target) $(objects)

%.o : %.c			#$@는 .o, %<는 .c를 의미
	$(cc) $(cflags) -c -o $@ $<

main.o print_arr.o cal.o : Header.h	#Header.h를 include하는 object파일

.PHONY : clean		#clean파일이 존재할 경우를 대비해 사용
clean :			#make clean 을 입력하면 target파일과 object파일 삭제
	rm $(target) $(objects)
