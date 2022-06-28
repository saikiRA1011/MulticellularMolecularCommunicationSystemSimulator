CC := g++-11
CFLAGS := -std=c++20 -Wall -Wextra -fopenmp -O2
OBJS := Vec3.o Cell.o Simulation.o CellList.o UserSimulation.o

nowdate:=$(shell date +%Y%m%d_%H%M)

FULL_ARCHIVE := 1# 0がfalse 1がtrue

SimMain: src/SimMain.cpp $(OBJS)
	$(CC) -o SimMain $(CFLAGS) $(OBJS) src/SimMain.cpp

Vec3.o: src/utils/Vec3.cpp
	$(CC) -c $(CFLAGS) src/utils/Vec3.cpp

Cell.o: src/Cell.cpp
	$(CC) -c $(CFLAGS) src/Cell.cpp

Simulation.o: src/Simulation.cpp src/SimulationSettings.hpp
	$(CC) -c $(CFLAGS) src/Simulation.cpp

UserSimulation.o: src/Simulation.cpp src/UserSimulation.cpp
	$(CC) -c $(CFLAGS) src/UserSimulation.cpp

SegmentTree.o: src/SegmentTree.cpp
	$(CC) -c $(CFLAGS) src/SegmentTree.cpp

CellList.o: src/CellList.cpp
	$(CC) -c $(CFLAGS) src/CellList.cpp

VariableRatioCellList.o: src/VariableRatioCellList.cpp
	$(CC) -c $(CFLAGS) src/VariableRatioCellList.cpp

seg-test: SegmentTree.o src/SegTest.cpp
	$(CC) -o SegTest $(CFLAGS) SegmentTree.o src/SegTest.cpp

all: clean SimMain run convert open

run: SimMain
	./SimMain

clean: 
	rm -f *.o

data-cleanup:
	rm -f result/*
	rm -f image/*
	rm -f video/out.mp4

png:
	python3 src/convert_tools/create_image.py

video:
	python3 src/convert_tools/img2video.py

convert:
	python3 src/convert_tools/create_image.py
	python3 src/convert_tools/img2video.py

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
	@echo "make : build the program"
	@echo "make run : run the program"
	@echo "make convert : convert to mp4"
	@echo "make open : open out.mp4"
	@echo "make all : build and run, convert to, open mp4"
	@echo "make data-archive : result data to zip archive"
	@echo "make archive-restore date=[YYYYMMDD_HHMM] : restore archive files"
	@echo "make clean : remove all object files(*.o)"
	@echo "make data-cleanup : remove all data files(*.txt, *.png, out.mp4)"
	@echo "make archive-cleanup : remove all archive files(archive_*)"