#ifndef RF_REGRESSIONLEAFNODEFCN_H
#define RF_REGRESSIONLEAFNODEFCN_H

#include <vector>
#include <iostream>

#include <boost/shared_ptr.hpp>

#include <rv/ml/rf/leafnodefcn/leafnodefcn.h>
#include <rv/stat/core.h>

namespace rv {
namespace rf {
    
//------------------------------------------------------------------------------
class MeanRegressionLeafNodeFcn : public LeafNodeFcn {        
public:
  MeanRegressionLeafNodeFcn() {}
  virtual ~MeanRegressionLeafNodeFcn() {}  
    
  virtual boost::shared_ptr<LeafNodeFcn> Copy() const {
    boost::shared_ptr<MeanRegressionLeafNodeFcn> fcn = boost::make_shared<MeanRegressionLeafNodeFcn>();
    fcn->stat_.resize(stat_.size());
    for(size_t stat_idx = 0; stat_idx < stat_.size(); ++stat_idx) {
      fcn->stat_[stat_idx] = CopyRfMat(stat_[stat_idx]);
    }
    
    return fcn;
  }

  virtual boost::shared_ptr<LeafNodeFcn> Create(
      const LeafNodeFcnParameter& param,
      const std::vector<SamplePtr>& samples, 
      const VecTargetPtr& targets) const;

  virtual VecTargetPtr Eval(const SamplePtr sample,
      const std::vector<boost::shared_ptr<LeafNodeFcn> >& fcns) const;
  
  virtual LeafNodeFcnParameter::LeafNodeFcnType type() const { return LeafNodeFcnParameter::REGRESSION_MEAN; }

DISABLE_COPY_AND_ASSIGN(MeanRegressionLeafNodeFcn);
};



//------------------------------------------------------------------------------
class MeanStdRegressionLeafNodeFcn : public MeanRegressionLeafNodeFcn {        
public:
  MeanStdRegressionLeafNodeFcn() {}
  virtual ~MeanStdRegressionLeafNodeFcn() {}  
    
  virtual boost::shared_ptr<LeafNodeFcn> Copy() const {
    boost::shared_ptr<MeanStdRegressionLeafNodeFcn> fcn = boost::make_shared<MeanStdRegressionLeafNodeFcn>();
    fcn->stat_.resize(stat_.size());
    for(size_t stat_idx = 0; stat_idx < stat_.size(); ++stat_idx) {
      fcn->stat_[stat_idx] = CopyRfMat(stat_[stat_idx]);
    }
    
    return fcn;
  }

  virtual boost::shared_ptr<LeafNodeFcn> Create(
      const LeafNodeFcnParameter& param,
      const std::vector<SamplePtr>& samples, 
      const VecTargetPtr& targets) const;

  virtual VecTargetPtr Eval(const SamplePtr sample,
      const std::vector<boost::shared_ptr<LeafNodeFcn> >& fcns) const;
  
  virtual LeafNodeFcnParameter::LeafNodeFcnType type() const { return LeafNodeFcnParameter::REGRESSION_MEAN_STD; }

DISABLE_COPY_AND_ASSIGN(MeanStdRegressionLeafNodeFcn);
};



//------------------------------------------------------------------------------
class MedianRegressionLeafNodeFcn : public LeafNodeFcn {        
public:
  MedianRegressionLeafNodeFcn() {}
  virtual ~MedianRegressionLeafNodeFcn() {}
    
  virtual boost::shared_ptr<LeafNodeFcn> Copy() const {
    boost::shared_ptr<MedianRegressionLeafNodeFcn> fcn = boost::make_shared<MedianRegressionLeafNodeFcn>();
    fcn->stat_.resize(stat_.size());
    for(size_t stat_idx = 0; stat_idx < stat_.size(); ++stat_idx) {
      fcn->stat_[stat_idx] = CopyRfMat(stat_[stat_idx]);
    }
    
    return fcn;
  }

  virtual boost::shared_ptr<LeafNodeFcn> Create(
      const LeafNodeFcnParameter& param,
      const std::vector<SamplePtr>& samples, 
      const VecTargetPtr& targets) const;

