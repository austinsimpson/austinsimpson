# austinsimpson
Straightforward implementation of the K-Nearest Neighbors algorithm in Qt/C++. Main classes are the templated TrainingData and NearestNeighborsMatcher. The usage is fairly simple:
  1) Initialize your list of training data with their respective classification.
  2) Define a distance function for your training data (the templated type is T, so your function will need to return a non-negative real number from two T's.
  3) Use the matcher on a new point of type T.
  

