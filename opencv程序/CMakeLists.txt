cmake_minimum_required (VERSION 2.8)
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
project (demo)

include_directories(
${PROJECT_SOURCE_DIR}/include
)

#连接工业派的库目录
LINK_DIRECTORIES(/home/lizhou/ti-processor-sdk-linux-rt-am57xx-evm-05.03.00.07/linux-devkit/sysroots/armv7ahf-neon-linux-gnueabi/usr/lib)


add_executable(fruit_test fruit.cpp project_main.cpp)
# 将库文件链接到可执行程序上
#target_link_libraries( fruit_test libopencv_core.so libopencv_imgcodecs.so libopencv_imgproc.so #libopencv_highgui.so libopencv_calib3d.so libopencv_dnn.so libopencv_cvv.so)
target_link_libraries(fruit_test libopencv_aruco.so libopencv_plot.so libopencv_bgsegm.so libopencv_reg.so libopencv_bioinspired.so libopencv_rgbd.so libopencv_calib3d.so libopencv_saliency.so libopencv_ccalib.so libopencv_sfm.so libopencv_core.so libopencv_shape.so libopencv_cvv.so         libopencv_stereo.so libopencv_datasets.so libopencv_stitching.so libopencv_dpm.so              libopencv_structured_light.so libopencv_face.so libopencv_superres.so libopencv_features2d.so       
libopencv_surface_matching.so libopencv_flann.so libopencv_text.so libopencv_fuzzy.so            libopencv_tracking.so libopencv_highgui.so libopencv_videoio.so libopencv_imgcodecs.so        
libopencv_video.so libopencv_imgproc.so libopencv_videostab.so libopencv_line_descriptor.so  
libopencv_xfeatures2d.so libopencv_ml.so libopencv_ximgproc.so libopencv_objdetect.so        
libopencv_xobjdetect.so libopencv_optflow.so libopencv_xphoto.so libopencv_photo.so)
















