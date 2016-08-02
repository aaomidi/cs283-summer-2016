/*
 * hw3.c
 *
 *  Created on: Mar 10, 2013
 *      Author: lou
 */

#include "hw3.h"

/**
 * Loads an image (stored as an IplImage struct) for each
 * filename provided.
 * @param numImages		The number of images to load
 * @param filenames		A list of strings of the filenames to load
 */
IplImage **loadImages(int numImages, char **fileNames) {
    IplImage **rv;
    int i;

    if (NULL == (rv = malloc(numImages * (sizeof(IplImage *))))) {
        fprintf(stderr, "Not enough memory.\n");
        exit(0);
    }

    for (i = 0; i < numImages; i++) {
        char *fileName = fileNames[i];

        void *img = cvLoadImage(fileName, CV_LOAD_IMAGE_COLOR);
        rv[i] = img;
        if (rv[i] == NULL) {
            fprintf(stderr, "Could not load image %s\n", fileName);
            return NULL;
        }
    }
    return rv;
}

/**
 * Computes the distance between two colors (stored as CvScalars).
 *
 * Given a CvScalar c, you can access the blue, green, and red (BGR) elements
 * via c.val[0], c.val[1], and c.val[2], respectively.
 *
 * This function computes the distance between two colors as the euclidean
 * distance between the two BGR vectors.
 *
 * @see http://en.wikipedia.org/wiki/Euclidean_distance
 *
 * @param c1	The first color
 * @param c2	The second color
 * @returns		The euclidean distance between the two 3-d vectors
 */
double colorDistance(CvScalar c1, CvScalar c2) {
    double d = 0;
    int i;

    for (i = 0; i < 3; i++) {
        double v1 = c1.val[i];
        double v2 = c2.val[i];

        double val = v2 - v1;

        d += pow(val, 2);
    }
    if (d == 0) {
        return 0;
    }
    return sqrt(d);
}

/**
 * Splits up an image into numColumns by numRows sub-images and returns the results.
 *
 * @param src	The source image to split
 * @param numColumns	The number of columns to split into
 * @param numRows 		The number of rows to split into
 * @returns				A numColumns x numRows array of IplImages
 */
IplImage **getSubImages(IplImage *src, int numColumns, int numRows) {
    int cellWidth, cellHeight, y, x, i;
    IplImage **rv;
    CvSize s = cvGetSize(src);

    // Compute the cell width and the cell height
    cellWidth = s.width / numColumns;
    cellHeight = s.height / numRows;

    // Allocate an array of IplImage* s
    rv = malloc(sizeof(IplImage *) * numColumns * numRows);
    if (rv == NULL) {
        fprintf(stderr, "Could not allocate rv array\n");
    }

    // Iterate over the cells
    i = 0;
    for (y = 0; y < s.height; y += cellHeight)
        for (x = 0; x < s.width; x += cellWidth) {
            // Create a new image of size cellWidth x cellHeight and
            // store it in rv[i]. The depth and number of channels should
            // be the same as src.
            rv[i] = cvCreateImage(cvSize(cellWidth, cellHeight), src->depth,
                                  src->nChannels);
            if (rv[i] == NULL) {
                fprintf(stderr, "Could not allocate image %d\n", i);
            }

            // set the ROI of the src image
            cvSetImageROI(src, cvRect(x, y, cellWidth, cellHeight));

            // copy src to rv[i] using cvCopy, which obeys ROI
            cvCopy(src, rv[i], NULL);

            // reset the src image roi
            cvResetImageROI(src);

            // increment i
            i++;
        }

    // return the result
    return rv;
}

/**
 * Finds the CvScalar in colors closest to t using the colorDistance function.
 * @param t		 	The color to look for
 * @param scolors	The colors to look through
 * @param numColors	The length of scolors
 * @returns			The index of scolors that t is closest to
 * 					(i.e., colorDistance( t, scolors[result]) <=
 * 					colorDistance( t, scolors[i]) for all i != result)
 */
int findClosest(CvScalar t, CvScalar *scolors, int numColors) {
    int rv = 0, i;
    double d, m = colorDistance(t, scolors[0]);

    for (i = 1; i < numColors; i++) {
        d = colorDistance(t, scolors[i]);
        if (d < m) {
            rv = i; // Store result
            m = d; // Store minimum value
        }
    }

    return rv;
}

/**
 * For each image provided, computes the average color vector
 * (represented as a CvScalar object).
 *
 * @param images	The images
 * @param numImages	The length of images
 * @returns 		An numImages length array of CvScalars where rv[i] is the average color in images[i]
 */
CvScalar *getAvgColors(IplImage **images, int numImages) {
    CvScalar *rv;
    if (NULL == (rv = malloc(sizeof(CvScalar) * numImages))) {
        fprintf(stderr, "Not enough memory.\n");
        exit(0);
    }

    int i;

    for (i = 0; i < numImages; i++) {
        rv[i] = cvAvg(images[i], 0);
    }

    return rv;
}

/**
 * Given an ordered list of images (iclosest), creates a
 * numColumns x numRows grid in a new image, copies each image in, and returns the result.
 *
 * Thus, if numColumns is 10, numRows is 5, and each iclosest image is 64x64, the resulting image
 * would be 640x320 pixels.
 *
 * @param iclostest		A numColumns x numRows list of images in row-major order to be put into the resulting image.
 * @param numColumns  	Number of horizontal cells
 * @param numRows		Number of vertical cells
 */
IplImage *stitchImages(IplImage **iclosest, int numColumns, int numRows) {
    int i, j, z;

    CvSize size = cvGetSize(iclosest[0]);

    int cellWidth = size.width;
    int cellHeight = size.height;

    int width = cellWidth * numColumns;
    int height = cellHeight * numRows;


    IplImage *img = cvCreateImage(cvSize(width, height), iclosest[0]->depth, iclosest[0]->nChannels);

    z = 0;
    for (i = 0; i < height; i += cellHeight) {
        for (j = 0; j < width; j += cellWidth) {
            cvSetImageROI(img, cvRect(j, i, cellWidth, cellHeight));
            cvCopy(iclosest[z], img, NULL);
            cvResetImageROI(img);
            z++;
        }
    }

    return img;
}
