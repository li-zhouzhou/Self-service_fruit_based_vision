#include "fruit.h"
int myprocess::process(Mat src,int weight)
{
    if (src.data == NULL)
    {
        cout << "û���ҵ���Ӧ��ͼƬ" << endl;
    }
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
   
    medianBlur(src_gray, blur_gray, 3);
    return mysoble(src, src, weight);


}
int myprocess::mysoble(Mat& src, Mat first_image, int weight)
{
      Mat kennlx = (Mat_<float>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
      Mat kennly = (Mat_<float>(3, 3) << -1, 0,1, -2, 0, 2, -1, 0, 1);
      filter2D(src, soble_x, -1, kennlx, Point(-1, -1), 0, 4);
      filter2D(src, soble_y, -1, kennly, Point(-1, -1), 0, 4);
      addWeighted(soble_x, 0.5, soble_y,0.5,0, soble_image);
      return myfindcourours(soble_image, first_image, weight);
   
}
int myprocess::myfindcourours(Mat& src, Mat first_image,int weight)
{
    int x, y;
    //���ˮ������
    vector<string>myclass;
    vector<Mat>imagearray;
    Mat mast = Mat::zeros(src.size(), src.type());
    Mat new_image; 
    vector<vector<int>>mycolor = { {11,43,46,25,255,255},{32,43,46,77,255,255},{0,43,46,10,255,255} };
    for (int num = 0; num < mycolor.size(); num++)   //����hsv���зָ��һ��ѭ���ֱ��㽶 ���� ����
    {
        Mat hsv_image = first_image.clone();
        Mat mask;
        cvtColor(hsv_image, new_image, COLOR_BGR2HSV);
        Scalar lowerb(mycolor[num][0], mycolor[num][1], mycolor[num][2]);
        Scalar upperb(mycolor[num][3], mycolor[num][4], mycolor[num][5]);
        // cout << hmin << "," << smin << "," << vmin << "," << hmax << "," << smax << "," << vmax << endl;
        inRange(new_image, lowerb, upperb, mask);    //inrange�������Խ��ж�ֵ�� 
        //imshow(to_string(num), mask);
        imagearray.push_back(mask);
    }
    for (int j = 0; j < imagearray.size();j++)
    {
        Canny(imagearray[j], canny_image, 10, 70, 3, true);
        
        Mat kennl = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
        dilate(canny_image, canny_image, kennl);
        erode(canny_image, canny_image, kennl);
        imshow(to_string(j) + "image", canny_image);
        findContours(canny_image, mycontours, myhierancy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
        
        //�����������
        vector<vector<Point>>myoutputarray(mycontours.size());
        //cout << "��" << j << "��СΪ" << mycontours.size() << endl;
        vector<Rect>mrboundrect(mycontours.size());
       
        int i = 0;
        int num = 0;
        int class_index = -1;
        for (int i = 0; i < mycontours.size(); i++)
        {
            approxPolyDP(mycontours[i], myoutputarray[i], 0, true);  //����forѭ������
            mrboundrect[i] = boundingRect(myoutputarray[i]);  //�ҳ���С��ת����
            Point point1 = mrboundrect[i].tl();
            Point point2 = mrboundrect[i].br();
            int area = contourArea(myoutputarray[i], true);
            if (point1.x < 142 || point1.y < 4 || point2.x > 600 || point2.y > 450)
                continue;
            else
            { 
                if (abs(area) <1000)
                    continue;

                if (j == 0)
                {
                    if (abs(area) > 6000)
                    {
                       /* num++;
                        if(num>10)*/
                        cout << "bananan" << endl;
                        putText(first_image, "banana " , Point(mrboundrect[i].tl().x, mrboundrect[i].tl().y-15), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
//rectangle(first_image, mrboundrect[i].tl(), mrboundrect[i].br(), Scalar(0, 0, 255), 2, 8);
                        return 1;

                    }
                    else if (abs(area) > 3360 && abs(area) < 3800)
                    {
                      
                        cout << "mango" << endl;
                        putText(first_image, "mango ", Point(mrboundrect[i].tl().x, mrboundrect[i].tl().y - 15), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
//rectangle(first_image, mrboundrect[i].tl(), mrboundrect[i].br(), Scalar(0, 0, 255), 2, 8);
                        return 2;
                    }   
                } 
                           
                if (j == 1)
                {
                    if (abs(area) > 1000)
                    {
                        cout << "pear" << endl;
                       putText(first_image, "pear " , Point(mrboundrect[i].tl().x, mrboundrect[i].tl().y-15), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
//rectangle(first_image, mrboundrect[i].tl(), mrboundrect[i].br(), Scalar(0, 0, 255), 2, 8);
                        return 3;
                    }

                        
                }
                if (j == 2)
                {
                    if (abs(area) > 2100&&abs(area) <2700)
                    {
                        cout << "nectarinee" << endl;
                         putText(first_image, "nectarinee " , Point(mrboundrect[i].tl().x, mrboundrect[i].tl().y-15), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
//rectangle(first_image, mrboundrect[i].tl(), mrboundrect[i].br(), Scalar(0, 0, 255), 2, 8);
                        return 4;
                    }
                    else if(abs(area) > 5000&&abs(area) <6400)
                    {
                        cout << "apple" << endl;
                         putText(first_image, "apple " , Point(mrboundrect[i].tl().x, mrboundrect[i].tl().y-15), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
//rectangle(first_image, mrboundrect[i].tl(), mrboundrect[i].br(), Scalar(0, 0, 255), 2, 8);
                        return 5;
                    }
                }               
              //  cout << abs(area) << " " << endl;
              
               // drawContours(first_image, mycontours, i, Scalar(0, 0, 255), 2, 8);
                rectangle(first_image, mrboundrect[i].tl(), mrboundrect[i].br(), Scalar(0, 0, 255), 2, 8);
                x = mrboundrect[i].tl().x;
                y = mrboundrect[i].tl().y;
            }
        }
    }
   
  //  namedWindow("canny_contours", WINDOW_NORMAL);
    namedWindow("counters_image", WINDOW_NORMAL);
  //  imshow("canny_contours", canny_image);
    imshow("counters_image", first_image);
}





