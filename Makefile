splay: splay_tree.c
	gcc -o splay_tree splay_tree.c

euler: splay_tree.c euler_tour_tree.c
	gcc -o euler_tour splay_tree.c euler_tour_tree.c