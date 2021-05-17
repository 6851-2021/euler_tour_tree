splay: splay_tree.c
	gcc -o splay_tree splay_tree.c

euler: splay_tree.c euler_tour_tree.c
	gcc -o euler_tour splay_tree.c euler_tour_tree.c

test: splay_tree.c euler_tour_tree.c euler_tour_tree_test.c
	gcc -o euler_tour_test splay_tree.c euler_tour_tree.c euler_tour_tree_test.c -O3 -std=c11 -march=native

bench: benchmark.c splay_tree.c euler_tour_tree.c
	gcc -o benchmark benchmark.c splay_tree.c euler_tour_tree.c -g -march=native

bench-gen: benchmark_gen.c splay_tree.c euler_tour_tree.c
	gcc -o benchmark_gen benchmark_gen.c splay_tree.c euler_tour_tree.c -g -march=native