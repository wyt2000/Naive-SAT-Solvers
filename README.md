# Naive-SAT-Solvers
基于 DPLL 和 CDCL，输入为 CNF 的布尔可满足性问题求解器。

## Requirement

cmake >= 3.16

## Build

```bash
mkdir -p build && cd build
cmake ..
make
```

## Usage

```bash
solver -in <cnfpath> [-a <algorithm>]
```

- 输入文件格式为 [DIMACS](http://www.satcompetition.org/2009/format-benchmarks2009.html) 。
- \<cnfpath\> 为输入文件路径，\<algorithm\> 为 dpll 或 cdcl，默认为 cdcl 。

## 算法解释

### DPLL

#### unitResolute(clauses, literal)

用文字 literal 对合取范式 clauses 做归约，即假定 literal 为真的情况下对 clauses 进行化简。

对于 clauses 的每一个子句：

- 如果该子句仅含 literal 的否定，则归约失败。

- 如果该子句含有 literal 的否定和其他文字，则在该子句中删除 literal 的否定。
- 如果该子句含有 literal，则在合取范式中删除该子句。

#### DPLL(clauses, assignments)

- 不停地选择 clauses 中的只含单个文字的子句，将其加入 assignments。并用此文字对 clauses 做归约，直到找不出只含单个文字的子句。

- 如果该过程中归约失败，则此问题无解。
- 如果 clauses 为空，则 assignments 即为满足此问题的赋值。

- 否则需要随机选取命题变元 p，分别将 p 和非 p 加入 clauses 后递归调用 DPLL，如果两个递归调用有任何一个有解，则原问题有解。

### CDCL

#### unitPropagate()

根据当前已有的赋值和子句，推出新的赋值。

对 clauses 的每一个子句：

- 如果该子句仅有一个文字没有被赋值，且其他文字都已经被赋了相反的值，则给这个仅有的文字赋对应的值。
- 如果所有文字都被赋了相反的值，说明当前的赋值有矛盾，需要回退（backJump）。
- 如果有某个命题变元分别在两个子句中被赋了相反的值，也说明当前的赋值有矛盾，需要回退。但此时可以通过去掉这两个字句中反号的两个命题变元，并将其他命题变元用或连接起来学习（learn）到一个新的子句。
- 如果不存在新的赋值，说明需要进行试探（decide）。

#### decide()

从未赋值的命题变元中选择一个赋值，如果没有未赋值的变量，说明该问题有解。

#### backJump()

当前赋值推出了矛盾，说明最近的试探是错的，但这个矛盾可能与前面的一些赋值无关，此时需要回退到尽可能早且能够推出该矛盾的位置。

由于该矛盾的发生直接导致了新子句被学习了，所以只需回退到之后推出赋值的命题变元都不在新子句中出现的试探位置即可，然后将原试探命题变元取反。

如果没有试探变元，但仍有矛盾需要回退，说明该问题无解。

#### CDCL()

- 不断进行 unitPropagate。
- 如果有矛盾，则利用学习到的子句进行 backJump。如果之前没有试探变元，说明该问题无解。
- 如果没矛盾且没有新的赋值，则进行试探赋值。如果所有命题变元均被赋值，说明该问题有解。

### 实现过程

各个模块的功能已经写到头文件的注释里了，不再赘述。

在实现 CDCL 的过程中，我首先采用了两种不同的试探命题变元的确定性启发式方法：

- MRV：优先赋值拥有最多相反赋值变量的子句中未赋值的变量，从而尽早产生矛盾，在[这里](https://github.com/wyt2000/Naive-SAT-Solvers/tree/8d576a279af283e63d25549987f1cb5a471a84a5) 。
- 选择出现频率最高的命题变元进行赋值，在[这里](https://github.com/wyt2000/Naive-SAT-Solvers/tree/77cb37d8ea0bb226341e92df0e675556f32fdcee) 。

但这两种确定性启发式方法均出现了某几个测试样例运行时间远超其他测试样例运行时间的情况，经过查阅[相关资料](https://www.cs.upc.edu/~oliveras/LAI/cdcl.pdf)我了解到 CDCL 会存在 heavy tail 的现象，即随着 backJump 次数的增加，求解总时间会突然大幅上升。

在采用随机选取试探命题变元的非确定性启发式算法，并结合 backJumping 次数达到一定限度时重新开始（restart）推理的策略，该问题得到解决。

但这种方法可能会导致算法不完备（必须 backJumping 超过一定次数才能推出正确的结果），所以我采用类似迭代加深的深度优先搜索的方法，不断增加 backJumping 的次数上界。

### 测试集

从 [SATLAB](https://www.cs.ubc.ca/~hoos/SATLIB/benchm.html) 上面找了一些较小的[测试样例](https://www.cs.ubc.ca/~hoos/SATLIB/Benchmarks/SAT/DIMACS/AIM/aim.tar.gz)，在根目录下运行 test.sh 即可查看测试结果。

### 性能与比较

运行全部 72 个测试样例共需 13.351s ，显著快于 GitHub 上的另一个[朴素实现](https://github.com/sukrutrao/SAT-Solver-CDCL)（33.332s）。

比 [cadical](https://github.com/arminbiere/cadical)（0.537s）慢好多。。。

### 不足

- 未实现 forget 。
- 对于大样例（几千个命题变元和几千个 cnf ），跑不出来。cadical 可以在一段时间之后得到结果。