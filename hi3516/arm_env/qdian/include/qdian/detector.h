#include "opencv2/core.hpp"
#include "detect_struct.h"

using namespace cv;

#define LICENSE_VALID 1    // license 有效
#define LICENSE_INVALID 0  // license 无效
#define LICENSE_EXPIRED -1 // license 过期

extern "C"
{
/*
初始化检测器
const char *model_param_file: 模型参数文件，文件后缀 .param
const char *model_file  : 模型文件，文件后缀 .model
const char * license_file : 授权文件
 float  threshold_confidence ：置信度， 大于等于该置信度的被输出，缺省 0.4。取值范围 0-1 。该值越小漏检越小，误检越高
 float nms_threshold:   非最大抑制阀值，缺省 0.3 。取值范围 0-1 。该值越小漏检越小，误检越高

 返回值：1  初始化成功
            0   初始化失败
			-1  license无效或过期 ， 初始化失败

*/
int init_detector(const char *model_param_file,const char *model_file,const char * license_file, float  threshold_confidence, float nms_threshold);

/*
销毁检测器
 返回值：1  成功
            0   失败
*/
int destory_detector();

/*
检测图片
cv::Mat img：要检测的图片

 返回值：检测的结果
*/
std::vector<BoxInfo> detect( cv::Mat img);


////
/////////////////////// 授权相关
////

// 获取授权 key
// 使用该key 向客服获取授权文件 
// 客服微信号：qdianai
char *get_key();


/*
   char *license_file : license文件
    
    返回值：1 有效
            0 无效
			-1 过期 
    */
int check_license(const char *license_file);


bool is_license_valid();
}
