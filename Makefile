# The directory of the compiler (ia-atmos/lang), already built.
LANG_DIR ?= $(abspath ../lang)
IUP       = $(CURDIR)/iup

.PHONY: all test study i0 clean

all: atmos-iup

$(IUP)/libiup.so:
	./get-iup.sh

# The compiler dedicated to the environment.
atmos-iup: iup.atm $(IUP)/libiup.so $(LANG_DIR)/atmos $(LANG_DIR)/build/libatmos.a
	$(MAKE) -s -C $(LANG_DIR) env OUT=$(CURDIR)/atmos-iup ENV=$(CURDIR)/iup.atm \
	    ENV_CFLAGS="-I$(IUP)/include" \
	    ENV_LDFLAGS="-L$(IUP) -liup -Wl,-rpath,$(IUP)"

# The tests, plain and under the address sanitizer.
test: atmos-iup
	tst/run.sh
	ATMOS_ASAN=1 tst/run.sh

# The C versions of the study, under the scripts of the Atmos versions.
study: $(IUP)/libiup.so
	study/run.sh

# The check of I0: needs Xvfb and ImageMagick (import).
i0: $(IUP)/libiup.so
	mkdir -p out
	gcc -Wall -o out/i0 tst/i0.c -I$(IUP)/include -L$(IUP) -liup -Wl,-rpath,$(IUP)
	xvfb-run -a -s "-screen 0 640x480x24" out/i0

clean:
	rm -rf atmos-iup out
