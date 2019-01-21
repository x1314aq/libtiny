CC := gcc
LD := bfd
C_FLAGS := -Wall -Wextra -g -O0
INC_DIR := $(abspath ../include)
LINK_DIR := $(abspath ../build)
LIBS := tiny

rbtree_test: rbtree_test.c
	$(CC) $(C_FLAGS) -I$(INC_DIR) -L$(LINK_DIR) -fuse-ld=$(LD) -Wl,-rpath=$(LINK_DIR) $< -l$(LIBS) -o $@

clean:
	@rm -rf *.o
	@rm -rf rbtree_test
