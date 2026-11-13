IUP = iup

.PHONY: i0

$(IUP)/libiup.so:
	./get-iup.sh

# The check of I0: needs Xvfb and ImageMagick (import).
i0: $(IUP)/libiup.so
	mkdir -p out
	gcc -Wall -o out/i0 tst/i0.c -I$(IUP)/include -L$(IUP) -liup -Wl,-rpath,$(CURDIR)/$(IUP)
	xvfb-run -a -s "-screen 0 640x480x24" out/i0
