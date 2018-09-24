#!/bin/bash
mkdir results
echo "============== STARTING FILE 1 OF 6 =============="
./triangulate toys/points1k.node toys/points1k.node example.txt
cd results/
mkdir 1k
cd ..
mv *_out.* results/1k/
mv example.txt results/1k/

echo "============== STARTING FILE 2 OF 6 =============="
./triangulate toys/points2k.node toys/points2k.node example.txt
cd results/
mkdir 2k
cd ..
mv *_out.* results/2k/
mv example.txt results/2k/

echo "============== STARTING FILE 3 OF 6 =============="
./triangulate toys/points3k.node toys/points3k.node example.txt
cd results/
mkdir 3k
cd ..
mv *_out.* results/3k/
mv example.txt results/3k/

echo "============== STARTING FILE 4 OF 6 =============="
./triangulate toys/points4k.node toys/points4k.node example.txt
cd results/
mkdir 4k
cd ..
mv *_out.* results/4k/
mv example.txt results/4k/

echo "============== STARTING FILE 5 OF 6 =============="
./triangulate toys/points5k.node toys/points5k.node example.txt
cd results/
mkdir 5k
cd ..
mv *_out.* results/5k/
mv example.txt results/5k/

echo "============== STARTING FILE 6 OF 6 =============="
./triangulate toys/points6k.node toys/points6k.node example.txt
cd results/
mkdir 6k
cd ..
mv *_out.* results/6k/
mv example.txt results/6k/

echo "============== DONE =============="
