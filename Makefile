all: lmc.o main.o
	gcc -Llibs -o main $^ -s -Os

%.o: %.c
	gcc -c $< -o $@ -s -Os -std=c11 -Wall -Wno-switch

clean:
	rm -f *.o main main.exe