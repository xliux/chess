#include <stdio.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

DECLARE_bool(alsologtostderr);

int main(int argc, char **argv) {
  FLAGS_alsologtostderr = true;
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
