LINKS=-lraylib -lm
CLANG=clang
CLANGFLAGS=-O2 -ggdb -Wall -Wextra
LIBS=animation.c cats/cat.c timer.c emojis/emoji.c

codingcats: codingcats.c animation.c 
	$(CLANG) $(CLANGFLAGS) $(LIBS) codingcats.c -o codingcats $(LINKS)

.PHONY: run clean
run: 
	./codingcats &
clean:
	rm -rf codingcats

