#include <iostream>
#include <memory>

#include "../vision/include/vision_model.h"

using namespace vision_model;
using namespace std;

int main()
{
    auto vision_model_ = make_shared<VisionModel>();;
    return 0;
}