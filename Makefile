# indicate how the object files are to be created
CC         := g++
# CC		   := icc
CFLAGS     := -g -O3 -fopenmp
# CFLAGS     := -g -O3 -axCORE-AVX2  -qopenmp

OBJECT_FILES_BRUTE := brute_nw.o helper.o
OBJECT_FILES_OPTIMIZED_1 := optimized_nw_1.o helper.o
OBJECT_FILES_OPTIMIZED_2 := optimized_nw_2.o helper.o
OBJECT_FILES_OPTIMIZED_3 := optimized_nw_3.o helper.o
OBJECT_FILES_OPTIMIZED_4 := optimized_nw_4.o helper.o

brute_nw: $(OBJECT_FILES_BRUTE)
	$(CC) $(CFLAGS) $(OBJECT_FILES_BRUTE) -o brute_nw

optimized_nw_1: $(OBJECT_FILES_OPTIMIZED_1)
	$(CC) $(CFLAGS) $(OBJECT_FILES_OPTIMIZED_1) -o optimized_nw_1

optimized_nw_2: $(OBJECT_FILES_OPTIMIZED_2)
	$(CC) $(CFLAGS) $(OBJECT_FILES_OPTIMIZED_2) -o optimized_nw_2

optimized_nw_3: $(OBJECT_FILES_OPTIMIZED_3)
	$(CC) $(CFLAGS) $(OBJECT_FILES_OPTIMIZED_3) -o optimized_nw_3

optimized_nw_4: $(OBJECT_FILES_OPTIMIZED_4)
	$(CC) $(CFLAGS) $(OBJECT_FILES_OPTIMIZED_4) -o optimized_nw_4

helper:
	$(CC) $(CFLAGS) helper.o -o helper

clean:
	rm *.o