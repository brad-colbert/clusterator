#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace cv;
using namespace std;

typedef unsigned int uint_32;

Mat loadFile(const char* filename, const uint_32 datadim = 2)
{
    vector<float> data;

    ifstream file(filename);
    if(file.is_open())
    {
        string token;

       while(getline(file, token, ','))
        {
            data.push_back(atof(token.c_str()));
        }
    }

    Mat mat(data.size()/datadim, datadim, CV_32F, data.data());

    cout << mat << endl;
    
    return mat;
}

bool parseArgs(int argc, char** argv, uint_32& dim, uint_32& clusters, vector<string>& filenames)
{
    if(argc < 4)
        return false;

    dim = atoi(argv[1]);
    clusters = atoi(argv[2]);
    for(uint_32 i = 0; i < argc-3; i++)
    {
        string filename = argv[i+3];
        filenames.push_back(filename);
    }

    return true;
}

// cluster dim clusters filename [...]
// ex: cluster 1 5 example_date/input*
// Finds clusters in the input data.  Produces a 2D graph (image) of the clusters, color coding each.
int main(int argc, char** argv)
{
    // Normally there should be a more in-depth command line parsing  here
    // but for the sake of brevity, an assumption is made that the first argument is
    // the data dimensionality, the next is the number of clusters, and the rest
    // are the data filenames.
    Mat mypoints;
    uint_32 dim, numClusters;
    vector<string> filenames;
    if( parseArgs(argc, argv, dim, numClusters, filenames) )
    {
        // Load the files into the CV matricies
        for_each(filenames.begin(), filenames.end(), [&](string& filename) {
            Mat mat(loadFile(filename.c_str(), dim));
            if(!mypoints.rows)
                mypoints = mat;
            else
                mypoints.push_back(mat);
            } );

        // Colors for each cluster.  At the moment a maximum of 5 can be represented.
        Scalar colorTab[] =
        {
            Scalar(0, 0, 255),
            Scalar(0,255,0),
            Scalar(255,100,100),
            Scalar(255,0,255),
            Scalar(0,255,255)
        };
        Mat img(500, 500, CV_8UC3);
        RNG rng(12345);
        for(;;)
        {
            int k, clusterCount = 5;
            int sampleCount = mypoints.rows;
            Mat labels;
            clusterCount = MIN(clusterCount, sampleCount);
            Mat centers;

            kmeans(mypoints, clusterCount, labels,
                   TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 10, 1.0),
                   3, KMEANS_PP_CENTERS, centers);

            img = Scalar::all(0);

            for( uint_32 i = 0; i < sampleCount; i++ )
            {
                int clusterIdx = labels.at<int>(i);
                Point ipt(mypoints.at<float>(i), mypoints.at<float>(i));
                circle( img, ipt, 2, colorTab[clusterIdx], FILLED, LINE_AA );
            }

            imshow("clusters", img);

            // Print out the centers of the clusters
            cout << centers << endl;

            char key = (char)waitKey();
            if( key == 27 || key == 'q' || key == 'Q' ) // 'ESC'
                break;
        }
        return 0;
    }

    return -1;
}