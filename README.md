# GCN Matrix Multiplicator

## Info file format

Info file must follow this format

```
Adjacency height
Adjacency width
X height
X width
Weight height
Weight width
```

## Adjacency matrix file format

A file must follow csr format

```
Row index
Colomn index
```

## X & Weight matrix file format

X & W file must follow matrix format

```
Row 0
Row 1
...
Row n
```

## How to run

This program can run followed by step

1) complie code : `make`
2) run program : `gcn info.txt a.txt x.txt w.txt`
3) you can remove object file : `make clean`

* if you want to generate random matrix, run mat_gen.py in mat_gen folder
1) go to mat_gen folder : `cd mat_gen`
2) run program : `python mat_gen.py matrix_height matrix_width`
	ex) if you want to make 128 * 128 matrix : `python mat_gen.py 128 128`