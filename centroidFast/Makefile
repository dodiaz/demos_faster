CC=gcc
CFLAGS=-fopenmp -std=gnu99 -O2

centroid_updated.x: centroid_updated_new.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f centroid_updated.x
