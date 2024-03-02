assembler: main.c ienc.c strop.c map.c
	gcc -o assembler main.c

# main.o: main.c
# 	gcc -Wall -O -c main.c

# strop.o: strop.c
# 	gcc -Wall -O -c strop.c

# ienc.o: ienc.c
# 	gcc -Wall -O -c ienc.c

clean:
	rm -f assember