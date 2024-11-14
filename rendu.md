# TP2

Hamza Bouihi, Bilal Akliai

## Part one: preparatory work

### Q.1.1

For this question, the implémentation of `speck48_96` is in [second_preim_48.c](src/second_preim_48.c) and the test verifying the correctness of our implementation with the test values provided in [BSS+13,
App. C](doc/specifications.pdf), by using the (already implemented) function `test_vector_okay` is in [test_speck48_96.c](test/test_speck48_96.c).

### Q.1.2

For this question, the implémentation of `speck48_96_inv` is in [second_preim_48.c](src/second_preim_48.c) and the test verifying the correctness of our implementation is in [test_speck48_96_inv.c](test/test_speck48_96_inv.c).

### Q.1.3

For this question, the implémentation of `cs48_dm` is in [second_preim_48.c](src/second_preim_48.c) and the test verifying the correctness of our implementation is in [test_speck48_96_inv.c](test/test_cs48_dm.c).

### Q.1.4

In order to find a point `fp` such that `cs48_dm(m,fp) == fp`, we need to solve the equation `speck48_96(m, fp) == 0`. To do this, we can apply the inverse function `speck48_96_inv` to both sides, yielding `fp == speck48_96_inv(m, 0)`

The implémentation of `get_cs48_dm_fp` is in [second_preim_48.c](src/second_preim_48.c) and the test verifying the correctness of our implementation is in [test_speck48_96_inv.c](test/test_get_cs48_dm_fp.c).

## Part two: the attack

### Q.2.1

#### Choices for the Data structure

#### Performances

### Q.2.2
