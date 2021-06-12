#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "qdian/detector.h"

#include <stdio.h>
#include <dirent.h>
// #include "json/json.h"
#include "utils.h"

const char *class_names[] = {"bicyle","electromobile"};

int main(int argc, char **argv)
{

  int ret;
  ret = init_detector("electromobile_598.param", "electromobile_598.model", "lic.lic",0.35,0.2);
  if (ret == 1)
  {
    printf("init_detector OK\n");
  }
  else if (ret == 0)
  {
    printf("init_detector failed\n");
    // return -1;
  }
  else if (ret == -1)
  {
    printf("license failed\n");
    // return -1;
  }

  std::vector<std::string> file_names;

  if (read_files_in_dir("./img", file_names) < 0)
  {
    printf("read_files_in_dir failed.");
    return -1;
  }

  for (int f = 0; f < (int)file_names.size(); f++)
  {
    cv::Mat frame;
    std::string file_name = std::string("./img") + "/" + file_names[f];
    const char *img_path = file_name.c_str();
    std::cout << img_path << std::endl;
    cv::Mat img = cv::imread(img_path, 1);
    frame = cv::imread(img_path, 1);

    auto t0 = std::chrono::high_resolution_clock::now(); //计算推理时间

    std::vector<BoxInfo> result = detect(frame);

    auto t1 = std::chrono::high_resolution_clock::now();
    std::cout << "Detect time: " << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() / 1000.0 << "ms." << std::endl;

    //画图部分
    for (int i = 0; i < result.size(); i++)
    {
      const auto obj = result[i];
      fprintf(stderr, "%d = %.5f at %.2f %.2f %.2f x %.2f\n", obj.label, obj.score,
              obj.x1, obj.y1, obj.x2, obj.y2);
      cv::rectangle(frame, cvPoint(obj.x1, obj.y1), cvPoint(obj.x2, obj.y2), cv::Scalar(255, 0, 0));

      char text[256];
      

      sprintf(text, "%s %.1f%%", class_names[obj.label], obj.score * 100);

      int baseLine = 0;
      cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

      int x = obj.x1;
      int y = obj.y1 - label_size.height - baseLine;
      if (y < 0)
        y = 0;
      if (x + label_size.width > frame.cols)
        x = frame.cols - label_size.width;

      cv::rectangle(frame, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)),
                    cv::Scalar(255, 255, 255), -1);

      cv::putText(frame, text, cv::Point(x, y + label_size.height),
                  cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
    }

    std::string file_name_out = std::string("./result") + "/" + file_names[f];
    cv::imwrite(file_name_out.c_str(), frame);

  }

  return 0;
}
