# Euler Tour Tree in C

This project is a C implementation of rooted Euler tour trees,
using splay trees as the underlying tree data structure.
We implement the standard dynamic rooted tree operations of `find_root(v)`,
`cut(v)`, `link(u, v)`, and `connectivity(v, w)`, as well as
`subtree_aggregate(v)` for `min`, `max`, `sum`, and `size`.
We also implemented a few extra operations for modifying the tree:
`point_update(u, new_value)` and `subtree_update(u, increment_amount)`.

See the [project writeup](writeup.pdf) for details.

## Usage

To compile the code, run `make`.

In particular, the resulting program `repl` presents a REPL for
experimenting with Euler tour trees using the operations listed above.

## Code Overview

* `euler_tour_tree.{c,h}` implement the Euler tour trees operations
  described above.
* `splay_tree.{c,h}` implement the typical splay tree operations and
  also support range queries for augmentations.
* `augmentations.h` controls which subtree augmentations to build in support
  for (gaining higher efficiency if fewer augmentations are desired).
* `repl.c` implements a REPL for user interaction.
* Other files implement testing and benchmarking.
