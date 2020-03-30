# austinsimpson
Straightforward implementation of the K-Nearest Neighbors algorithm in Qt/C++. Main classes are the templated TrainingData and NearestNeighborsMatcher. The usage is fairly simple:
  1) Initialize your list of training data with their respective classification.
  2) Define a distance function for your training data (the templated type is T, so your function will need to return a non-negative real number from two T's.
  3) Use the matcher on a new point of type T.
  
The demo UI shows how to use this matcher on a subset of the real plane. The user can click to add training points from three different classes (labeled 0, 1, 2 for expediency). User can also modify how many neighbors are used in the calculation.
