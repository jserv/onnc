//===- BM188xComputeOperator.h---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef BM188X_COMPUTE_OPERATOR_H
#define BM188X_COMPUTE_OPERATOR_H

#include "ComputeOperator.h"
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>
#include <string>

namespace onnc {

class BM188xComputeOperator : public ComputeOperator2
{
public:
  BM188xComputeOperator(const ::onnx::Node &pNode, const std::string &pTypeName)
      : ComputeOperator2(pNode, pTypeName), m_pNode(&pNode)
  {
  }

  ~BM188xComputeOperator() override = default;

  void emit() const override { return; }

  // update instruction info by calibration table
  virtual void update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable)
  {
    return;
  }

protected:
  const ::onnx::Node *m_pNode; // NOLINT
};

} // namespace onnc

#endif // BM188X_COMPUTE_OPERATOR_H
