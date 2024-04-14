.PHONY: sftpdir clean

BIN=sftpdir

CCFLAGS := -g3 -O0 -W -Wall -std=gnu11 -pipe -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-variable -Wno-unused-but-set-variable -I.
LDFLAGS := -L. -lssh2 -lssl -lcrypto -lz

vpath %.h .
vpath %.c .

sftpdir: sftpdir.c
	@echo "test compatibility on different platform ..."
	gcc $(CCFLAGS) $^ -o $(BIN) $(LDFLAGS)
clean:
	@echo "clean resources ..."
	@rm -f $(BIN)
