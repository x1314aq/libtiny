CC := gcc
LD := bfd
C_FLAGS := -Wall -Wextra -g -O0
INC_DIR := $(abspath ../include)
LINK_DIR := $(abspath ../build)
LIBS := tiny
APPS := rbtree_test avl_test

.PHONY: all
all: $(APPS)

rbtree_test: rbtree_test.c
	$(CC) $(C_FLAGS) -I$(INC_DIR) -L$(LINK_DIR) -Wl,-rpath $(LINK_DIR) $< -l$(LIBS) -o $@

avl_test: avl_test.c
	$(CC) $(C_FLAGS) -I$(INC_DIR) -L$(LINK_DIR) -Wl,-rpath $(LINK_DIR) $< -l$(LIBS) -o $@

.PHONY: clean
clean:
	@rm -rf *.o
	@rm -rf $(APPS)
	@rm -rf *.dSYM
