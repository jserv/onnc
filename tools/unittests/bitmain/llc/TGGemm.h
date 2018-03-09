#ifndef ONNX_BM_TGGEMM_H
#define ONNX_BM_TGGEMM_H

#include "onnx/common/ir.h"
#include "TGOperator.h"

class TGGemm : public TGOperator {
public:
  TGGemm(const onnx::Node &node, uint64_t offset);
  void dumpOnnxConv(const onnx::Node &node);
  virtual void emit(void) const;

private:
  std::vector<uint64_t> m_weightOffset;
  uint64_t m_inputAddr;
  uint64_t m_weightAddr;
  uint64_t m_biasAddr;
  uint64_t m_outputAddr;
  int m_inRowNum;
  int m_inColNum;
  int m_outColNum;
  int m_haveBias;
  int m_usingRelu;
  bool m_weightTp;
};

#endif
