# clusterator
Given input data, finds clusters within an epsilon.

cluster dim clusters filename [...]

dim - Integer that defines the number of columns in the data.
clusters - Integer that defines the number of clusters to look for.
filename - String or list of strings (space delimited) that are the input files (comma delimited)

ex: cluster 1 5 example_data/input*

Cluster uses the k-means (https://en.wikipedia.org/wiki/K-means_clustering) algorithm to find clusters within the input data.  K-means has linear complexity and should converge to a solution in O(n).  Here I've leveraged an established implementation in OpenCV.

Current Issues and Thoughts on improvement:
* The first two entries in the cv::Mat are corrupted after passing by value from the load function.  Something to look into.
* K-means requires that you have to define the number of clusters up front.  Not ideal but may fit for resource constrained devices.
* K-means has to be initialized with random cluster coordinates, resulting in different answers for the same data. Probably not an issue for real world tracking but something to keep in mind if using this for mapping (ie. point clouds or hazard locations will not be in the same pace twice).
* Should consider using DBSCAN which better handles outliers and does not add them to a cluster as K-means may. (https://en.wikipedia.org/wiki/DBSCAN)

Depends:
OpenCV 3.2 or greater

Build:

Linux:

cmake .
make 

Windows:
:)

