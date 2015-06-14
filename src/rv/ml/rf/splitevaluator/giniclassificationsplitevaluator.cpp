#include <rv/ml/rf/splitevaluator/classificationsplitevaluator.h>

float rv::rf::GiniClassificationSplitEvaluator::Purity(
    const std::vector<rv::rf::TargetPtr>& targets) const {
  
  int rows = targets[0]->data()->rows();
  int cols = targets[0]->data()->cols();
  RfMat f(rows, cols);
  f.setZero();
  
  //compute frequency
  double sum = 0;
  for(size_t tidx = 0; tidx < targets.size(); ++tidx) {
    RfMatPtr data = targets[tidx]->data();
    f = f + (*data);
    sum += data->sum();
  }
  f = f * (1.0 / sum);
  
  //compute gini
  float g = 0;
  for(int row = 0; row < rows; ++row) {
    for(int col = 0; col < cols; ++col) {
      float f_i = 0;
      if(sum > 0) {
        f_i = (f)(row, col) / sum;
      }
      
      g = g + f_i * f_i;
    }
  }
  
  g = 1.0 - g;
  
  return g;
}
