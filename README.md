BPlusTree
=========

Implementation of B+ Tree (In-memory)

### Installation
1. Get the code.
  ```Shell
  git clone https://github.com/BestSonny/BplusTree.git
  cd BplusTree
  ```

2. Build and run the code. Either with Cmake or Make
  ```Shell
  make
  ./treesearch file_name
  ```

   or

  ```Shell
  mkdir build
  cd build
  cmake ../
  make
  ./treesearch file_name
  ```
  
 ### sample file
 1. We provide a sample file (located at `./data` folder)
 
 ```
Insert(3.55,Value1)
Insert(4.01,Value10)
Insert(39.56,Value2)
Insert(-3.95,Value23)
Insert(-3.91,Value54609)
Insert(3.55,Value67)
Insert(0.02,Value98)
Search(3.55)
Search(-3.91,30.96)
Insert(3.26,Value56089)
Insert(121.56,Value1234)
Insert(-109.23,Value43234)
Search(3.71)
```
2. Run `./treesearch ../data/sample.txt`, the result should be 

 ```
Value1, Value67
(-3.91, Value54609),(0.02, Value98),(3.55, Value1),(3.55, Value67),(4.01, Value10)
Null
```

  
