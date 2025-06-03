# RStarTreeProject

A repository for the implementation of the R* tree for the undergraduate CompSci course Database Technologies at csd.auth.gr

## ToDo

- [ ] Buffer
  - [ ] Data Block structure
  - [ ] Tree Node Block Structure
  - [ ] getNextBlockID() ????
- [ ] CLI
  - [ ] initialize
  - [ ] set files path
  - [ ] load data
  - [ ] create buffer with d-buff-size and t-buff-size (or one unified buffer size???)
  - [ ] CRUD
- [ ] R-tree
  - [ ] Search
  - [ ] Insert: **will be overwritten in R\***
    - [ ] ChooseLeaf 
    - [ ] AdjustTree
  - [ ] Delete
    - [ ] FindLeaf
    - [ ] CondenseTree
  - [ ] Split: **will be overwritten in R\***
    - [ ] QSplit
    - [ ] PickSeeds
    - [ ] PickNext
  - [ ] Queries Support:
    - [ ] Search
    - [ ] k-nn
    - [ ] range
    - [ ] insert one
    - [ ] build from 0
    - [ ] delete
    - [ ] skyline
- [ ] R*-tree
  - [ ] Insert
    - [ ] Insert
    - [ ] ChooseSubtree
    - [ ] OverflowTreatment
    - [ ] ReInsert
  - [ ] Split
    - [ ] Split
    - [ ] ChooseSplitAxis
    - [ ] ChooseSplitIndex
- [ ] Data
