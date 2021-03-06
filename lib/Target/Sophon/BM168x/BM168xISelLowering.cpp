//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#include "BM168xISelLowering.h"
#include "TGConv.h"
#include "TGGemm.h"
#include "TGLRN.h"
#include "TGMaxPool.h"
#include "TGRelu.h"
#include "TGSoftmax.h"

#define DEBUG_TYPE "bm168x_lowering"
#include <onnc/Support/Debug.h>

using namespace onnc;

ComputeOperator2 *BM168xTargetLowering::LowerHelper(const xNode &pNode)
{
  uint32_t symbol = pNode.kind();
  if (symbol == xSymbol("Conv"))
    return new TGConv(pNode);
  else if (symbol == xSymbol("Relu"))
    return new TGRelu(pNode);
  else if (symbol == xSymbol("LRN"))
    return new TGLRN(pNode);
  else if (symbol == xSymbol("MaxPool"))
    return new TGMaxPool(pNode);
  else if (symbol == xSymbol("Gemm"))
    return new TGGemm(pNode);
  else if (symbol == xSymbol("Softmax"))
    return new TGSoftmax(pNode);
  DEBUG(dbgs() << "unsupported node type: " << pNode.kind().toString()
               << std::endl;);
  return nullptr;
}

ComputeOperator2 *BM168xTargetLowering::LowerOperation(
    const xNode &pNode,
    std::vector<std::unique_ptr<ComputeOperator2> > &pInstList)
{
  ComputeOperator2 *oper = LowerHelper(pNode);
  // FIXME ignore unsupported operation
  if (nullptr == oper)
    return nullptr;
  DEBUG(dbgs() << "lowering type: " << oper->getTypeName()
               << "\nlayer name:" << oper->getLayerName() << "\n";);
  return oper;
}
