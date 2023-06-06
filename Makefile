simfs_test: simfs_test.o simfs.a
	gcc -o $@ $^

simfs_test.o: simfs_test.c ctest.h
	gcc -c $< -DCTEST_ENABLE

simfs.a: block.o free.o image.o inode.o mkfs.o pack.o ls.o directory.o
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

pack.o: pack.c
	gcc -c $<

ls.o: ls.c
	gcc -c $<

directory.o: directory.c
	gcc -c $<

.PHONY: test

test: simfs_test
	./simfs_test

.PHONY: clean
	rm -f *.o