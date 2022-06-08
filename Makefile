CC := g++-11
CFLAGS := -std=c++20 -Wall -Wextra -fopenmp
OBJS := Vec3.o Cell.o Simulation.o

nowdate:=$(shell date +%Y%m%d_%H%M)

FULL_ARCHIVE := 1# 0がfalse 1がtrue

all: SimMain.cpp $(OBJS)
	$(CC) -o SimMain $(CFLAGS) $(OBJS) SimMain.cpp

Vec3.o: Vec3.cpp
	$(CC) -c $(CFLAGS) Vec3.cpp

Cell.o: Cell.cpp
	$(CC) -c $(CFLAGS) Cell.cpp

Simulation.o: Simulation.cpp
	$(CC) -c $(CFLAGS) Simulation.cpp

run: SimMain
	./SimMain

clean: 
	rm -f *.o

data-cleanup:
	rm -f result/*
	rm -f image/*
	rm -f video/out.mp4

png:
	python3 create_image.py

video:
	python3 img2video.py

convert:
	python3 create_image.py
	python3 img2video.py

open:
	open video/out.mp4

ifeq (FULL_ARCHIVE,1)
	ARCHIVE_COMMAND := cp image/* archive_$(nowdate)/image\
	cp video/* archive_$(nowdate)/video
endif

data-archive:
	mkdir archive_$(nowdate)
	mkdir archive_$(nowdate)/result
	mkdir archive_$(nowdate)/image
	mkdir archive_$(nowdate)/video

	cp result/* archive_$(nowdate)/result/

	$(ARCHIVE_COMMAND)

	zip -r archive_$(nowdate).zip archive_$(nowdate)

	rm -rf archive_$(nowdate)

archive-cleanup:
	rm -r archive_*

archive-restore:
	unzip archive_$(date).zip

	@touch result/result.txt
	@touch image/image.txt

	rm image/*
	rm result/*

	@touch archive_$(date)/result/result.txt
	@touch archive_$(date)/image/image.txt
	@touch archive_$(date)/video/video.txt

	mv archive_$(date)/image/* image/
	mv archive_$(date)/result/* result/
	mv archive_$(date)/video/* video/

	@rm result/result.txt
	@rm image/image.txt
	@rm video/video.txt

	rm -r archive_$(date)

help:
	@echo "Usage: make [target]"
	@echo "make : builds the program"
	@echo "make clean : removes all object files(*.o)"
	@echo "make data-cleanup : removes all data files(*.txt, *.png, out.mp4)"