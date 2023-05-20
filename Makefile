simfs_test: simfs_test.o simfs.a
	gcc -o $@ $^

simfs_test.o: simfs_test.c ctest.h
	gcc -c $<

simfs.a: block.o free.o image.o inode.o mkfs.o
	ar rcs $@ $^

block.o: block.c
	gcc -c $<

free.o: free.c
	gcc -c $<

image.o: image.c
	gcc -c $<

inode.o: inode.c
	gcc -c $<

mkfs.o: mkfs.c
	gcc -c $<


.PHONY: test

test: simfs_test
	./simfs_test