  virtual VecTargetPtr Eval(const SamplePtr sample,
      const std::vector<boost::shared_ptr<LeafNodeFcn> >& fcns) const;
  
  virtual LeafNodeFcnParameter::LeafNodeFcnType type() const { return LeafNodeFcnParameter::REGRESSION_MEDIAN; }

DISABLE_COPY_AND_ASSIGN(MedianRegressionLeafNodeFcn);
};
    


//------------------------------------------------------------------------------
class RegressionRegressionLeafNodeFcn : public LeafNodeFcn {        
public:
  RegressionRegressionLeafNodeFcn() {}
  virtual ~RegressionRegressionLeafNodeFcn() {}
    
  virtual boost::shared_ptr<LeafNodeFcn> Copy() const {
    boost::shared_ptr<RegressionRegressionLeafNodeFcn> fcn = boost::make_shared<RegressionRegressionLeafNodeFcn>();
    fcn->stat_.resize(stat_.size());
    for(size_t stat_idx = 0; stat_idx < stat_.size(); ++stat_idx) {
      fcn->stat_[stat_idx] = CopyRfMat(stat_[stat_idx]);
    }

    fcn->target_rows_ = target_rows_;
    fcn->target_cols_ = target_cols_;
    fcn->use_bias_ = use_bias_;
    fcn->poly_degree_ = poly_degree_;
    fcn->A_ = CopyRfMat(A_);
    
    return fcn;
  }

  virtual boost::shared_ptr<LeafNodeFcn> Create(
      const LeafNodeFcnParameter& param,
      const std::vector<SamplePtr>& samples, 
      const VecTargetPtr& targets) const;

  virtual VecTargetPtr Eval(const SamplePtr sample,
      const std::vector<boost::shared_ptr<LeafNodeFcn> >& fcns) const;
  
  virtual LeafNodeFcnParameter::LeafNodeFcnType type() const { return LeafNodeFcnParameter::REGRESSION_REGRESSION; }
  
  virtual void Save(rv::io::SerializationOut& ar) const {
    LeafNodeFcn::Save(ar);

    ar << target_rows_;
    ar << target_cols_;
    ar << use_bias_;
    ar << poly_degree_;

    int rows = A_->rows();
    int cols = A_->cols();

    ar << rows;
    ar << cols;

    for(int row = 0; row < rows; ++row) {
      for(int col = 0; col < cols; ++col) {
        float val = (*A_)(row, col);
        ar << val;
      }
    }
  }

  virtual void Load(rv::io::SerializationIn& ar) {
    LeafNodeFcn::Load(ar);
    
    ar >> target_rows_;
    ar >> target_cols_;
    ar >> use_bias_;
    ar >> poly_degree_;

    int rows, cols;
    ar >> rows;
    ar >> cols;

    A_ = CreateRfMat(rows, cols);

    for(int row = 0; row < rows; ++row) {
      for(int col = 0; col < cols; ++col) {
        float val;
        ar >> val;

        (*A_)(row, col) = val;
      }
    }
  }

  virtual RfMatPtr A() const { return A_; }
  virtual void set_A(RfMatPtr& A) { A_ = A; }

  virtual int target_rows() const { return target_rows_; }
  virtual int target_cols() const { return target_cols_; }
  virtual void set_target_rows(int target_rows) { target_rows_ = target_rows; }
  virtual void set_target_cols(int target_cols) { target_cols_ = target_cols; }

  virtual bool use_bias() const { return use_bias_; }
  virtual unsigned int poly_degree() const { return poly_degree_; }
  virtual void set_use_bias(bool use_bias) { use_bias_ = use_bias; }
  virtual void set_poly_degree(unsigned int poly_degree) { poly_degree_ = poly_degree; }

protected:
  RfMatPtr A_;
  int target_rows_;
  int target_cols_;
  bool use_bias_;
  unsigned int poly_degree_;

DISABLE_COPY_AND_ASSIGN(RegressionRegressionLeafNodeFcn);
};
    

}
}

#endif
