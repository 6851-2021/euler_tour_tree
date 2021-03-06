all: test bench bench-gen repl

test: splay_tree.c euler_tour_tree.c euler_tour_tree_test.c
	gcc -o euler_tour_test splay_tree.c euler_tour_tree.c euler_tour_tree_test.c -O3 -std=c11 -march=native

bench: benchmark.c splay_tree.c euler_tour_tree.c
	gcc -o benchmark benchmark.c splay_tree.c euler_tour_tree.c -O3 -march=native

bench-gen: benchmark_gen.c splay_tree.c euler_tour_tree.c
	gcc -o benchmark_gen benchmark_gen.c splay_tree.c euler_tour_tree.c -O3 -march=native

repl: splay_tree.c euler_tour_tree.c repl.c
	gcc -o repl splay_tree.c euler_tour_tree.c repl.c -O3 -march=native
