# TP2

Hamza Bouihi, Bilal Akliai

## Part one: preparatory work

### Q.1.1

For this question, the implementation of `speck48_96` is in [second_preim_48.c](src/second_preim_48.c#L4) and the test verifying the correctness of our implementation with the test values provided in [BSS+13,
App. C](doc/specifications.pdf), by using the (already implemented) function `test_vector_okay` is in [test_speck48_96.c](test/test_speck48_96.c).

### Q.1.2

For this question, the implementation of `speck48_96_inv` is in [second_preim_48.c](src/second_preim_48.c#L33) and the test verifying the correctness of our implementation is in [test_speck48_96_inv.c](test/test_speck48_96_inv.c).

### Q.1.3

For this question, the implementation of `cs48_dm` is in [second_preim_48.c](src/second_preim_48.c#L66) and the test verifying the correctness of our implementation is in [test_speck48_96_inv.c](test/test_cs48_dm.c).

### Q.1.4

In order to find a point `fp` such that `cs48_dm(m,fp) == fp`, we need to solve the equation `speck48_96(m, fp) == 0`. To do this, we can apply the inverse function `speck48_96_inv` to both sides, yielding `fp == speck48_96_inv(m, 0)`

The implementation of `get_cs48_dm_fp` is in [second_preim_48.c](src/second_preim_48.c#L113) and the test verifying the correctness of our implementation is in [test_speck48_96_inv.c](test/test_get_cs48_dm_fp.c).

## Part two: the attack

### Q.2.1

#### Choices for the Data structure

To store and search values efficiently, we can create a table where the chaining values serve as keys, and the corresponding messages act as values. However, this method comes with a significant limitation: excessive memory usage. To ensure each chaining value has a unique position in the table, we would require a table size of $2^{48}$ times the size of a message block ($4\times24$). This is far beyond the memory capacity of most computers.

Therefore, we need to explore a method to optimize and reduce memory usage.

A common solution is to use a hashmap, which requires much less memory than a full table. However, hashmaps can still have collisions, so a good hashing function is essential. In our case, we can use the chaining value as the key (modulo a smaller number to reduce the number of buckets) and the message as the value. By minimizing the number of keys mapping to the same bucket, we can maintain an efficient storage and search process.

Hashmaps have a size defined by their number of buckets $N_b$, which we need to determine based on $N$. To simplify computations, $N_b$ is rounded to a power of 2, allowing bitwise operations instead of expensive modulo calculations. The ratio $\dfrac{N}{N_b}$ is the load factor, which affects collision rates. A higher load factor increases collisions, so we aim for a balance between memory usage and performance. Based on recommendations (e.g.[this post](https://www.quora.com/Whats-a-good-load-factor-to-use-when-creating-a-hash-table)), a load factor of 0.75 offers a good compromise.

### Implementation

- The implementation of `find_exp_mess` is in [second_preim_48.c](src/second_preim_48.c#L250).

- You can fin in [second_preim_48.h](src/second_preim_48.h#L127), the definition of the Hashmap structure in this file and in [second_preim_48.c](src/second_preim_48.c) the implementation of `init_hash()`, `add_hash()`, `get_hash()`, and `free_hash()`.

- All the test of correctness ans performances on `find_exp_mess` are implemented in [test_em.c](test/test_em.c)

#### Performances

For this part, we will disregard the cost of initializing the hashmap.

Let us denote:

- $C$ the cost of `find_exp_mess`
- $C_1$ the cost for the generation of the [Step 1](src/second_preim_48.c#248)
- $C_2$ the cost for the generation of the [Step 2](src/second_preim_48.c#255)

Since `cs48_dm` outputs 48-bit messages, each random message has a probability of $\dfrac{N}{2^{48}}$ of its fixed point already existing in the hashmap. Assuming uniform distribution of messages and hashing, the average number of messages we need to generate in order to find a collision is $\dfrac{2^{48}}{N}$ and we have $C = C_1 \times N + C_2 \times \dfrac{2^{48}}{N}$.

In order to minimize $C$, we can define: $f: x \mapsto C_1x + \dfrac{C_2\times2^{48}}{x}$ and search for its minimum which is reaching its min in $N = \sqrt{\dfrac{C_2\times2^{48}}{C_1}} = 2^{24}\times\sqrt{\dfrac{C_2}{C_1}}$.

To approximate $\sqrt{\dfrac{C_2}{C_1}}$, we ran [find_c1_c2](test/test_em.c#L37) and we obtained $N \approx 2^{24} \times 1.4697 \approx 24657474$.

Now, we can compare experimentally which value of $N$ is the best for our implementation. The code is in [compare_perf](test/test_em.c#L92)

After plotting the values on a graph, we obtain:

```mermaid
---
config:
    xyChart:
        width: 900
        height: 600
    themeVariables:
        xyChart:
            titleColor: "#ff0000"
---
xychart-beta
    title "Performances"
    x-axis "Value of N (in Millions)" 10 --> 30
    y-axis "Execution time (in $)" 5 --> 20
    line [11.515870, 12.482899, 9.008464, 12.488521, 8.879034, 10.233930, 13.543986, 10.633675, 12.145674, 12.250121, 12.769462, 11.723171, 12.226785, 16.274649, 13.123087, 12.545838, 13.086305, 15.654178, 14.947266, 15.475547]
```

In practice ,the best of $N$ seems to be around $14000000$ so we will take this default value.

### Q.2.2

- The implementation of `attack` is in [second_preim_48.c](src/second_preim_48.c#L296).

- The implementation of the tests and measure of performences are in [test_attack.c](test/test_attack.c). You'll have remove the `\\` in the `main` function if you want to run it during the  

```shell
make test 
```

The attack to find a second preimage for a message of $N'$ blocks of $4*32$ bits follows the same principle as before but requires colliding with one of the intermediate chaining values when hashing the target message.

For a message with $fourlen = 2^{20}$, there are $2^{20}$ blocks of $4 \times 32$ bits. Once we generate the expandable message, the process to find the collision block is similar to before but uses `cs48_dm(cm,fp)` instead of `cs48_dm(m1,IV)`. The cost remains $C = C_1 \times N + C_2 \times \dfrac{2^{48}}{N}$, with the constraint $N < fourlen$ , as the maximum number of chaining values is limited by the number of blocks.

Here, with $fourlen=2^{18}$, we set $N = 2^{18}$ to store all chaining values and try collisions with all of them. Using previously estimated $C_1$ and $C_2$, the attack's cost will be $\approx C_1 \times 2^{18} + C_2 \times \dfrac{2^{48}}{2^{18}} \approx  0.191564\times 2^{18} + 0.281541 \times \dfrac{2^{48}}{2^{18}} \approx 3 \times 10^{8}$ which is about 5 minutes on my computer.
In practice, we have to wait $72.155243s$.

The most expensive step is finding a collision with a chaining value. Parallelizing this search could reduce runtime by a factor equal to the number of CPU cores or distributed machines used.
