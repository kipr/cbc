fb_print: fb.c fb_print.cpp font.c
	arm-linux-g++ -O2 -g -Wall $^ -o $@

upgrade: upgrade.cpp fb.c font.c
	arm-linux-g++ -g -Wall $^ -o $@

test: test.c fb.c font.c
	arm-linux-gcc -g -Wall $^ -o $@

install: mount.sh fb_print
	scp $^ root@`../../filesystem/chumby-target`:/mnt/kiss

install_filesystem: fb_print
	rsync -a $^ $(DEST)

dumpfont: dumpfont.cpp
	g++ -Wall dumpfont.cpp -o dumpfont

raw2txt: raw2txt.cpp
	g++ -Wall raw2txt.cpp -o raw2txt

genfont: genfont.cpp
	g++ -Wall genfont.cpp -o genfont

font.c: genfont
	./genfont > font.c

clean:
	rm -rf genfont fb_print font.c
